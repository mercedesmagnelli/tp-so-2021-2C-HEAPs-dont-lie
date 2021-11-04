#include "ram_main.h"

void cerrar_todo();
void signal_handler(int n);
void debug_configuracion();


int main(int argc, char** argv) {

	// TODO: Implementar signal de CtrlC y  tal vez CtrlZ
	signal(SIGUSR1, signal_handler);
	int error = iniciar_configuracion(argc, argv);
	/*if (error != STATUS_OK) {
		puts("Error en los argumentos\n");
		return EXIT_FAILURE;
	}*/

	error = init_mutex_log(get_log_route(), get_log_app_name(), get_log_in_console(), get_log_level_info());
	if (error != STATUS_OK) {
		puts("Error al crear el logger\n");
		cerrar_todo();
		return EXIT_FAILURE;
	}

	debug_configuracion();

	error = levantar_servidor();
	if (error != STATUS_OK) {
		cerrar_todo();
		return EXIT_FAILURE;
	}

	inicializar_estructuras_administrativas();

	cerrar_todo();
	destruir_estructuras_administrativas();

	return EXIT_SUCCESS;
}



void cerrar_todo() {
	cerrar_conexiones(true); // Hasta que no se cierre el hilo que escuchan las notificaciones no apaga  
	destroy_configuracion();
	destroy_log();
}

void signal_handler(int n){
	loggear_warning("Llego un SIGNAL, n: %d", n);
}

void debug_configuracion() {
	loggear_debug("---Variables en archivo de configuración---");
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
