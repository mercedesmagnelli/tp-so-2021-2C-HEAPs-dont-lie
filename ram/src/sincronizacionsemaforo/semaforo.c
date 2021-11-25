#include "semaforo.h"

sem_t semaforo_finalizar;

void semaforo_iniciar() {
	sem_init(&semaforo_finalizar, 0, 0);
}

void semaforo_destruir() {
	sem_destroy(&semaforo_finalizar);
}

void semaforo_post_fin() { sem_post(&semaforo_finalizar); }

void semaforo_wait_fin() { sem_wait(&semaforo_finalizar); }

