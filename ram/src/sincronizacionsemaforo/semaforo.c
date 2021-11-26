#include "semaforo.h"

sem_t semaforo_finalizar;

void semaforo_iniciar() {
	sem_init(&semaforo_finalizar, 0, 0);
}

void semaforo_destruir() {
	sem_destroy(&semaforo_finalizar);
}

void semaforo_post_fin() {
	loggear_debug("SE CIERRA TODO!!!!");
	sem_post(&semaforo_finalizar);
	//return NULL;
}

void semaforo_wait_fin() {
	loggear_debug("ESPERAMOS A QUE TERMINE TODO!!!");
	sem_wait(&semaforo_finalizar);
	loggear_debug("TERMINO LA ESPERA PARA CERRAR TODO");
}

