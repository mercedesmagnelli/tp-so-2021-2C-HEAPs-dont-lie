#include "ejecutar_hilo.h"

void ejecutar_hilo(t_hilo * hilo) {
	loggear_debug("[PID: %d] --- [EXEC] --- Se esta ejecuando el hilo", pid(hilo));
	while (1) {
		sleep(1);
		loggear_debug("[PID: %d] --- [EXEC] --- Ejecuto 1 ciclo", pid(hilo));
	}
}

void ejecutar_hilo_iniciar_ejecucion(t_hilo * hilo) {
	pthread_create(hilos_crear_hilo(), NULL, (void *) ejecutar_hilo, hilo);
}
