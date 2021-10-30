#include "ejecutar_hilo.h"

void procesar_ejecucion_semaforo(t_hilo * hilo, t_matelib_semaforo * semaforo, t_header header) {
	t_estado_ejecucion ejecucion;

	switch (header) {
		case MATELIB_SEM_INIT:
			ejecucion = semaforo_iniciar(semaforo);

			if (ejecucion == SEM_OK) {
				loggear_warning("[PID: %d] --- [EXEC Sem] --- Se creo el semaforo", semaforo->pid);
			} else {
				loggear_error("[PID: %d] --- [EXEC Sem] --- No se pudo crear el semaforo", semaforo->pid);
			}

			break;
		case MATELIB_SEM_WAIT:
			ejecucion = semaforo_wait(semaforo);

			if (ejecucion == SEM_OK) {
				loggear_warning("[PID: %d] --- [EXEC Sem] --- WAIT, no hacer nada", semaforo->pid);
			} else if (ejecucion == SEM_BLOQUEAR) {
				loggear_warning("[PID: %d] --- [EXEC Sem] --- WAIT, bloquear hilo", semaforo->pid);

				colas_mover_exec_block(hilo);
			} else {
				loggear_error("[PID: %d] --- [EXEC Sem] --- WAIT, error al ejecutar", semaforo->pid);
			}

			break;
		case MATELIB_SEM_POST:
			ejecucion = semaforo_post(semaforo);

			if (ejecucion == SEM_OK) {
				loggear_warning("[PID: %d] --- [EXEC Sem] --- POST, enviar 1 desbloqueo", semaforo->pid);

				colas_desbloquear_1_hilo(semaforo->semaforo_nombre);
			} else {
				loggear_error("[PID: %d] --- [EXEC Sem] --- POST, error al ejecutar", semaforo->pid);
			}

			break;
		case MATELIB_SEM_DESTROY:
			ejecucion = semaforo_destruir(semaforo);

			if (ejecucion == SEM_OK) {
				loggear_warning("[PID: %d] --- [EXEC Sem] --- DESTROY, enviar desbloquear todos", semaforo->pid);

				colas_desbloquear_todos_hilos(semaforo->semaforo_nombre);
			} else {
				loggear_error("[PID: %d] --- [EXEC Sem] --- DESTROY, error al ejecutar", semaforo->pid);
			}

			break;
		default:
			loggear_error("Ocurrio un error al procesar a la llamada a semaforos");
			break;
	}

}

void ejecutar_hilo(t_hilo * hilo) {
	int contador = 0;
	loggear_debug("[PID: %d] --- [EXEC] --- Se esta ejecuando el hilo", pid(hilo));
	while (1) {
		sleep(1);
		loggear_debug("[PID: %d] --- [EXEC] --- Ejecuto 1 ciclo", pid(hilo));

		// TODO: DELETE THIS
		if (contador++ >= 2) {
			// DORMIR HILO
			loggear_warning("[PID: %d] --- [EXEC] --- Pasaron %d instantes, me bloqueo", pid(hilo), contador);

			colas_mover_exec_block(hilo);



			// TODO: DEstruir el hilo de la esturctura
			pthread_exit(NULL);
		}
	}
}

void ejecutar_hilo_iniciar_ejecucion(t_hilo * hilo) {
	pthread_create(hilos_crear_hilo(), NULL, (void *) ejecutar_hilo, hilo);
}
