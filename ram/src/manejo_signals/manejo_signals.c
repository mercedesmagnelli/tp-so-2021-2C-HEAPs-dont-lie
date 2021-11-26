#include "manejo_signals.h"

void* manejar_sigint(){
	//char* nombre_archivo;
	//loggear_info("Se va a generar el archivo del dump, con el nombre %s", nombre_archivo);

	uint32_t total_miss;
	uint32_t total_hits;

	void sumar_miss(void* pr) {
		t_proceso* p = (t_proceso*) pr;
		total_miss+= (p->miss_proceso);
	}
	void sumar_hits(void* pr) {
		t_proceso* p = (t_proceso*) pr;
		total_hits+= (p->hits_proceso);
	}
	if(list_size(listaProcesos) == 0){
		loggear_warning("[SIGINT] - La lista está vacia, no hay nada para calcular");
		loggear_info("CANTIDAD DE MISS TOTALES: 0");
		loggear_info("CANTIDAD DE HITS TOTALES: 0");
	}else{
		loggear_trace("aber");
	list_iterate(listaProcesos,sumar_miss);
	loggear_trace("aber2");
	list_iterate(listaProcesos, sumar_hits);
	loggear_trace("aber33");

	loggear_info("CANTIDAD DE MISS TOTALES: %d", total_miss);
	loggear_info("CANTIDAD DE HITS TOTALES: %d", total_hits);

	loggear_info("   PID   |   HITS   |   MISS   ");
	for (int i = 0; i <list_size(listaProcesos);i++) {
		t_proceso* p = (t_proceso*) list_get(listaProcesos, i);
		imprimir_entrada_proceso(p);
		}
	}
	loggear_trace("YA TERMINE DE IMPRIMIR TODO");
	return NULL;
}

void imprimir_entrada_proceso(t_proceso* p){

	loggear_info("    %d    |     %d    |     %d    ",p->PID, p->hits_proceso, p->miss_proceso);


}
void manejar_sigusr1(){

	char* timestamp = temporal_get_string_time("%s%y%m%d%H%M%S.txt");
	char* ruta = string_from_format("DUMP_%s", timestamp);
	loggear_info("[SIGUSR1] - Se va a generar el archivo dump en la ruta: %s");
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
			info_entrada = string_from_format("Entrada: %d | Estado: Ocupado", i);
			fputs(info_entrada, dump);
		}
	}
}
	free(info_entrada);
	free(pid_pag[0]);
	free(pid_pag[1]);
	free(pid_pag);


}

void manejar_sigusr2(){

	loggear_warning("[SIGUSR2] - Se van a limpiar todas las entradas de la tlb");
	if(list_size(TLB)==0) {
		loggear_warning("[SIGNURS2] - No hay ninguna entrada para eliminar");
	}else {
		limpiar_tlb();

	}

}
