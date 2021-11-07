#include "deadlocks.h"

pthread_t thread_deadlock;
int ms_tiempo_deadlock;
bool detener_control_deadlock;

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

/**
 * 1. Obtengo todos los procesos en la lista de BLOQUEADO o SUSPENDIDO-BLOQUEADO
 * 2. Filtro solo los que esten bloqueados por un semaforo
 * 3.
 */
int deadlocks_ejecutar() {
	while (true) {
		usleep(ms_tiempo_deadlock * 1000),
		loggear_trace("[DEADLOCK] - Pasaron %d milisegundos, controlamos deadlock", ms_tiempo_deadlock);

		colas_bloquear_listas_bloqueados();

		t_list * list_bloqueados = colas_obtener_listas_bloqueados(SEMAFORO);

		loggear_info("----------- DEADLOCK INICIO -----------");
		for (int i = 0; i < list_size(list_bloqueados); ++i) {
			t_hilo * hilo = list_get(list_bloqueados, i);
			loggear_info("\t [PID: %zu]", hilo->pid);

			if (hilo->bloqueante == SEMAFORO) {
				loggear_info("\t\t Bloqueado por SEMAFORO");
			} else {
				loggear_info("\t\t Bloqueado por IO");
			}

			loggear_info("\t\t Nombre bloqueante: %s", hilo->nombre_bloqueante);
			loggear_info("\t\t Semaforos retenidos: %d", list_size(hilo->semaforos_pedidos));

			if (hilo->estado == ESTADO_BLOCK) {
				loggear_info("\t\t Estado: BLOQUEADO");
			} else {
				loggear_info("\t\t Estado: SUSPENDIDO-BLOQUEADO");
			}

			for (int j = 0; j < list_size(hilo->semaforos_pedidos); ++j) {
				loggear_info("\t\t\t Nombre semaforo: %s", list_get(hilo->semaforos_pedidos, j));
			}
		}
		loggear_info("----------- DEADLOCK FIN -----------");

		colas_desbloquear_listas_bloqueados();
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
