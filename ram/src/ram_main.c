#include "ram_main.h"

void debug_configuracion();
void imprimir_hdm(t_list* lista_heaps);
void imprimir_procesos();
void testeamos();
void imprimir_frames();

void manejar_signal() {
	signal(SIGINT, manejar_sigint);
	signal(SIGUSR1, manejar_sigusr1);
	signal(SIGUSR2, manejar_sigusr2);


}

int main(int argc, char** argv) {


	semaforo_iniciar();
	pthread_t hilo;
	pthread_create(&hilo, NULL, (void *) manejar_signal, NULL);
	pthread_detach(hilo);


	int error = iniciar_configuracion(argc, argv);

	error = init_mutex_log(get_log_route(), get_log_app_name(), get_log_in_console(), get_log_level_info());
	if (error != STATUS_OK) {
		puts("Error al crear el logger\n");
		cerrar_todo(true);
		return EXIT_FAILURE;
	}

	debug_configuracion();

	inicializar_estructuras_administrativas();

	error = levantar_servidor();

	if (error != STATUS_OK) {
		cerrar_todo(true);
		return EXIT_FAILURE;
	}

	//testeamos();

	semaforo_wait_fin();

	cerrar_todo(false);
	//destruir_estructuras_administrativas();

	return EXIT_SUCCESS;
}

void signal_handler(int n){


	manejar_sigint();
	loggear_trace("ya termine de manejar el sigint");
//	int a = pthread_join(hilo_senial, NULL);
//	loggear_trace("termine de joinear");
//	if (a !=0 ) {
//		loggear_error("hubo un problema joineando el hilo de las seniales");
//	}
	cancelar_notificaciones();
	//return x;
	//cerrar_todo();
			/*switch (n){
				case SIGUSR1:
					manejar_sigusr1();
						break;
				case SIGUSR2:
					manejar_sigusr2();
						break;
				case SIGINT:
					manejar_sigint();
					pthread_join(hilo_senial, NULL);
					//cerrar_todo();
					break;
			}*/
}

void* manejo_seniales(void* s) {
	signal(SIGINT,signal_handler);

	//signal(SIGUSR1,signal_handler);
	//signal(SIGUSR2,signal_handler);
	return NULL;
}


void cerrar_todo(bool cierro) {
	cerrar_conexiones(cierro); // Hasta que no se cierre el hilo que escuchan las notificaciones no apaga
	destruir_estructuras_administrativas();
	destroy_configuracion();
	destroy_log();
}



void debug_configuracion() {
	loggear_debug("---Variables en archivo de configuraci??n---");
	loggear_debug("PUERTO:\t\t\t\t%d", get_puerto());
	loggear_debug("TAMANIO_MEMORIA:\t\t%d", get_tamanio_memoria());
	loggear_debug("ALGORITMO_REEMPLAZO_MMU:\t%d", get_algoritmo_reemplazo_mmu());
	loggear_debug("ALGORITMO_REEMPLAZO_TLB:\t%d", get_algoritmo_reemplazo_tlb());
	loggear_debug("TIPO_ASIGNACION:\t\t%d", get_tipo_asignacion());
	loggear_debug("MARCOS_MAXIMOS:\t\t\t%d", get_marcos_maximos());
	loggear_debug("CANTIDAD_ENTRADAS_TLB:\t\t%d", get_cantidad_entradas_tlb());
	loggear_debug("RETARDO_ACIERTO_TLB:\t\t%d", get_retardo_acierto_tlb());
	loggear_debug("RETARDO_FALLO_TLB:\t\t%d", get_retardo_fallo_tlb());
	loggear_debug("LOG_ROUTE: \t\t\t%s", get_log_route());
	loggear_debug("LOG_APP_NAME: \t\t\t%s", get_log_app_name());
	loggear_debug("LOG_IN_CONSOLE: \t\t%d", get_log_in_console());
	loggear_debug("LOG_LEVEL_INFO: \t\t%d", get_log_level_info());
}


void testeamos() {
		uint32_t pid = 1;
		sleep(10);

		inicializar_proceso(pid);
		inicializar_proceso(pid+1);
		//inicializar_proceso(pid+2);

		PID_listo(pid);
		PID_listo(pid+1);
	    int a = memalloc(pid,23);
	    loggear_trace("Hice el memalloc en %d", a);

	    memwrite("holissss", a, pid, sizeof("holissss"));

	    a = memalloc(pid+1,23);
	    loggear_trace("Hice el memalloc en %d", a);

	    close_PID(pid);

	    void * lectura = malloc(sizeof("holissss"));
	    int x =  memread(9, pid, sizeof("holissss"), &lectura);
	    if(x < 0) {
	    	loggear_error("error, no puedo leer algo de un proceso inexistente");
	    }

	   // a = memalloc(pid+2,23);
	   	//loggear_trace("Hice el memalloc en %d", a);

 imprimir_frames();
	    close_PID(pid);
    imprimir_frames();
    close_PID(pid+1);
//	  	imprimir_frames();
//	  	close_PID(pid+2);
//	  	imprimir_frames();


//	    inicializar_proceso(pid+1);
//	 //   PID_listo(pid+1);
////
////        //pido malloc
//	    a = memalloc(pid+1,23);
//		loggear_trace("Hice el memalloc en %d", a);
////
////        a = memalloc(pid+1,23);
////        loggear_trace("Hice el memalloc en %d", a);
////
////
////        a = memalloc(pid,23);
//         loggear_trace("Hice el memalloc en %d", a);
//
//         a = memalloc(pid+1,23);
//         loggear_trace("Hice el memalloc en %d", a);
//
//         if(inicializar_proceso(pid+2) == 0){
//         PID_listo(pid+2);
//        a = memalloc(pid+2,23);
//        loggear_trace("Hice el memalloc en %d", a);
//         }
//       imprimir_tlb();
//       imprimir_frames();

//	//escribo
//	char* pedido_Dora = "??puedes hacerme un sanwitch? e_e";
//	void* sandwitch = malloc(33);
//	memcpy(sandwitch, pedido_Dora,33);
//	loggear_info("lo que voy a guardar en memoria es %s", ((char*) sandwitch));
//
//	memwrite(sandwitch, a, pid, 33);
//	free(sandwitch);
//	imprimir_tlb();
//	imprimir_frames();
//
//	void* sandwitch2 = malloc(33);
//	memcpy(sandwitch2, memoria_principal + 9, 33);
//	loggear_info("lo leido manualmente luego del memwrite fue %s", ((char*) sandwitch2));
//	sleep(3);
//	free(sandwitch2);
//
//	suspender_PID(pid);
//	imprimir_tlb();
//	imprimir_frames();
//
//	void* sandwitch3 = malloc(33);
//	memcpy(sandwitch3, memoria_principal + 9, 33);
//	loggear_info("lo leido manualmente dsps de suspender fue %s", ((char*) sandwitch3));
//	sleep(3);
//	free(sandwitch3);
//
//	inicializar_proceso(pid+1);
//	//pido malloc
//	int b = memalloc(pid+1,33);
//	loggear_trace("Hice el memalloc en %d", b);
//	imprimir_tlb();
//	imprimir_frames();
//
//	void* sandwitch4 = malloc(33);
//	memcpy(sandwitch4, memoria_principal + 9, 33);
//	loggear_info("lo leido manualmente fue %s", ((char*) sandwitch4));
//	sleep(3);
//	free(sandwitch4);
//
//	//leo
//	void* ptro_tengo_q_leer;
//	memread(a, pid, 34, &ptro_tengo_q_leer);
//	loggear_info("lo leido de memoria fue %s", ((char*) ptro_tengo_q_leer));
//
//	imprimir_tlb();
//	imprimir_frames();

//	    //variante 2
//
//	    //escribo
//
//	    //malloc 1
//
//	    //leo


//	    a = memalloc(pid + 1, 23);
//	    loggear_trace("Hice el memalloc en %d", a);
//	    imprimir_tlb();
//
//	    a = memalloc(pid + 2, 23);
//	   	loggear_trace("Hice el memalloc en %d", a);
//	   	imprimir_tlb();
//
//	    a = memalloc(pid + 3, 23);
//	  	loggear_trace("Hice el memalloc en %d", a);
//
//	   	imprimir_procesos();
//
//	    a = memalloc(pid + 4, 23);
//	  	loggear_trace("Hice el memalloc en %d", a);

//		PRUEBAS UTILIZADAS PARA LRU

//	  	int a = memalloc(pid,10);
//		loggear_trace("Hice el memalloc en %d", a);
//		imprimir_tlb();
//
//		a = memalloc(pid, 5);
//		loggear_trace("Hice el memalloc en %d", a);
//		imprimir_tlb();
//
//		a = memalloc(pid, 22);
//		loggear_trace("Hice el memalloc en %d", a);
//		imprimir_tlb();
//
//		a = memalloc(pid, 23);
//		loggear_trace("Hice el memalloc en %d", a);
//		imprimir_tlb();
//
//		imprimir_procesos();




//	    loggear_trace("--PROCESOS ANTES DE HACER CUALQUIER TIPO DE LIBERACION--");
//
//	    imprimir_procesos();
//
//	    loggear_trace("----AHORA VOY A  PROBAR LA LIBERACI??N DE ALLOCS------");
//
//	    memfree(30, pid);
//	    loggear_trace("-- PROCESOS DESPU??S DE LIBERAR EL HEAP DE 9 --");
//
//	    imprimir_procesos();

//    loggear_trace("----AHORA VOY A ESCRIBIR-----");
//
//    uint32_t x = memwrite("Hola", a, pid, 5);
//
//    loggear_trace("termine de escribir");
//
//    if(x < 0) {
//    	loggear_error("fua le chingue en la escritura");
//    }
//
//    loggear_debug("Me voy a mimir 3 segundos porque me canse de escribir Hola");
//    	sleep(3);
//
//    void* ptroLectura = malloc(5);
//	memcpy(ptroLectura, memoria_principal + 9, 5);
//	char* leido = (char*) ptroLectura;
//	loggear_info("lo leido manualmente fue %s", leido);
//
//    void* lectura = NULL;
//    loggear_trace("antes del memread");
//    int s = memread(a, pid, 5, &lectura);
//    loggear_trace("dsp del memread");
//	 if(s < 0) {
//			loggear_error("fua le chingue en la lectura");
//		}else{
//		loggear_debug("Lei bien! %s", (char*) lectura);
//
//
//	}
//	 	 sleep(3);

//    memwrite("Chau", 41, pid, 5);
//    loggear_debug("Me voy a mimir 2 segundos porque me canse de escribir Chau");
//    sleep(2000);
//
//    memwrite("Carpincho", 73, pid, 10);
//    loggear_debug("Me voy a mimir 2 segundos porque me canse de escribir Carpincho");
//    sleep(2000);
//
//    memwrite("Capibara",105, pid, 10);
//    loggear_debug("Me voy a mimir 2 segundos porque me canse de escribir Capibara");
//    sleep(2000);
//
//    memwrite("Hydrochaeris",105, pid, 13);
//    loggear_debug("Me voy a mimir 2 segundos porque me canse de escribir Hydrochaeris");
//    sleep(2000);



}

void imprimir_frames() {

	for (int i = 0; i < list_size(listaFrames); i++) {
		t_frame* frame = (t_frame*)list_get(listaFrames, i);
		loggear_debug("FRAME:%d | ESTADO: %d | PROCESO: %d | PAGINA : %d", frame->nroFrame, frame->estado, frame->proceso, frame->pagina);

	}

}

void imprimir_procesos() {


    for (int i = 0; i < list_size(listaProcesos); i++){
        t_proceso* proc = (t_proceso*) list_get(listaProcesos, i);
        loggear_warning("El proceso %d est?? ocupando %d paginas", proc->PID, list_size(proc->tabla_paginas));
        loggear_warning("Los heaps del proceso %d son: \n", proc->PID);
        imprimir_hdm(proc->lista_hmd);

    }

}

void imprimir_hdm(t_list* lista_heaps) {
    heap_metadata* heap_actual;
    for(int i = 0; i < list_size(lista_heaps); i++){
        heap_actual = (heap_metadata*) list_get(lista_heaps, i);
        loggear_warning("El HEAP %d tiene los siguientes datos", i);
        loggear_warning("CurrAlloc     = %d", heap_actual->currAlloc);
        loggear_warning("PrevAlloc     = %d", heap_actual->prevAlloc);
        loggear_warning("NextAlloc     = %d", heap_actual ->nextAlloc);
        loggear_warning("IsFree     = %d", heap_actual->isFree);
    }
}
