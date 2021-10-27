#include "hilos_planificador.h"

sem_t hilos_semaforo_new;

sem_t hilos_semaforo_ready;
sem_t hilos_grado_multiprogramacion;

sem_t hilos_semaforo_exec;
sem_t hilos_grado_multitarea;

t_list * list_hilos_ejecutando;
pthread_mutex_t mutex_hilos;

int hilos_planificador_iniciar() {
	list_hilos_ejecutando = list_create();

	sem_init(&hilos_semaforo_new, 0, 0);

	sem_init(&hilos_semaforo_ready, 0, 0);
	sem_init(&hilos_grado_multiprogramacion, 0, get_grado_multiprogramacion());

	sem_init(&hilos_semaforo_exec, 0, 0);
	sem_init(&hilos_grado_multitarea, 0, get_grado_multiprocesamiento());

	return 0;
}

void hilos_planificador_destruir() {
	void limpiar_hilo(void * hilo) {
		pthread_cancel(*(pthread_t *) hilo);
		free(hilo);
	}

	list_destroy_and_destroy_elements(list_hilos_ejecutando, limpiar_hilo);

	sem_destroy(&hilos_semaforo_new);
	sem_destroy(&hilos_semaforo_ready);
	sem_destroy(&hilos_semaforo_exec);
}

pthread_t * hilos_crear_hilo() {
	pthread_t * hilo = malloc(sizeof(pthread_t));

	pthread_mutex_lock(&mutex_hilos);
	list_add(list_hilos_ejecutando, hilo);
	pthread_mutex_unlock(&mutex_hilos);

	return hilo;
}

void hilos_destruir_hilo_actual() {
	pthread_t hilo_actual = pthread_self();

	bool son_iguales(void * hilo_en_lista) {
		return pthread_equal(hilo_actual, *(pthread_t *) hilo_en_lista);
	}

	pthread_mutex_lock(&mutex_hilos);
	void * hilo_lista = list_remove_by_condition(list_hilos_ejecutando, son_iguales);
	pthread_mutex_unlock(&mutex_hilos);

	if (hilo_lista != NULL) {
		pthread_detach(hilo_actual);
		free(hilo_lista);
	}

	pthread_exit(NULL);
}

void hilos_post_new() { sem_post(&hilos_semaforo_new); }
void hilos_wait_new() { sem_wait(&hilos_semaforo_new); }
void hilos_post_ready() { sem_post(&hilos_semaforo_ready); }
void hilos_wait_ready() { sem_wait(&hilos_semaforo_ready); }
void hilos_post_multiprogramacion() { sem_post(&hilos_grado_multiprogramacion); }
void hilos_wait_multiprogramacion() { sem_wait(&hilos_grado_multiprogramacion); }
void hilos_post_exec() { sem_post(&hilos_semaforo_exec); }
void hilos_wait_exec() { sem_wait(&hilos_semaforo_exec); }
void hilos_post_multitarea() { sem_post(&hilos_grado_multitarea); }
void hilos_wait_multitarea() { sem_wait(&hilos_grado_multitarea); }




