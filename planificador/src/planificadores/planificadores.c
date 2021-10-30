
#include "planificadores.h"

void planificador_largo_plazo() {
	loggear_debug("[SYSTEM] --- Se creo el planificador de largo plazo");

	while (true) {
		hilos_wait_new();
		hilos_wait_multiprogramacion();

		t_hilo * hilo;
		if (hay_procesos_en_suspendido_ready()) {
			hilo = colas_mover_block_ready_ready();
			loggear_debug("[PID: %d] --- [Mediano Plazo] --- Se movió de SUSP-READY a READY", pid(hilo));
		} else {
			hilo = colas_mover_new_ready();
			loggear_debug("[PID: %d] --- [Largo Plazo] --- Se movió de NEW a READY", pid(hilo));
		}

		hilos_post_ready();
	}
}

void planificador_corto_plazo() {
	loggear_debug("[SYSTEM] --- Se creo el planificador de corto plazo");

	while (true) {
		hilos_wait_ready();
		hilos_wait_multitarea();

		t_hilo * hilo = colas_mover_ready_exec();
		loggear_debug("[PID: %d] --- [Corto Plazo] --- Se movió de READY a EXEC", pid(hilo));

		ejecutar_hilo_iniciar_ejecucion(hilo);
	}
}

void planificador_medio_plazo() {
	loggear_debug("[SYSTEM] --- Se creo el planificador de mediano plazo");

	while (true) {
		hilos_wait_nuevo_bloqueado();

		hilos_post_multitarea();

		bool multiprogramacion_copada = deberia_suspenderse_procesos();

		if (multiprogramacion_copada) {
			t_hilo * hilo = colas_mover_block_block_susp();

			loggear_debug("[PID: %d] --- [Mediano Plazo] --- Se movió de BLOCK a SUSP-BLOCK", pid(hilo));

			hilos_post_multiprogramacion();

			loggear_error("[PID: %d] --- [Mediano Plazo] --- TODO: Avisar a la RAM de la suspension", pid(hilo));
		}
	}
}

int planificadores_iniciar() {
	colas_iniciar();
	hilos_planificador_iniciar();

	int error = 0;

	error += pthread_create(hilos_crear_hilo(), NULL, (void *) planificador_largo_plazo, NULL);
	error += pthread_create(hilos_crear_hilo(), NULL, (void *) planificador_corto_plazo, NULL);
	error += pthread_create(hilos_crear_hilo(), NULL, (void *) planificador_medio_plazo, NULL);

    return error;
}

void planificadores_destruir() {
	colas_destruir();
	hilos_planificador_destruir();
}

// CERRADO
int planificadores_proceso_iniciar(uint32_t ppid) {
	loggear_debug("[SYSTEM] --- Llego un nuevo proceso al planificador");

	t_hilo * hilo = colas_insertar_new(ppid);
	hilos_post_new();

	loggear_debug("[PID: %d] --- [SYSTEM] --- Se movió a NEW", pid(hilo));

    return 0;
}

int planificadores_proceso_cerrar(void * proceso) {
    return 0;
}

int planificadores_semaforos_iniciar(void * proceso, void * semaforo) {
    return 0;
}

int planificadores_semaforos_wait(void * proceso, void * semaforo) {
    return 0;
}

int planificadores_semaforos_post(void * proceso, void * semaforo) {
    return 0;
}

int planificadores_semaforos_destruir(void * proceso, void * semaforo) {
    return 0;
}

int planificadores_io_llamar(void * proceso, void * dispositivo_io) {
    return 0;
}

void * planificadores_memoria_alloc(void * proceso, void * llamada_memoria) {
    return NULL;
}

int planificadores_memoria_free(void * proceso, void * llamada_memoria) {
    return 0;
}

void * planificadores_memoria_read(void * proceso, void * llamada_memoria) {
    return NULL;
}

int planificadores_memoria_write(void * proceso, void * llamada_memoria) {
    return 0;
}

