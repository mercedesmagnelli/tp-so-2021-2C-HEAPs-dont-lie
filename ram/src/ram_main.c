#include "ram_main.h"

void cerrar_todo();

void signal_handler(int n){
	loggear_warning("Llego un SIGNAL, n: %d", n);
}

int main(int argc, char** argv) {
	// TODO: Implementar signal de CtrlC y  tal vez CtrlZ
	signal(SIGUSR1, signal_handler);

	int error = iniciar_configuracion(argc, argv);
	if (error != STATUS_OK) {
		puts("Error en los argumentos\n");
		return EXIT_FAILURE;
	}

	error = init_mutex_log(get_log_route(), get_log_app_name(), get_log_in_console(), get_log_level_info());
	if (error != STATUS_OK) {
		puts("Error al crear el logger\n");
		cerrar_todo();
		return EXIT_FAILURE;
	}

	error = levantar_servidor();
	if (error != STATUS_OK) {
		cerrar_todo();
		return EXIT_FAILURE;
	}

	cerrar_todo();

	return EXIT_SUCCESS;
}

void cerrar_todo() {
	cerrar_conexiones(true); // Hasta que no se cierre el hilo que escuchan las notificaciones no apaga  
	destroy_configuracion();
	destroy_log();
}


