#include "matelib.h"
#include <pthread.h>
#include <commons/string.h>

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


	void multi_hilo1(int * n) {
		mate_instance * lib_ref = malloc(sizeof(mate_instance));

		mate_init(lib_ref, string_from_format("Proceso %d", n));

		if (rand() % 10 > 5) {
			mate_sem_wait(lib_ref, "SEM_BBB");
		}
		mate_sem_wait(lib_ref, "SEM_CCC");

		mate_sem_wait(lib_ref, "SEM_AAA");

		loggear_trace("[PID: %zu] - En 10 segundos, hace SEM_POST", ((t_instance_metadata *) lib_ref->group_info)->pid);

		sleep(10);

		mate_sem_post(lib_ref, "SEM_AAA");
	}

	mate_instance * referencia = malloc(sizeof(mate_instance));

	mate_init(referencia, "Proceso 01");
	mate_sem_init(referencia, "SEM_AAA", 1);
	mate_sem_init(referencia, "SEM_BBB", 10);
	mate_sem_init(referencia, "SEM_CCC", 10);

	for (int i = 1; i < 5; ++i) {
		pthread_t thread;

		pthread_create(&thread, NULL, (void *) multi_hilo1, &i);
	}


	loggear_debug("En 30 segundos, Se destruye el semaforo");
	sleep(30);
	mate_sem_destroy(referencia, "SEM_AAA");
	mate_sem_destroy(referencia, "SEM_BBB");
	mate_sem_destroy(referencia, "SEM_CCC");

	sleep(100);

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

