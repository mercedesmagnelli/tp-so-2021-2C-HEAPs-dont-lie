#include "semaforo.h"

#define BLOQUEAR_PROCESO 0

t_dictionary * semaforos;

t_semaforo * crear_semaforo(t_matelib_semaforo * semaforo) {
	t_semaforo * sem = malloc(sizeof(t_semaforo));

	sem->nombre = semaforo->semaforo_nombre;
	sem->valor = semaforo->semaforo_valor;

	return sem;
}


int semaforo_estructuras_crear() {
	loggear_debug("Se creo las estructuras para manejo de semaforos");

	semaforos = dictionary_create();
    return 0;
}

int semaforo_estructuras_destruir() {
	loggear_debug("Se destruyen las estructuras para manejo de semaforos");

	dictionary_clean_and_destroy_elements(semaforos, free);
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

	dictionary_put(semaforos, semaforo->semaforo_nombre, sem);

	loggear_info("[PID: %zu] - Se crea semaforo: '%s' - Valor: %d", semaforo->pid, semaforo->semaforo_nombre, semaforo->semaforo_valor);

    return SEM_OK;
}

t_estado_ejecucion semaforo_wait(t_matelib_semaforo * sem) {
	if (!dictionary_has_key(semaforos, sem->semaforo_nombre)) {
		loggear_error("[PID: %zu] - WAIT - No existe el semaforo: %s", sem->pid, sem->semaforo_nombre);
		return SEM_ERROR_NO_EXISTE;
	}

	t_semaforo * semaforo = dictionary_get(semaforos, sem->semaforo_nombre);

	semaforo->valor--;

	if (semaforo->valor < BLOQUEAR_PROCESO) {
		return SEM_BLOQUEAR;
	}

    return SEM_OK;
}

t_estado_ejecucion semaforo_post(t_matelib_semaforo * sem) {
	if (!dictionary_has_key(semaforos, sem->semaforo_nombre)) {
		loggear_error("[PID: %zu] - WAIT - No existe el semaforo: %s", sem->pid, sem->semaforo_nombre);
		return SEM_ERROR_NO_EXISTE;
	}

	t_semaforo * semaforo = dictionary_get(semaforos, sem->semaforo_nombre);

	semaforo->valor++;

	colas_desbloquear_1_hilo(SEMAFORO, semaforo->nombre);

	return SEM_OK;
}

t_estado_ejecucion semaforo_destruir(t_matelib_semaforo * sem) {
	if (!dictionary_has_key(semaforos, sem->semaforo_nombre)) {
		loggear_error("[PID: %zu] - WAIT - No existe el semaforo: %s", sem->pid, sem->semaforo_nombre);
		return SEM_ERROR_NO_EXISTE;
	}

	colas_desbloquear_todos_hilos(SEMAFORO, sem->semaforo_nombre);

	dictionary_remove_and_destroy(semaforos, sem->semaforo_nombre, free);

    return SEM_OK;
}
