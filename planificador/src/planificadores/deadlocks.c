#include "deadlocks.h"

pthread_t thread_deadlock;
int ms_tiempo_deadlock;
bool detener_control_deadlock;

t_list * list_bloqueados;
int size_bloqueados;
t_dictionary * dict_visitados;

void iniciar_evaluacion_deadlock() {
	colas_bloquear_listas_bloqueados();

	t_list * list_bloqueados = colas_obtener_listas_bloqueados(SEMAFORO);

	size_bloqueados = list_size(list_bloqueados);

	dict_visitados = dictionary_create();

	for (int i = 0; i < size_bloqueados; ++i) {
		t_hilo * hilo = list_get(list_bloqueados, i);

		char * key = string_from_format("%zu", pid(hilo));

		bool * visitado = malloc(sizeof(bool));
		*visitado = false;

		dictionary_put(dict_visitados, key, visitado);
	}

}

void destruir_evaluacion_deadlock() {
	list_destroy(list_bloqueados);
	size_bloqueados = 0;
	dictionary_destroy_and_destroy_elements(dict_visitados, free);

	colas_desbloquear_listas_bloqueados();
}

bool esta_bloqueado_por_semaforo(void * hilo) {
	t_hilo * hilo_bloqueado = (t_hilo *) hilo;

	return (hilo_bloqueado->estado == ESTADO_BLOCK || hilo_bloqueado->estado == ESTADO_SUSPENDED_BLOCK) && hilo_bloqueado->bloqueante == SEMAFORO;
}

t_list * dfs(t_hilo * first_hilo, t_hilo * hilo_preguntar) {
	bool * visitado = dictionary_get(dict_visitados, string_from_format("%zu", pid(hilo_preguntar)));

	if (*visitado) { return NULL; }
	*visitado = true;

	if (first_hilo == NULL) {
		*first_hilo = hilo_preguntar;
	} else if (pid(first_hilo) == pid(hilo_preguntar)) {
		// Se cierra el ciclo. Encontro el deadlock
		t_list * lista = list_create();
		//list_add(lista, hilo_preguntar);

		return lista;
	}

	// Procesos VECINOS
	t_semaforo * semaforo_bloqueante = hilo_preguntar->bloqueante;
	t_list * procesos_ocupan_semaforo = list_filter(semaforo_bloqueante->list_procesos_retienen, esta_bloqueado_por_semaforo);

	for (int i = 0; i < list_size(procesos_ocupan_semaforo); ++i) {
		t_hilo * hilo_ocupan = list_get(procesos_ocupan_semaforo, i); // Hilo vecino [i]

		t_list * lista = dfs(first_hilo, hilo_ocupan);
		if (lista != NULL) {
			// Significa que de donde viene, encontro deadlock.
			list_add(lista, hilo_ocupan);

			return lista;
		}
	}

	return NULL;
}


void ejemplo() {
	iniciar_evaluacion_deadlock();
	t_hilo * primer_hilo_bloqueado = list_get(list_bloqueados, 0);
	t_list * hilos_deadlock = dfs(NULL, primer_hilo_bloqueado);
	if (hilos_deadlock != NULL) {
		loggear_warning("Hay deadlock");
	}
	destruir_evaluacion_deadlock();
}

int iniciar_control_deadlock();

int deadlocks_iniciar() {
	ms_tiempo_deadlock = get_tiempo_deadlock();

	int error = iniciar_control_deadlock();

	return error;
}

int deadlocks_destruir() {
	loggear_debug("[DEADLOCK] - Se va a detener el hilo de control de deadlocks");
	int err = pthread_cancel(thread_deadlock);
	if (err != 0) {
		loggear_error("[DEADLOCK] - Ocurrio un error al cancelar el thread de deadlock, %d", err);
	}

	return err;
}



int deadlocks_ejecutar() {
	while (true) {
		usleep(ms_tiempo_deadlock * 1000),
		loggear_trace("[DEADLOCK] - Pasaron %d milisegundos, controlamos deadlock", ms_tiempo_deadlock);


	}
}

int iniciar_control_deadlock() {
	int ret = pthread_create(&thread_deadlock, NULL, (void *) deadlocks_ejecutar, NULL);

	if (ret != 0) {
		loggear_error("[DEADLOCK] - Ocurrió un error al crear el hilo para controlar deadlocks, Error: %d", ret);
		if (ret == 11) {
			loggear_error("[DEADLOCK] - No se pueden crear mas threads, error %d", ret);
		}

		return ret;
	}

	loggear_debug("[DEADLOCK] - Se creo el hilo para controlar deadlocks cada %d milisegundos", ms_tiempo_deadlock);

	return 0;
}
