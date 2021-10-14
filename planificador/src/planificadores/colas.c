#include "colas.h"

/**
 * TODOs
 * 1. [Listo] Crear todas las colas y listas
 * 2. [Listo] Crear la destruccion de las colas y listas
 * 3. Pensar en que estructuras se van a pasar entre estados
 * 4. Modificar los void * y los return donde corresponda para pasarlo por parametro o retornarlo
 * 5. Programar la forma de comparar hilos (supongo que comparacion por .pid)
 * 6. Recibir algoritmo de corto plazo para mover entre READY->EXEC
 * 7. Recibir algoritmo de mediano plazo para mover de BLOCK->SUSP/BLOCK
 * */

t_queue * new_queue;
pthread_mutex_t mutex_new_queue;

t_list * ready_list;
pthread_mutex_t mutex_ready_list;

t_list * exec_list;
pthread_mutex_t mutex_exec_list;

t_queue * finish_queue;
pthread_mutex_t mutex_finish_queue;

t_list * blocked_list;
pthread_mutex_t mutex_blocked_list;

t_list * suspended_blocked_list;
pthread_mutex_t mutex_suspended_blocked_list;

t_queue * suspended_ready_queue;
pthread_mutex_t mutex_suspended_ready_queue;

int colas_iniciar() {
	new_queue = queue_create();
	ready_list = list_create();
	exec_list = list_create();
	finish_queue = queue_create();
	blocked_list = list_create();
	suspended_blocked_list = list_create();
	suspended_ready_queue = queue_create();

	pthread_mutex_init(&mutex_new_queue, NULL);
	pthread_mutex_init(&mutex_ready_list, NULL);
	pthread_mutex_init(&mutex_exec_list, NULL);
	pthread_mutex_init(&mutex_finish_queue, NULL);
	pthread_mutex_init(&mutex_blocked_list, NULL);
	pthread_mutex_init(&mutex_suspended_blocked_list, NULL);
	pthread_mutex_init(&mutex_suspended_ready_queue, NULL);

    return 0;
}

int colas_destruir() {
	queue_destroy(new_queue);
	list_destroy(ready_list);
	list_destroy(exec_list);
	queue_destroy(finish_queue);
	list_destroy(blocked_list);
	list_destroy(suspended_blocked_list);
	queue_destroy(suspended_ready_queue);

	pthread_mutex_destroy(&mutex_new_queue);
	pthread_mutex_destroy(&mutex_ready_list);
	pthread_mutex_destroy(&mutex_exec_list);
	pthread_mutex_destroy(&mutex_finish_queue);
	pthread_mutex_destroy(&mutex_blocked_list);
	pthread_mutex_destroy(&mutex_suspended_blocked_list);
	pthread_mutex_destroy(&mutex_suspended_ready_queue);

    return 0;
}

t_hilo * colas_insertar_new(void * proceso) {
	t_hilo * hilo = malloc(sizeof(t_hilo));

	hilo->proceso = proceso;
	hilo->estado = NEW;

	pthread_mutex_lock(&mutex_new_queue);
	queue_push(new_queue, hilo);
	pthread_mutex_unlock(&mutex_new_queue);

    return hilo;
}

t_hilo * colas_mover_new_ready() {

	pthread_mutex_lock(&mutex_new_queue);
	t_hilo * hilo = queue_pop(new_queue);
	pthread_mutex_unlock(&mutex_new_queue);

	pthread_mutex_lock(&mutex_ready_list);
	list_add(ready_list, hilo);
	pthread_mutex_lock(&mutex_ready_list);

    return hilo;
}

t_hilo * colas_mover_ready_exec() {
	// TODO: Pasar un criterio para seleccionar el que se toma de ready
	// TODO: DE momento se toma el primero

	pthread_mutex_lock(&mutex_ready_list);
	t_hilo * hilo = list_get(ready_list, 0);
	pthread_mutex_lock(&mutex_ready_list);

	pthread_mutex_lock(&mutex_exec_list);
	list_add(exec_list, hilo);
	pthread_mutex_unlock(&mutex_exec_list);

    return hilo;
}

t_hilo * colas_mover_exec_finish() {
	// TODO: Pasar criterio para seleccionar el EXEC
	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_get(exec_list, 0);
	pthread_mutex_unlock(&mutex_exec_list);

	pthread_mutex_lock(&mutex_finish_queue);
	queue_push(finish_queue, hilo);
	pthread_mutex_unlock(&mutex_finish_queue);

    return hilo;
}

t_hilo * colas_mover_exec_block() {
	// TODO: Pasar criterio para seleccionar de EXEC
	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_get(exec_list, 0);
	pthread_mutex_unlock(&mutex_exec_list);

	pthread_mutex_lock(&mutex_blocked_list);
	list_add(blocked_list, 0);
	pthread_mutex_unlock(&mutex_blocked_list);

	// TODO: Llamar funcion para consultar si hay que moverlo a bloqueado-suspendido

    return hilo;
}

t_hilo * colas_mover_block_ready() {
	// TODO: PAsar criterio para obtener de bloqueado
	pthread_mutex_lock(&mutex_blocked_list);
	t_hilo * hilo = list_get(blocked_list, 0);
	pthread_mutex_unlock(&mutex_blocked_list);

	// TODO: Asegurarse que inserte en 0 o al final
	pthread_mutex_lock(&mutex_ready_list);
	//list_add_in_index(ready_list, hilo, 0);
	list_add(ready_list, hilo);
	pthread_mutex_unlock(&mutex_ready_list);

    return hilo;
}

t_hilo * colas_mover_block_block_susp() {
	// TODO: Armar criterio de busqueda
	pthread_mutex_lock(&mutex_blocked_list);
	int ultimo_bloqueado_pos = list_size(blocked_list);
	t_hilo * hilo = list_get(blocked_list, ultimo_bloqueado_pos - 1);
	pthread_mutex_unlock(&mutex_blocked_list);

	pthread_mutex_lock(&mutex_suspended_blocked_list);
	list_add(suspended_blocked_list, hilo);
	pthread_mutex_unlock(&mutex_suspended_blocked_list);

    return hilo;
}

t_hilo * colas_mover_block_susp_block_ready() {
	// TODO: Criterio para obtener de suspendido bloqueado

	pthread_mutex_lock(&mutex_suspended_blocked_list);
	t_hilo * hilo = list_get(suspended_blocked_list, 0);
	pthread_mutex_unlock(&mutex_suspended_blocked_list);

	pthread_mutex_lock(&mutex_suspended_ready_queue);
	queue_push(suspended_ready_queue, hilo);
	pthread_mutex_unlock(&mutex_suspended_ready_queue);

    return hilo;
}

t_hilo * colas_mover_block_ready_ready() {
	pthread_mutex_lock(&mutex_suspended_ready_queue);
	t_hilo * hilo = queue_pop(suspended_ready_queue);
	pthread_mutex_unlock(&mutex_suspended_ready_queue);

	// TODO: Asegurarse que inserte en 0 o al final
	pthread_mutex_lock(&mutex_ready_list);
	list_add_in_index(ready_list, hilo, 0);
	pthread_mutex_unlock(&mutex_ready_list);

	return hilo;
}
