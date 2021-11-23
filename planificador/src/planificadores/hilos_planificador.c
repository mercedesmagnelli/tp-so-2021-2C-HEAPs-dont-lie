#include "hilos_planificador.h"

sem_t hilos_semaforo_new;

sem_t hilos_semaforo_ready;
sem_t hilos_grado_multiprogramacion;

sem_t hilos_semaforo_exec;
sem_t hilos_grado_multitarea;

sem_t hilos_nuevo_bloqueado;

sem_t hilos_finalizado;

t_list * list_hilos_ejecutando;
pthread_mutex_t mutex_hilos;

t_dictionary * dict_hilos;

int hilos_planificador_iniciar() {
	list_hilos_ejecutando = list_create();
	dict_hilos = dictionary_create();

	sem_init(&hilos_semaforo_new, 0, 0);

	sem_init(&hilos_semaforo_ready, 0, 0);
	sem_init(&hilos_grado_multiprogramacion, 0, get_grado_multiprogramacion());

	sem_init(&hilos_semaforo_exec, 0, 0);
	sem_init(&hilos_grado_multitarea, 0, get_grado_multiprocesamiento());

	sem_init(&hilos_nuevo_bloqueado, 0, 0);

	sem_init(&hilos_finalizado, 0, 0);

	return 0;
}

void limpiar_semaforo(void * sem) {
	t_hilos_semaforo * hilo = (t_hilos_semaforo *) sem;

	sem_destroy(&hilo->sem_ejecutar);
	free(hilo);
}

void hilos_planificador_destruir() {
	void limpiar_hilo(void * hilo) {
		pthread_cancel(*(pthread_t *) hilo);
		free(hilo);
	}

	list_destroy_and_destroy_elements(list_hilos_ejecutando, limpiar_hilo);

	dictionary_destroy_and_destroy_elements(dict_hilos, limpiar_semaforo);

	sem_destroy(&hilos_semaforo_new);

	sem_destroy(&hilos_semaforo_ready);
	sem_destroy(&hilos_grado_multiprogramacion);

	sem_destroy(&hilos_semaforo_exec);
	sem_destroy(&hilos_grado_multitarea);

	sem_destroy(&hilos_nuevo_bloqueado);

	sem_destroy(&hilos_finalizado);
}

pthread_t * hilos_crear_hilo() {
	pthread_t * hilo = malloc(sizeof(pthread_t));

	pthread_mutex_lock(&mutex_hilos);
	list_add(list_hilos_ejecutando, hilo);
	pthread_mutex_unlock(&mutex_hilos);

	return hilo;
}

char * hilos_get_key(uint32_t pid) {
	return string_from_format("%zu", pid);
}

void hilos_agregar_nuevo_hilo(uint32_t pid) {
	t_hilos_semaforo * hilo_semaforo = malloc(sizeof(t_hilos_semaforo));

	sem_init(&hilo_semaforo->sem_ejecutar, 0, 0);

	hilo_semaforo->finalizo = false;

	dictionary_put(dict_hilos, hilos_get_key(pid), hilo_semaforo);
}

void hilos_destruir_hilo_finish(uint32_t pid) {
	t_hilos_semaforo * hilo_semaforo = dictionary_remove(dict_hilos, hilos_get_key(pid));

	limpiar_semaforo(hilo_semaforo);
}

void hilos_post_ejecucion(uint32_t pid) {
	t_hilos_semaforo * hilo_semaforo = dictionary_get(dict_hilos, hilos_get_key(pid));

	sem_post(&hilo_semaforo->sem_ejecutar);
}

void hilos_se_movio_finalizado(uint32_t pid) {
	t_hilos_semaforo * hilo_semaforo = dictionary_get(dict_hilos, hilos_get_key(pid));

	hilo_semaforo->finalizo = true;

	sem_post(&hilo_semaforo->sem_ejecutar);
}

void hilos_wait_ejecucion(uint32_t pid) {
	t_hilos_semaforo * hilo_semaforo = dictionary_get(dict_hilos, hilos_get_key(pid));

	sem_wait(&hilo_semaforo->sem_ejecutar);
}

bool hilos_check_finalizo_proceso(uint32_t pid) {
	t_hilos_semaforo * hilo_semaforo = dictionary_get(dict_hilos, hilos_get_key(pid));

	sem_wait(&hilo_semaforo->sem_ejecutar);

	return hilo_semaforo->finalizo;
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
void hilos_post_nuevo_bloqueado() { sem_post(&hilos_nuevo_bloqueado); }
void hilos_wait_nuevo_bloqueado() { sem_wait(&hilos_nuevo_bloqueado); }
void hilos_post_finalizado() { sem_post(&hilos_finalizado); }
void hilos_wait_finalizado() { sem_wait(&hilos_finalizado); }


