#include "planificador_main.h"

void cerrar_todo();

int main(int argc, char** argv) {

	int error = 0;

	error = iniciar_configuracion(argc, argv);
	if (error != STATUS_OK) {
		puts("Error en los argumentos\n");
		return EXIT_FAILURE;
	}

	error = init_mutex_log(get_log_route(), get_log_app_name(), get_log_in_console(), LOG_LEVEL_TRACE);
	if (error != STATUS_OK) {
		puts("Error al crear el logger\n");
		cerrar_todo();
		return EXIT_FAILURE;
	}

	pthread_t filesystem_handshake = thread_ejecutar_funcion(filesystem_enviar_handshake);
	pthread_t ram_handshake = thread_ejecutar_funcion(ram_enviar_handshake);

	error = thread_join_and_free(filesystem_handshake);
	if (error != 0) {
		loggear_error("Filesystem no nos acepto, Error: %d", error);
		cerrar_todo();
		thread_detach_and_free(filesystem_handshake);
		return EXIT_FAILURE;
	}

	error = thread_join_and_free(ram_handshake);
	if (error != 0) {
		loggear_error("RAM no nos acepto, Error: %d", error);
		cerrar_todo();
		thread_detach_and_free(ram_handshake);
		return EXIT_FAILURE;
	}

	// TODO: Iniciar planificador

	levantar_consola();

	cerrar_todo();
	return EXIT_SUCCESS;
}

void cerrar_todo() {
	filesystem_cerrar_conexiones(false);
	loggear_info("Cerrada conexion con filesystem");
	ram_cerrar_conexiones(false);
	loggear_info("Cerrada conexion con ram");
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
}
