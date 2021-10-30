#include "ejecutar_hilo.h"

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
