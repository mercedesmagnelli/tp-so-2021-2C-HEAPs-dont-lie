#include "manejo_signals.h"

void manejar_sigint(){
	//char* nombre_archivo;
	//loggear_info("Se va a generar el archivo del dump, con el nombre %s", nombre_archivo);

	uint32_t total_miss = 0;
	uint32_t total_hits = 0;

	void sumar_miss(void* pr) {
		historico_procesos* p = (historico_procesos*) pr;
		loggear_trace("AHORA VOY A SUMARLE AL CONTADOR %d DEL PROCESO %d", p->miss, p->pid);
		total_miss+= (p->miss);
	}
	void sumar_hits(void* pr) {
		historico_procesos* p = (historico_procesos*) pr;
		loggear_trace("AHORA VOY A SUMARLE AL CONTADOR %d DEL PROCESO %d", p->hit, p->pid);
		total_hits+= (p->hit);
	}
	if(list_size(metricas) == 0){

		loggear_warning("[SIGINT] - La lista está vacia, no hay nada para calcular");
		loggear_info("CANTIDAD DE MISS TOTALES: 0");
		loggear_info("CANTIDAD DE HITS TOTALES: 0");

	}else{

		list_iterate(metricas,sumar_miss);

		list_iterate(metricas,sumar_hits);


	loggear_info("CANTIDAD DE MISS TOTALES: %zu", total_miss);
	loggear_info("CANTIDAD DE HITS TOTALES: %zu", total_hits);
	loggear_info("   PID   |   HITS   |   MISS   ");

	for (int i = 0; i <list_size(metricas);i++) {
		historico_procesos* p = (historico_procesos*) list_get(metricas, i);
		imprimir_entrada_proceso(p);
		}
	}
	imprimir_frames();
	semaforo_post_fin();

}

void imprimir_entrada_proceso(historico_procesos* p){

	loggear_info("    %d    |     %d    |     %d    ",p->pid, p->hit, p->miss);


}
void manejar_sigusr1(){
	loggear_info("[SIGUSR1] - Se va a generar el archivo dump en la ruta: %s");

	char* timestamp = temporal_get_string_time("%s%y%m%d%H%M%S.txt");
	char* ruta = string_from_format("DUMP_%s", timestamp);

	FILE* dump = fopen(ruta, "wb+");
	char** pid_pag;
	char* info_entrada;
	if(list_size(TLB) == 0){
		loggear_warning("[SIGUSR1] - La TLB no tiene ninguna entrada aún");
	}else{
	for(int i = 0; i < list_size(TLB); i++){
	entrada_tlb* e = (entrada_tlb*) list_get(TLB, i);
	pid_pag = string_split(e->hash_key, "-");
	uint32_t pid = atoi(pid_pag[0]);
	uint32_t pag = atoi(pid_pag[1]);
	info_entrada = string_from_format("Entrada: %d | Estado: Ocupado | Carpincho: %d | Pagina: %d | Marco %d \n", i,pid, pag, e->frame);
	fputs(info_entrada, dump);
	}
	if(list_size(TLB) < get_cantidad_entradas_tlb()) {
		for(int i = 0; i < (get_cantidad_entradas_tlb() - list_size(TLB)); i++) {
			info_entrada = string_from_format("Entrada: %d | Estado: Libre \n", i);
			fputs(info_entrada, dump);
		}
	}
}
	free(info_entrada);
	free(pid_pag[0]);
	free(pid_pag[1]);
	free(pid_pag);

	//semaforo_post_fin();

}

void manejar_sigusr2(){

	loggear_warning("[SIGUSR2] - Se van a limpiar todas las entradas de la tlb");
	if(list_size(TLB)==0) {
		loggear_warning("[SIGNURS2] - No hay ninguna entrada para eliminar");
	}else {
		limpiar_tlb();

	}

}
