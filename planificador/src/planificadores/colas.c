#include "colas.h"

/**
 * TODOs
 * 1. [Listo] Crear todas las colas y listas
 * 2. [Listo] Crear la destruccion de las colas y listas
 * 3. [Listo] Pensar en que estructuras se van a pasar entre estados, todos t_hilo excepto en NEW
 * 4. [Listo] Modificar los void * y los return donde corresponda para pasarlo por parametro o retornarlo
 * 5. [Listo] Programar la forma de comparar hilos (supongo que comparacion por .pid)
 * 6. [Listo] Recibir algoritmo de corto plazo para mover entre READY->EXEC
 * 7. [Listo] Recibir algoritmo de mediano plazo para mover de BLOCK->SUSP/BLOCK
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
	hilo->estado = ESTADO_NEW;
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
	hilo->estado = ESTADO_READY;
	hilo->bloqueante = NINGUNO;
	hilo->nombre_bloqueante = "";

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

t_hilo * colas_mover_exec_finish(uint32_t pid_mover) {
	bool son_iguales(void * hilo2) { return pid_mover == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_remove_by_condition(exec_list, son_iguales);
	pthread_mutex_unlock(&mutex_exec_list);

	hilo->estado = ESTADO_FINISH;

	pthread_mutex_lock(&mutex_finish_queue);
	queue_push(finish_queue, hilo);
	pthread_mutex_unlock(&mutex_finish_queue);

    return hilo;
}

t_hilo * colas_obtener_finalizado() {
	pthread_mutex_lock(&mutex_finish_queue);
	t_hilo * hilo = queue_pop(finish_queue);
	pthread_mutex_unlock(&mutex_finish_queue);

	return hilo;
}

t_hilo * colas_mover_exec_block(t_dispositivo_bloqueante dispositivo_bloqueante, char * nombre_bloqueante, uint32_t pid) {
	bool son_iguales(void * hilo2) { return pid == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_remove_by_condition(exec_list, son_iguales);
	pthread_mutex_unlock(&mutex_exec_list);

	hilo->estado = ESTADO_BLOCK;
	hilo->bloqueante = dispositivo_bloqueante;
	hilo->nombre_bloqueante = nombre_bloqueante;
	hilo->timestamp_salir_exec = estructuras_current_timestamp();
	hilo->timestamp_tiempo_exec = estructuras_timestamp_diff(hilo->timestamp_entrar_exec, hilo->timestamp_salir_exec);

	pthread_mutex_lock(&mutex_blocked_list);
	list_add(blocked_list, hilo);
	pthread_mutex_unlock(&mutex_blocked_list);

	hilos_post_nuevo_bloqueado();

    return hilo;
}

t_hilo * colas_mover_block_ready(t_hilo * hilo_mover) {
	bool son_iguales(void * hilo2) { return hilo_mover->pid == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_blocked_list);
	t_hilo * hilo = list_remove_by_condition(blocked_list, son_iguales);
	pthread_mutex_unlock(&mutex_blocked_list);

	hilo->estado = ESTADO_READY;
	hilo->timestamp_entrar_ready = estructuras_current_timestamp();

	pthread_mutex_lock(&mutex_ready_list);
	list_add_in_index(ready_list, 0, hilo);
	pthread_mutex_unlock(&mutex_ready_list);

    return hilo;
}

t_hilo * colas_mover_block_block_susp() {
	pthread_mutex_lock(&mutex_blocked_list);
	int ultimo_bloqueado_pos = list_size(blocked_list);
	t_hilo * hilo = list_remove(blocked_list, ultimo_bloqueado_pos - 1);
	pthread_mutex_unlock(&mutex_blocked_list);

	hilo->estado = ESTADO_SUSPENDED_BLOCK;

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

	hilo->estado = ESTADO_SUSPENDED_READY;

	pthread_mutex_lock(&mutex_suspended_ready_queue);
	queue_push(suspended_ready_queue, hilo);
	pthread_mutex_unlock(&mutex_suspended_ready_queue);

	hilos_post_new();

    return hilo;
}

t_hilo * colas_mover_block_ready_ready() {
	pthread_mutex_lock(&mutex_suspended_ready_queue);
	t_hilo * hilo = queue_pop(suspended_ready_queue);
	pthread_mutex_unlock(&mutex_suspended_ready_queue);

	hilo->estado = ESTADO_READY;
	hilo->timestamp_entrar_ready = estructuras_current_timestamp();

	pthread_mutex_lock(&mutex_ready_list);
	list_add_in_index(ready_list, 0, hilo);
	pthread_mutex_unlock(&mutex_ready_list);

	return hilo;
}


bool deberia_suspenderse_procesos() {
	pthread_mutex_lock(&mutex_suspended_blocked_list);
	bool hay_procesos_bloqueados = list_size(blocked_list) > 0;
	pthread_mutex_unlock(&mutex_suspended_blocked_list);

	if (hay_procesos_bloqueados) {
		pthread_mutex_lock(&mutex_ready_list);
		bool no_hay_procesos_ready = list_size(ready_list) == 0;
		pthread_mutex_unlock(&mutex_ready_list);

		if (no_hay_procesos_ready) {
			pthread_mutex_lock(&mutex_new_queue);
			bool hay_procesos_new = queue_size(new_queue) > 0;
			pthread_mutex_unlock(&mutex_new_queue);

			if (hay_procesos_new) {
				return true;
			}
		}
	}

	return false;
}

bool hay_procesos_en_suspendido_ready() {
	pthread_mutex_lock(&mutex_suspended_ready_queue);
	bool hay_procesos = queue_size(suspended_ready_queue) > 0;
	pthread_mutex_unlock(&mutex_suspended_ready_queue);

	return hay_procesos;
}

t_hilo * colas_desbloquear_1_hilo(t_dispositivo_bloqueante dispositivo_bloqueante, char * dispositivo_nombre) {
	loggear_error("TODO: REVISAR ESTA FUNCION, COLAS_DESBLOQUEAR_1_HILO");
	bool esta_bloqueado(void * hilo2) {
		t_hilo * hilo_comparar = (t_hilo *) hilo2;

		return dispositivo_bloqueante == hilo_comparar->bloqueante && dispositivo_nombre == hilo_comparar->nombre_bloqueante;
	}

	t_hilo * hilo_retornar;
	bool esta_en_bloqueado;
	bool esta_en_suspendido;

	pthread_mutex_lock(&mutex_blocked_list);
	esta_en_bloqueado = list_any_satisfy(blocked_list, esta_bloqueado);
	if (esta_en_bloqueado) {
		hilo_retornar = list_find(blocked_list, esta_bloqueado);
	}
	pthread_mutex_unlock(&mutex_blocked_list);

	if (esta_en_bloqueado) {
		colas_mover_block_ready(hilo_retornar);

		hilos_post_ready();

		return hilo_retornar;
	}

	pthread_mutex_lock(&mutex_suspended_blocked_list);
	esta_en_suspendido = list_any_satisfy(suspended_blocked_list, esta_bloqueado);
	if (esta_en_suspendido) {
		hilo_retornar = list_find(suspended_blocked_list, esta_bloqueado);
	}
	pthread_mutex_unlock(&mutex_suspended_blocked_list);

	if (esta_en_suspendido) {
		colas_mover_block_susp_block_ready(hilo_retornar);

		return hilo_retornar;
	}

	return NULL;
}

t_hilo * colas_desbloquear_hilo_concreto(t_hilo * hilo_bloqueado) {
	if (hilo_bloqueado->estado == ESTADO_BLOCK) {
		colas_mover_block_ready(hilo_bloqueado);
		hilos_post_ready();
		loggear_debug("[PID: %zu] - Se desbloqueo el hilo y se envio a READY", hilo_bloqueado->pid);
	} else if (hilo_bloqueado->estado == ESTADO_SUSPENDED_BLOCK) {
		colas_mover_block_susp_block_ready(hilo_bloqueado);
		loggear_debug("[PID: %zu] - Se desbloqueo el hilo y se envio a SUSPENDIDO-READY", hilo_bloqueado->pid);
	} else {
		loggear_error("[PID: %zu] - Se quiso desbloquear el hilo, pero no esta en estado bloqueado, esta en %d", hilo_bloqueado->pid, hilo_bloqueado->estado);
	}

	return hilo_bloqueado;
}

void colas_desbloquear_todos_hilos(t_dispositivo_bloqueante dispositivo_bloqueante, char * dispositivo_nombre) {
	bool esta_bloqueado(void * hilo2) {
		t_hilo * hilo_comparar = (t_hilo *) hilo2;

		return dispositivo_bloqueante == hilo_comparar->bloqueante && dispositivo_nombre == hilo_comparar->nombre_bloqueante;
	}

	t_hilo * hilo_mover = NULL;
	while (hilo_mover == NULL) {
		pthread_mutex_lock(&mutex_blocked_list);
		hilo_mover = list_find(blocked_list, esta_bloqueado);
		pthread_mutex_unlock(&mutex_blocked_list);

		colas_mover_block_ready(hilo_mover);

		hilos_post_ready();
	}

	hilo_mover = NULL;
	while (hilo_mover == NULL) {
		pthread_mutex_lock(&mutex_suspended_blocked_list);
		hilo_mover = list_find(suspended_blocked_list, esta_bloqueado);
		pthread_mutex_unlock(&mutex_suspended_blocked_list);

		colas_mover_block_susp_block_ready(hilo_mover);
	}
}



