#include "filesystem_main.h"

void cerrar_todo();
void manejar_signal(int n);

int main(int argc, char** argv) {
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

	// TODO: Iniciar el Filesystem

	int servidor = levantar_servidor();
	if (servidor != 0) {
		cerrar_todo();
		return EXIT_FAILURE;
	}

	signal(SIGUSR1, manejar_signal);

	cerrar_todo();

	return EXIT_SUCCESS;
}

void cerrar_todo() {
	cerrar_conexiones(true); // Hasta que no se cierre el hilo que escuchan las notificaciones no apaga
	destroy_configuracion();
	destroy_log();
}

void manejar_signal(int n){
	loggear_warning("Llego la signal para mandar sabotaje, TODO: Cambiar la funcion en filesystem_main.c:35 para que cumpla la funcion");
}
