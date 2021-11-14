#include "semaforo.h"

#define BLOQUEAR_PROCESO 0

pthread_mutex_t mutex_semaforos;
t_dictionary * semaforos;

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
		pthread_mutex_unlock(&(semaforo->mutex));
		colas_mover_exec_block(SEMAFORO, semaforo->nombre, sem->pid);
		list_add(semaforo->list_procesos_bloqueados, hilo_wait);

		return SEM_BLOQUEAR;
	} else {
		list_add(semaforo->list_procesos_retienen, hilo_wait);
		pthread_mutex_unlock(&(semaforo->mutex));
	}

	colas_agregar_wait_semaforo(sem->pid, semaforo);

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
			if (!desbloquear_todos) {
				return hilo;
			}
		} else if (hilo_con_semaforo->estado == ESTADO_SUSPENDED_BLOCK) {
			t_hilo * hilo = colas_mover_block_susp_block_ready(hilo_con_semaforo);

			loggear_trace("[SEMAFORO] - PID: %zu - Se movio de SUSPENDED-BLOCK a READY", pid(hilo));

			list_remove(semaforo->list_procesos_bloqueados, i);
			if (!desbloquear_todos) {
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

	loggear_trace("[SEMAFORO] - PID: %zu - Luego del SEM_POST, se desbloqueo este hilo", pid(hilo_desbloqueado));

	t_hilo * hilo_wait = colas_obtener_hilo_en_exec(sem->pid);
	for (int i = 0; i < list_size(hilo_wait->semaforos_pedidos); ++i) {
		t_semaforo * semaforo_hilo = list_get(hilo_wait->semaforos_pedidos, i);
		if (strcmp(semaforo_hilo->nombre, semaforo->nombre)) {
			list_remove(hilo_wait->semaforos_pedidos, i);
			break;
		}
	}

	pthread_mutex_unlock(&(semaforo->mutex));

	return SEM_OK;
}

void semaforo_deadlock_post(t_semaforo * semaforo) {
	pthread_mutex_lock(&(semaforo->mutex));
	semaforo->valor++;

	loggear_trace("[SEMAFORO] - SEM_POST: %s - Se va a desbloquear el primer proceso en la lista de retenidos", semaforo->nombre);

	t_hilo * hilo_desbloqueado = desbloquear_semaforo(semaforo, false);

	loggear_trace("[SEMAFORO] - PID: %zu - Luego del SEM_POST, se desbloqueo este hilo", pid(hilo_desbloqueado));

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

    return SEM_OK;
}
