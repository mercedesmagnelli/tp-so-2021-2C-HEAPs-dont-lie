#include "manejo_hilos.h"

int thread_join_and_free(pthread_t thread_id) {
	void * resultado_hilo;

	pthread_join(thread_id, &resultado_hilo);

	if ((int) resultado_hilo != STATUS_OK) {
		loggear_error("Join del hilo devolvió un error, Error: %d",  (int) resultado_hilo);
	}

	return (int) resultado_hilo;
}

int thread_detach_and_free(pthread_t thread_id) {
	int resultado_hilo = pthread_detach(thread_id);

	if (resultado_hilo != STATUS_OK) {
		loggear_error("Detach del hilo devolvió un error, Error: %d",  resultado_hilo);
	}

	return resultado_hilo;
}

pthread_t thread_ejecutar_funcion( int (*log_function)() ) {
	pthread_t thread;

	pthread_create(&thread, NULL, (void *) log_function, NULL);

	return thread;
}
