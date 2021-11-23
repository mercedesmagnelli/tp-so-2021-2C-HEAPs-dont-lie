#include "semaforo.h"

#define BLOQUEAR_PROCESO 0

pthread_mutex_t mutex_semaforos;
t_dictionary * semaforos;

char * lista_hilos(t_list * lista) {
	char * hilos = string_new();
	string_append(&hilos, "[ ");

	for (int i = 0; i < list_size(lista); ++i) {
		t_hilo * hilo = list_get(lista, i);
		string_append(&hilos, string_from_format("%zu ", pid(hilo)));
	}
	string_append(&hilos, "]");
	return hilos;
}

void semaforo_imprimir_status() {
	void imprimir(char * key, void * sem) {
		t_semaforo * semaforo = (t_semaforo *) sem;

		loggear_warning("--- Nombre: %s --- Valor: %d --- Bloqueados: %s --- Retenidos: %s ---",
		semaforo->nombre,
		semaforo->valor,
		lista_hilos(semaforo->list_procesos_bloqueados),
		lista_hilos(semaforo->list_procesos_retienen));
	}
	pthread_mutex_lock(&mutex_semaforos);
	loggear_warning("--------------INICIO SEMAFOROS--------------");
	dictionary_iterator(semaforos, imprimir);
	loggear_warning("--------------FIN SEMAFOROS--------------");
	pthread_mutex_unlock(&mutex_semaforos);
}

t_semaforo * crear_semaforo(t_matelib_semaforo * semaforo) {
	t_semaforo * sem = malloc(sizeof(t_semaforo));

	sem->nombre = semaforo->semaforo_nombre;
	sem->valor = semaforo->semaforo_valor;
	pthread_mutex_init(&(sem->mutex), NULL);
	sem->list_procesos_retienen = list_create();
	sem->list_procesos_bloqueados = list_create();

	return sem;
}

t_semaforo * semaforo_get(char * nombre_semaforo) {
	t_semaforo * semaforo;

	pthread_mutex_lock(&mutex_semaforos);
	semaforo = dictionary_get(semaforos, nombre_semaforo);
	pthread_mutex_unlock(&mutex_semaforos);

	return semaforo;
}

int semaforo_estructuras_crear() {
	loggear_debug("Se creo las estructuras para manejo de semaforos");

	semaforos = dictionary_create();
	pthread_mutex_init(&mutex_semaforos, NULL);

	semaforo_imprimir_status();

    return 0;
}

int semaforo_estructuras_destruir() {
	loggear_debug("Se destruyen las estructuras para manejo de semaforos");
	void destruir(void * sem) {
		t_semaforo * semaforo = (t_semaforo *) sem;
		pthread_mutex_destroy(&(semaforo->mutex));
		list_destroy(semaforo->list_procesos_retienen);
		list_destroy(semaforo->list_procesos_bloqueados);
		free(sem);
	}

	dictionary_clean_and_destroy_elements(semaforos, destruir);
    return 0;
}

t_estado_ejecucion semaforo_iniciar(t_matelib_semaforo * semaforo) {
	if (semaforo->semaforo_valor < 0) {
		loggear_error("[PID: %zu] - Semaforo: '%s' - Valor: %d - No se puede crear semaforo < 0", semaforo->pid, semaforo->semaforo_nombre, semaforo->semaforo_valor);

		return SEM_ERROR_MENOR_CERO;
	} else if (dictionary_has_key(semaforos, semaforo->semaforo_nombre)) {
		loggear_warning("[PID: %zu] - Semaforo: '%s' - Valor: %d - Ya existia no se crea", semaforo->pid, semaforo->semaforo_nombre, semaforo->semaforo_valor);
		return SEM_ERROR_YA_EXISTIA;
	}

	t_semaforo * sem = crear_semaforo(semaforo);

	pthread_mutex_lock(&mutex_semaforos);
	dictionary_put(semaforos, semaforo->semaforo_nombre, sem);
	pthread_mutex_unlock(&mutex_semaforos);

	loggear_info("[PID: %zu] - Se crea semaforo: '%s' - Valor: %d", semaforo->pid, semaforo->semaforo_nombre, semaforo->semaforo_valor);

	semaforo_imprimir_status();

    return SEM_OK;
}

t_estado_ejecucion semaforo_wait(t_matelib_semaforo * sem) {
	pthread_mutex_lock(&mutex_semaforos);
	bool has_key = dictionary_has_key(semaforos, sem->semaforo_nombre);
	if (!has_key) {
		pthread_mutex_unlock(&mutex_semaforos);
		loggear_error("[PID: %zu] - WAIT - No existe el semaforo: %s", sem->pid, sem->semaforo_nombre);
		return SEM_ERROR_NO_EXISTE;
	}

	t_semaforo * semaforo = dictionary_get(semaforos, sem->semaforo_nombre);
	pthread_mutex_unlock(&mutex_semaforos);

	t_hilo * hilo_wait = colas_obtener_hilo_en_exec(sem->pid);

	pthread_mutex_lock(&(semaforo->mutex));
	semaforo->valor--;
	bool bloquear = semaforo->valor < BLOQUEAR_PROCESO;

	if (bloquear) {
		colas_mover_exec_block(SEMAFORO, semaforo->nombre, sem->pid);
		list_add(semaforo->list_procesos_bloqueados, hilo_wait);
		pthread_mutex_unlock(&(semaforo->mutex));

		semaforo_imprimir_status();

		return SEM_BLOQUEAR;
	} else {
		list_add(semaforo->list_procesos_retienen, hilo_wait);
		pthread_mutex_unlock(&(semaforo->mutex));
	}

	colas_agregar_wait_semaforo(sem->pid, semaforo);

	semaforo_imprimir_status();

    return SEM_OK;
}

t_hilo * desbloquear_semaforo(t_semaforo * semaforo, bool desbloquear_todos) {
	for (int i = 0; i < list_size(semaforo->list_procesos_bloqueados); ++i) {
		t_hilo * hilo_con_semaforo = list_get(semaforo->list_procesos_bloqueados, i);
		if (hilo_con_semaforo->sera_eliminado_por_deadlock) {
			continue;
		}

		loggear_trace("[SEMAFORO] - PID: %zu va ser desbloqueado por SEM_POST en el semaforo: %s", hilo_con_semaforo->pid, semaforo->nombre);

		if (hilo_con_semaforo->estado != ESTADO_BLOCK && hilo_con_semaforo->estado != ESTADO_SUSPENDED_BLOCK) {
			loggear_error("[SEMAFORO] - El PID: %zu que se quiere desbloquear por SEM_POST no esta bloqueado, esta en: %d", pid(hilo_con_semaforo), hilo_con_semaforo->estado);
		}

		if (hilo_con_semaforo->estado == ESTADO_BLOCK) {
			t_hilo * hilo = colas_mover_block_ready(hilo_con_semaforo);

			loggear_trace("[SEMAFORO] - PID: %zu - Se movio de BLOCK a READY", pid(hilo));

			list_remove(semaforo->list_procesos_bloqueados, i);
			list_add(semaforo->list_procesos_retienen, hilo);
			list_add(hilo->semaforos_pedidos, semaforo);
			if (!desbloquear_todos) {
				semaforo_imprimir_status();

				return hilo;
			}
		} else if (hilo_con_semaforo->estado == ESTADO_SUSPENDED_BLOCK) {
			t_hilo * hilo = colas_mover_block_susp_block_ready(hilo_con_semaforo);

			loggear_trace("[SEMAFORO] - PID: %zu - Se movio de SUSPENDED-BLOCK a READY", pid(hilo));

			list_remove(semaforo->list_procesos_bloqueados, i);
			list_add(semaforo->list_procesos_retienen, hilo);
			list_add(hilo->semaforos_pedidos, semaforo);
			if (!desbloquear_todos) {
				semaforo_imprimir_status();

				return hilo;
			}
		}
	}

	return NULL;
}

t_estado_ejecucion semaforo_post(t_matelib_semaforo * sem) {
	pthread_mutex_lock(&mutex_semaforos);
	bool has_key = dictionary_has_key(semaforos, sem->semaforo_nombre);
	if (!has_key) {
		pthread_mutex_unlock(&mutex_semaforos);
		loggear_error("[PID: %zu] - WAIT - No existe el semaforo: %s", sem->pid, sem->semaforo_nombre);
		return SEM_ERROR_NO_EXISTE;
	}

	t_semaforo * semaforo = dictionary_get(semaforos, sem->semaforo_nombre);
	pthread_mutex_unlock(&mutex_semaforos);

	pthread_mutex_lock(&(semaforo->mutex));
	semaforo->valor++;

	loggear_trace("[SEMAFORO] - PID: %zu - SEM_POST: %s", sem->pid, sem->semaforo_nombre);

	t_hilo * hilo_desbloqueado = desbloquear_semaforo(semaforo, false);
	if (hilo_desbloqueado != NULL) {
		loggear_trace("[SEMAFORO] - PID: %zu - Luego del SEM_POST, se desbloqueo este hilo", pid(hilo_desbloqueado));
	} else {
		loggear_trace("[SEMAFORO] - EL SEM_POST a %s, no desbloqueo ningun hilo", semaforo->nombre);
	}

	t_hilo * hilo_wait = colas_obtener_hilo_en_exec(sem->pid);
	if (hilo_wait == NULL) {
		loggear_error("TODO: SEMAFORO, NO DEBERIA ENTRAR ACA SE ELIMINO EL HILO");
		pthread_mutex_unlock(&(semaforo->mutex));
		semaforo_imprimir_status();
		return SEM_ERROR;
	}
	for (int i = 0; i < list_size(hilo_wait->semaforos_pedidos); ++i) {
		t_semaforo * semaforo_hilo = list_get(hilo_wait->semaforos_pedidos, i);
		if (strcmp(semaforo_hilo->nombre, semaforo->nombre) == 0) {
			list_remove(hilo_wait->semaforos_pedidos, i);
			break;
		}
	}

	semaforo_imprimir_status();

	pthread_mutex_unlock(&(semaforo->mutex));

	return SEM_OK;
}

void semaforo_deadlock_post(t_semaforo * semaforo) {
	pthread_mutex_lock(&(semaforo->mutex));
	semaforo->valor++;

	loggear_trace("[SEMAFORO] - SEM_POST: %s - Se va a desbloquear el primer proceso en la lista de retenidos", semaforo->nombre);

	t_hilo * hilo_desbloqueado = desbloquear_semaforo(semaforo, false);

	if (hilo_desbloqueado != NULL) {
		loggear_trace("[SEMAFORO] - PID: %zu - Luego del SEM_POST, se desbloqueo este hilo", pid(hilo_desbloqueado));
	} else {
		loggear_trace("[SEMAFORO] - Luego del SEM_POST, no se desbloqueo ningun hilo");
	}

	semaforo_imprimir_status();

	pthread_mutex_unlock(&(semaforo->mutex));
}

t_estado_ejecucion semaforo_destruir(t_matelib_semaforo * sem) {
	pthread_mutex_lock(&mutex_semaforos);
	bool has_key = dictionary_has_key(semaforos, sem->semaforo_nombre);
	if (!has_key) {
		pthread_mutex_unlock(&mutex_semaforos);
		loggear_error("[PID: %zu] - WAIT - No existe el semaforo: %s", sem->pid, sem->semaforo_nombre);
		return SEM_ERROR_NO_EXISTE;
	}
	t_semaforo * semaforo = dictionary_get(semaforos, sem->semaforo_nombre);
	pthread_mutex_unlock(&mutex_semaforos);

	desbloquear_semaforo(semaforo, true);

	loggear_info("Se desbloquearon todos los hilos");

	pthread_mutex_lock(&mutex_semaforos);
	dictionary_remove_and_destroy(semaforos, sem->semaforo_nombre, free);
	pthread_mutex_unlock(&mutex_semaforos);

	semaforo_imprimir_status();

    return SEM_OK;
}

void semaforo_eliminar_proceso(t_hilo * hilo) {
	bool es_hilo(void * hilov) { return pid(hilov) == pid(hilo); }
	void identity(void * element) { }

	if (hilo->bloqueante == SEMAFORO) {
		t_semaforo * semaforo_bloqueante = semaforo_get(hilo->nombre_bloqueante);

		loggear_info("[SEMAFORO] -- El PID: %zu, esta bloqueado por el semaforo: %s", pid(hilo), semaforo_bloqueante->nombre);

		pthread_mutex_lock(&(semaforo_bloqueante->mutex));
		semaforo_bloqueante->valor++;
		pthread_mutex_unlock(&(semaforo_bloqueante->mutex));

		list_remove_and_destroy_by_condition(semaforo_bloqueante->list_procesos_bloqueados, es_hilo, identity);

		loggear_info("[SEMAFORO] -- Se le incremeto un contador al semaforo: %s", semaforo_bloqueante->nombre);
	}

	if (hilo->semaforos_pedidos != NULL) {
		void avisar_semaforo_post(void * sem) {
			t_semaforo * semaforo = (t_semaforo *) sem;
			list_remove_and_destroy_by_condition(semaforo->list_procesos_retienen, es_hilo, identity);
			semaforo_deadlock_post(semaforo);
		}
		list_destroy_and_destroy_elements(hilo->semaforos_pedidos, avisar_semaforo_post);
		hilo->semaforos_pedidos = NULL;
	}

	loggear_info("[SEMAFORO] -- Se hizo POST de todos los semaforos ocupados por el PID: %zu", pid(hilo));

	semaforo_imprimir_status();
}
