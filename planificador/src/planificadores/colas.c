#include "colas.h"

/**
 * TODOs
 * 1. [Listo] Crear todas las colas y listas
 * 2. [Listo] Crear la destruccion de las colas y listas
 * 3. [Listo] Pensar en que estructuras se van a pasar entre estados, todos t_hilo excepto en NEW
 * 4. [Listo] Modificar los void * y los return donde corresponda para pasarlo por parametro o retornarlo
 * 5. [Listo] Programar la forma de comparar hilos (supongo que comparacion por .pid)
 * 6. [Listo] Recibir algoritmo de corto plazo para mover entre READY->EXEC
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

typedef bool (* son_iguales) (void *);
son_iguales son_mismo_hilo(t_hilo * hilo1) {
	bool son_iguales(void * hilo2) {
		t_hilo * hilo_comparar = (t_hilo *) hilo2;
		return hilo1->pid == hilo_comparar->pid;
	}

	return son_iguales;
}

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

	algoritmos_iniciar();

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

	algoritmos_destruir();

    return 0;
}

t_hilo * colas_insertar_new(uint32_t pid) {
	t_hilo * hilo = malloc(sizeof(t_hilo));

	hilo->pid = pid;
	hilo->estado = NEW;
	hilo->estimacion_anterior = get_estimacion_inicial();
	hilo->timestamp_tiempo_exec = 0.0;

	pthread_mutex_lock(&mutex_new_queue);
	queue_push(new_queue, hilo);
	pthread_mutex_unlock(&mutex_new_queue);

    return hilo;
}

t_hilo * colas_mover_new_ready() {
	pthread_mutex_lock(&mutex_new_queue);
	t_hilo * hilo = queue_pop(new_queue);
	pthread_mutex_unlock(&mutex_new_queue);

	hilo->timestamp_entrar_ready = estructuras_current_timestamp();
	hilo->estado = READY;

	pthread_mutex_lock(&mutex_ready_list);
	list_add(ready_list, hilo);
	pthread_mutex_unlock(&mutex_ready_list);

    return hilo;
}

t_hilo * colas_mover_ready_exec() {
	t_hilo * hilo_mover;
	bool son_iguales(void * hilo2) { return hilo_mover->pid == ((t_hilo *) hilo2)->pid; }

	pthread_mutex_lock(&mutex_ready_list);
	hilo_mover = hilo_obtener_siguiente_hilo_a_mover_exec(ready_list);
	t_hilo * hilo = list_remove_by_condition(ready_list, son_iguales);
	pthread_mutex_unlock(&mutex_ready_list);

	hilo->estado = ESTADO_EXEC;
	hilo->timestamp_entrar_exec = estructuras_current_timestamp();

	pthread_mutex_lock(&mutex_exec_list);
	list_add(exec_list, hilo);
	pthread_mutex_unlock(&mutex_exec_list);

    return hilo;
}

t_hilo * colas_mover_exec_finish(t_hilo * hilo_mover) {
	bool son_iguales(void * hilo2) { return hilo_mover->pid == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_remove_by_condition(exec_list, son_iguales);
	pthread_mutex_unlock(&mutex_exec_list);

	hilo->estado = FINISH;

	pthread_mutex_lock(&mutex_finish_queue);
	queue_push(finish_queue, hilo);
	pthread_mutex_unlock(&mutex_finish_queue);

    return hilo;
}

t_hilo * colas_mover_exec_block(t_hilo * hilo_mover) {
	bool son_iguales(void * hilo2) { return hilo_mover->pid == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_remove_by_condition(exec_list, son_iguales);
	pthread_mutex_unlock(&mutex_exec_list);

	hilo->estado = BLOCK;
	hilo->timestamp_salir_exec = estructuras_current_timestamp();
	hilo->timestamp_tiempo_exec = estructuras_timestamp_diff(hilo->timestamp_entrar_exec, hilo->timestamp_salir_exec);

	pthread_mutex_lock(&mutex_blocked_list);
	list_add(blocked_list, 0);
	pthread_mutex_unlock(&mutex_blocked_list);

	// TODO: Llamar funcion para consultar si hay que moverlo a bloqueado-suspendido

    return hilo;
}

t_hilo * colas_mover_block_ready(t_hilo * hilo_mover) {
	bool son_iguales(void * hilo2) { return hilo_mover->pid == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_blocked_list);
	t_hilo * hilo = list_remove_by_condition(blocked_list, son_iguales);
	pthread_mutex_unlock(&mutex_blocked_list);

	hilo->estado = READY;
	hilo->timestamp_entrar_ready = estructuras_current_timestamp();

	pthread_mutex_lock(&mutex_ready_list);
	list_add_in_index(ready_list, 0, hilo);
	pthread_mutex_unlock(&mutex_ready_list);

    return hilo;
}

t_hilo * colas_mover_block_block_susp() {
	// TODO: Armar criterio de busqueda
	pthread_mutex_lock(&mutex_blocked_list);
	int ultimo_bloqueado_pos = list_size(blocked_list);
	t_hilo * hilo = list_remove(blocked_list, ultimo_bloqueado_pos - 1);
	pthread_mutex_unlock(&mutex_blocked_list);

	hilo->estado = SUSPENDED_BLOCK;

	pthread_mutex_lock(&mutex_suspended_blocked_list);
	list_add(suspended_blocked_list, hilo);
	pthread_mutex_unlock(&mutex_suspended_blocked_list);

    return hilo;
}

t_hilo * colas_mover_block_susp_block_ready(t_hilo * hilo_mover) {
	bool son_iguales(void * hilo2) { return hilo_mover->pid == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_suspended_blocked_list);
	t_hilo * hilo = list_remove_by_condition(suspended_blocked_list, son_iguales);
	pthread_mutex_unlock(&mutex_suspended_blocked_list);

	hilo->estado = SUSPENDED_READY;

	pthread_mutex_lock(&mutex_suspended_ready_queue);
	queue_push(suspended_ready_queue, hilo);
	pthread_mutex_unlock(&mutex_suspended_ready_queue);

    return hilo;
}

t_hilo * colas_mover_block_ready_ready() {
	pthread_mutex_lock(&mutex_suspended_ready_queue);
	t_hilo * hilo = queue_pop(suspended_ready_queue);
	pthread_mutex_unlock(&mutex_suspended_ready_queue);

	hilo->estado = READY;
	hilo->timestamp_entrar_ready = estructuras_current_timestamp();

	pthread_mutex_lock(&mutex_ready_list);
	list_add_in_index(ready_list, 0, hilo);
	pthread_mutex_unlock(&mutex_ready_list);

	return hilo;
}


