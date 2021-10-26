
#include "planificadores.h"

// CERRADO, falta apagar el hilo
void planificador_largo_plazo() {
	loggear_debug("[SYSTEM] --- Se creo el planificador de largo plazo");

	// TODO: Cambiar while true, para que se detenga cuando corresponda
	while (true) {
		hilos_wait_new();
		hilos_wait_multiprogramacion();

		t_hilo * hilo = colas_mover_new_ready();
		loggear_debug("[PID: %d] --- [Largo Plazo] --- Se movió de NEW a READY", pid(hilo));

		hilos_post_ready();
	}
}

// CERRADO, falta modificar el archivo de colas para que pueda seleccionar bien que hilo mover de READY => EXEC
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

int planificadores_iniciar() {
	colas_iniciar();
	hilos_planificador_iniciar();

	int error = 0;

	error += pthread_create(hilos_crear_hilo(), NULL, (void *) planificador_largo_plazo, NULL);
	error += pthread_create(hilos_crear_hilo(), NULL, (void *) planificador_corto_plazo, NULL);

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

