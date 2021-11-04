#include "matelib.h"

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





	mate_instance* lib_ref = malloc(sizeof(mate_instance));

	mate_init(lib_ref, "hola");

/*
	t_instance_metadata* metadata = malloc(sizeof(t_instance_metadata));
	metadata->pid = generar_pid();

	lib_ref->group_info = metadata;

	t_instance_metadata* nueva_metadata = (t_instance_metadata*) lib_ref->group_info;

	loggear_debug("%d", nueva_metadata->pid);
*/
	//cerrar_todo();
	return EXIT_SUCCESS;
}

void cerrar_todo() {
	conexiones_cerrar_conexiones(false);
	loggear_info("Cerrada conexion con backend");
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
}

