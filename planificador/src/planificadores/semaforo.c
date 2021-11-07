#include "semaforo.h"

#define BLOQUEAR_PROCESO 0

pthread_mutex_t mutex_semaforos;
t_dictionary * semaforos;

t_semaforo * crear_semaforo(t_matelib_semaforo * semaforo) {
	t_semaforo * sem = malloc(sizeof(t_semaforo));

	sem->nombre = semaforo->semaforo_nombre;
	sem->valor = semaforo->semaforo_valor;
	pthread_mutex_init(&(sem->mutex), NULL);
	sem->list_procesos = list_create();

	return sem;
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
		list_destroy(semaforo->list_procesos);
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
	list_add(semaforo->list_procesos, hilo_wait);
	pthread_mutex_unlock(&(semaforo->mutex));

	if (bloquear) {
		colas_mover_exec_block(SEMAFORO, semaforo->nombre, sem->pid);

		return SEM_BLOQUEAR;
	}

	colas_agregar_wait_semaforo(sem->pid, semaforo);

    return SEM_OK;
}

void desbloquear_1_semaforo(t_semaforo * semaforo) {
	for (int i = 0; i < list_size(semaforo->list_procesos); ++i) {
		t_hilo * hilo_con_semaforo = list_get(semaforo->list_procesos, i);
		if (hilo_con_semaforo->estado == ESTADO_BLOCK) {
			colas_mover_block_ready(hilo_con_semaforo);
			list_remove(semaforo->list_procesos, i);
			return;
		} else if (hilo_con_semaforo->estado == ESTADO_SUSPENDED_BLOCK) {
			colas_mover_block_susp_block_ready(hilo_con_semaforo);
			list_remove(semaforo->list_procesos, i);
			return;
		}
	}
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

	desbloquear_1_semaforo(semaforo);

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

t_estado_ejecucion semaforo_destruir(t_matelib_semaforo * sem) {
	pthread_mutex_lock(&mutex_semaforos);
	bool has_key = dictionary_has_key(semaforos, sem->semaforo_nombre);
	if (!has_key) {
		pthread_mutex_unlock(&mutex_semaforos);
		loggear_error("[PID: %zu] - WAIT - No existe el semaforo: %s", sem->pid, sem->semaforo_nombre);
		return SEM_ERROR_NO_EXISTE;
	}
	pthread_mutex_unlock(&mutex_semaforos);

	colas_desbloquear_todos_hilos(SEMAFORO, sem->semaforo_nombre);

	loggear_info("Se desbloquearon todos los hilos");

	pthread_mutex_lock(&mutex_semaforos);
	dictionary_remove_and_destroy(semaforos, sem->semaforo_nombre, free);
	pthread_mutex_unlock(&mutex_semaforos);

    return SEM_OK;
}
