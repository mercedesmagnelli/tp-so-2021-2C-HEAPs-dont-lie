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


	// pid 2
	void multi_hilo1(int * n) {
		mate_instance * lib_ref = malloc(sizeof(mate_instance));

		mate_init(lib_ref, string_from_format("Proceso %d", n));

		mate_sem_wait(lib_ref, "SEM_CCC");

		sleep(3);

		mate_sem_wait(lib_ref, "SEM_BBB");

		mate_sem_wait(lib_ref, "SEM_AAA");

		loggear_trace("[PID: %zu] - En 10 segundos, hace SEM_POST", ((t_instance_metadata *) lib_ref->group_info)->pid);

		loggear_trace("[PID: %zu] - SEM_POST A SEM_AAA", ((t_instance_metadata *) lib_ref->group_info)->pid);

		mate_sem_post(lib_ref, "SEM_AAA");

		mate_close(lib_ref);
	}

	// pid 3
	void multi_hilo2(int * n) {
		mate_instance * lib_ref = malloc(sizeof(mate_instance));

		mate_init(lib_ref, string_from_format("Proceso %d", n));

		mate_sem_wait(lib_ref, "SEM_AAA");
		mate_sem_wait(lib_ref, "SEM_BBB");

		sleep(5);

		mate_sem_wait(lib_ref, "SEM_CCC");


		loggear_trace("[PID: %zu] - En 10 segundos, hace SEM_POST", ((t_instance_metadata *) lib_ref->group_info)->pid);

		loggear_trace("[PID: %zu] - SEM_POST a SEM_AAA", ((t_instance_metadata *) lib_ref->group_info)->pid);

		mate_sem_post(lib_ref, "SEM_AAA");

		mate_close(lib_ref);
	}

	// pid 4
	void multi_hilo3(int * n) {
		mate_instance * lib_ref = malloc(sizeof(mate_instance));

		mate_init(lib_ref, string_from_format("Proceso %d", n));

		sleep(2);

		mate_sem_wait(lib_ref, "SEM_AAA");

		sleep(3);
		mate_sem_wait(lib_ref, "SEM_BBB");
		mate_sem_wait(lib_ref, "SEM_CCC");

		loggear_trace("[PID: %zu] - En 10 segundos, hace SEM_POST", ((t_instance_metadata *) lib_ref->group_info)->pid);

		loggear_trace("[PID: %zu] - SEM_POST A SEM_AAA", ((t_instance_metadata *) lib_ref->group_info)->pid);

		mate_sem_post(lib_ref, "SEM_AAA");

		mate_close(lib_ref);
	}

	mate_instance * referencia = malloc(sizeof(mate_instance));

	mate_init(referencia, "Proceso 01");
	mate_sem_init(referencia, "SEM_AAA", 1);
	mate_sem_init(referencia, "SEM_BBB", 1);
	mate_sem_init(referencia, "SEM_CCC", 1);

	pthread_t thread_uno;
	pthread_t thread_dos;
	pthread_t thread_tres;

	int uno = 10;
	int dos = 20;
	int tres = 30;

	pthread_create(&thread_uno, NULL, (void *) multi_hilo1, &uno);
	pthread_create(&thread_dos, NULL, (void *) multi_hilo2, &dos);
	pthread_create(&thread_tres, NULL, (void *) multi_hilo3, &tres);

	loggear_debug("Esperamos a que finalice 1");
	pthread_join(thread_uno, NULL);
	loggear_debug("Esperamos a que finalice 2");
	pthread_join(thread_dos, NULL);
	loggear_debug("Esperamos a que finalice 3");
	pthread_join(thread_tres, NULL);

	mate_sem_destroy(referencia, "SEM_AAA");
	mate_sem_destroy(referencia, "SEM_BBB");
	mate_sem_destroy(referencia, "SEM_CCC");

	mate_close(referencia);

	loggear_warning("ESperamos 20 segundos para cerrar todo");
	sleep(20);
	loggear_warning("PAsaron 20 segundos");

	cerrar_todo();
	return EXIT_SUCCESS;
}

void cerrar_todo() {
	conexiones_cerrar_conexiones();
	loggear_info("Cerrada conexion con backend");
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
}

