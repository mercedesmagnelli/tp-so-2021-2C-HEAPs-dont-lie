#include "planificador_main.h"

void cerrar_todo();

void debug_variables();

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

	debug_variables();

	pthread_t ram_handshake = thread_ejecutar_funcion(ram_enviar_handshake);

	error = thread_join_and_free(ram_handshake);
	if (error != 0) {
		loggear_error("RAM no nos acepto, Error: %d", error);
		cerrar_todo();
		thread_detach_and_free(ram_handshake);
		return EXIT_FAILURE;
	}


	//levantar_consola();

	cerrar_todo();
	return EXIT_SUCCESS;
}

void cerrar_todo() {
	ram_cerrar_conexiones(false);
	loggear_info("Cerrada conexion con ram");
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
}

void debug_variables() {
	loggear_debug("---Variables en archivo de configuración---");
	loggear_debug("IP_MEMORIA: \t\t\t%s", get_ip_ram());
	loggear_debug("PUERTO_MEMORIA: \t\t%d", get_puerto_ram());
	loggear_debug("ALGORITMO_PLANIFICACION: \t%d", get_algoritmo());
	loggear_debug("CANTIDAD_DISPOSITIVOS_IO: \t%d", get_cantidad_dispositivos_io());
	for (int i = 0; i < get_cantidad_dispositivos_io(); ++i) {
		loggear_debug("DISPOSITIVOS_IO[%d]: \t\t%s", i, get_dispositivos_io()[i]);
	}
	loggear_debug("CANTIDAD_DURACIONES_IO: \t%d", get_cantidad_duraciones_io());
	for (int i = 0; i < get_cantidad_duraciones_io(); ++i) {
		loggear_debug("DURACIONES_IO[%d]: \t\t%s", i, get_duraciones_io()[i]);
	}
	loggear_debug("RETARDO_CPU: \t\t\t%d", get_retardo_cpu());
	loggear_debug("GRADO_MULTIPROGRAMACION: \t%d", get_grado_multiprogramacion());
	loggear_debug("GRADO_MULTIPROCESAMIENTO: \t%d", get_grado_multiprocesamiento());
	loggear_debug("LOG_ROUTE: \t\t\t%s", get_log_route());
	loggear_debug("LOG_APP_NAME: \t\t\t%s", get_log_app_name());
	loggear_debug("LOG_IN_CONSOLE: \t\t%d", get_log_in_console());
	loggear_debug("LOG_LEVEL_INFO: \t\t%d", get_log_level_info());
}

