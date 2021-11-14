#include "manejo_signals.h"

void manejar_sigint(){
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

	list_iterate(listaProcesos,sumar_miss);
	list_iterate(listaProcesos, sumar_hits);

	loggear_info("CANTIDAD DE MISS TOTALES: %d", total_miss);
	loggear_info("CANTIDAD DE HITS TOTALES: %d", total_hits);

	loggear_info("   PID   |   HITS   |   MISS   ");
	for (int i = 0; i <list_size(listaProcesos);i++) {
		t_proceso* p = (t_proceso*) list_get(listaProcesos, i);
		imprimir_entrada_proceso(p);
		}
	}
	cerrar_todo();
}


void cerrar_todo() {
	cerrar_conexiones(true); // Hasta que no se cierre el hilo que escuchan las notificaciones no apaga
	destroy_configuracion();
	destroy_log();
}


void imprimir_entrada_proceso(t_proceso* p){

	loggear_info("    %d    |     %d    |     %d    ",p->PID, p->hits_proceso, p->miss_proceso);


}
void manejar_sigusr1()
{
	//printf("soy el sigusr1");
}

void manejar_sigusr2(){
	//printf("soy el sigur2");

}
