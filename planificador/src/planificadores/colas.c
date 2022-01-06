#include "colas.h"

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
	hilo->estimacion_anterior = (double) get_estimacion_inicial();
	hilo->timestamp_tiempo_exec = 0.0;
	hilo->sera_eliminado_por_deadlock = false;
	//hilo->bloqueante = NINGUNO;
	//hilo->nombre_bloqueante = "";
	hilo->semaforos_pedidos = list_create();

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

	hilos_post_ready();

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

	hilos_post_finalizado();
	hilos_post_multitarea();
	hilos_post_multiprogramacion();

    return hilo;
}

t_hilo * colas_mover_block_finish(t_hilo * hilo_mover) {
	bool son_iguales(void * hilo2) { return hilo_mover->pid == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_blocked_list);
	t_hilo * hilo = list_remove_by_condition(blocked_list, son_iguales);
	pthread_mutex_unlock(&mutex_blocked_list);

	hilo->estado = ESTADO_FINISH;
	hilo->bloqueante = NINGUNO;
	hilo->nombre_bloqueante = "";

	pthread_mutex_lock(&mutex_finish_queue);
	queue_push(finish_queue, hilo);
	pthread_mutex_unlock(&mutex_finish_queue);

	hilos_post_finalizado();
	hilos_post_multiprogramacion();

    return hilo;
}

t_hilo * colas_mover_susp_block_finish(t_hilo * hilo_mover) {
	bool son_iguales(void * hilo2) { return hilo_mover->pid == ((t_hilo *) hilo2)->pid; }
	pthread_mutex_lock(&mutex_suspended_blocked_list);
	t_hilo * hilo = list_remove_by_condition(suspended_blocked_list, son_iguales);
	pthread_mutex_unlock(&mutex_suspended_blocked_list);

	hilo->estado = ESTADO_FINISH;
	hilo->bloqueante = NINGUNO;
	hilo->nombre_bloqueante = "";

	pthread_mutex_lock(&mutex_finish_queue);
	queue_push(finish_queue, hilo);
	pthread_mutex_unlock(&mutex_finish_queue);

	hilos_post_finalizado();

    return hilo;
}


t_hilo * colas_obtener_finalizado() {
	pthread_mutex_lock(&mutex_finish_queue);
	t_hilo * hilo = queue_pop(finish_queue);
	pthread_mutex_unlock(&mutex_finish_queue);

	return hilo;
}

t_hilo * colas_mover_exec_block(t_dispositivo_bloqueante dispositivo_bloqueante, char * nombre_bloqueante, uint32_t ppid) {
	bool son_iguales(void * hilo2) { return ppid == pid(hilo2); }
	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_remove_by_condition(exec_list, son_iguales);
	pthread_mutex_unlock(&mutex_exec_list);

	if (hilo == NULL) {
		loggear_error("[COLAS] [PID: %zu] Mover exec a bloqueado, no se encontro el hilo en la cola de exec", ppid);
		loggear_error("[COLAS] [PID: %zu] Dispositivo: %d, Nombre: %s", ppid, dispositivo_bloqueante, nombre_bloqueante);
		return hilo;
	}

	hilo->estado = ESTADO_BLOCK;
	hilo->timestamp_salir_exec = estructuras_current_timestamp();
	hilo->timestamp_tiempo_exec = estructuras_timestamp_diff(hilo->timestamp_entrar_exec, hilo->timestamp_salir_exec);

	hilo->bloqueante = dispositivo_bloqueante;
	hilo->nombre_bloqueante = nombre_bloqueante;

	pthread_mutex_lock(&mutex_blocked_list);
	list_add(blocked_list, hilo);
	pthread_mutex_unlock(&mutex_blocked_list);

	loggear_debug("[PID: %d] --- Se movió de EXEC a BLOCK", ppid);

	hilos_post_nuevo_bloqueado();

    return hilo;
}

t_hilo * colas_obtener_hilo_en_exec(uint32_t ppid) {
	bool son_iguales(void * hilo2) { return ppid == pid(hilo2); }

	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_find(exec_list, son_iguales);
	pthread_mutex_unlock(&mutex_exec_list);

	return hilo;
}

void colas_agregar_wait_semaforo(uint32_t ppid, void * semaforo) {
	bool son_iguales(void * hilo) { return ppid == pid(hilo); }

	pthread_mutex_lock(&mutex_exec_list);
	t_hilo * hilo = list_find(exec_list, son_iguales);
	list_add(hilo->semaforos_pedidos, semaforo);
	pthread_mutex_unlock(&mutex_exec_list);
}

t_hilo * colas_mover_block_ready(t_hilo * hilo_mover) {
	bool son_iguales(void * hilo2) { return pid(hilo_mover) == pid(hilo2); }
	pthread_mutex_lock(&mutex_blocked_list);
	t_hilo * hilo = list_remove_by_condition(blocked_list, son_iguales);
	pthread_mutex_unlock(&mutex_blocked_list);

	hilo->estado = ESTADO_READY;
	hilo->timestamp_entrar_ready = estructuras_current_timestamp();
	hilo->bloqueante = NINGUNO;
	hilo->nombre_bloqueante = "";

	pthread_mutex_lock(&mutex_ready_list);
	list_add_in_index(ready_list, 0, hilo);
	pthread_mutex_unlock(&mutex_ready_list);

	hilos_post_ready();

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
	bool son_iguales(void * hilo2) { return pid(hilo_mover) == pid(hilo2); }
	pthread_mutex_lock(&mutex_suspended_blocked_list);
	t_hilo * hilo = list_remove_by_condition(suspended_blocked_list, son_iguales);
	pthread_mutex_unlock(&mutex_suspended_blocked_list);

	hilo->estado = ESTADO_SUSPENDED_READY;
	hilo->bloqueante = NINGUNO;
	hilo->nombre_bloqueante = "";

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

	hilos_post_ready();

	return hilo;
}

void colas_bloquear_listas_bloqueados() {
	pthread_mutex_lock(&mutex_blocked_list);
	pthread_mutex_lock(&mutex_suspended_blocked_list);
}

void colas_desbloquear_listas_bloqueados() {
	pthread_mutex_unlock(&mutex_blocked_list);
	pthread_mutex_unlock(&mutex_suspended_blocked_list);
}

t_list * colas_obtener_listas_bloqueados(t_dispositivo_bloqueante dispositivo_bloqueante) {
	bool esta_bloqueado_por(void * hilo) {
		return ((t_hilo *) hilo)->bloqueante == dispositivo_bloqueante;
	}

	t_list * todos_los_bloqueados = list_create();

	colas_bloquear_listas_bloqueados();
	t_list * bloqueados = list_filter(blocked_list, esta_bloqueado_por);
	t_list * suspendidos = list_filter(suspended_blocked_list, esta_bloqueado_por);
	colas_desbloquear_listas_bloqueados();

	list_add_all(todos_los_bloqueados, bloqueados);
	list_add_all(todos_los_bloqueados, suspendidos);

	list_destroy(bloqueados);
	list_destroy(suspendidos);

	return todos_los_bloqueados;
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

t_hilo * colas_desbloquear_hilo_concreto(t_hilo * hilo_bloqueado) {
	if (hilo_bloqueado->estado == ESTADO_BLOCK) {
		colas_mover_block_ready(hilo_bloqueado);
		loggear_debug("[PID: %zu] - Se desbloqueo el hilo y se envio a READY", hilo_bloqueado->pid);
	} else if (hilo_bloqueado->estado == ESTADO_SUSPENDED_BLOCK) {
		colas_mover_block_susp_block_ready(hilo_bloqueado);
		loggear_debug("[PID: %zu] - Se desbloqueo el hilo y se envio a SUSPENDIDO-READY", hilo_bloqueado->pid);
	} else {
		loggear_error("[PID: %zu] - Se quiso desbloquear el hilo, pero no esta en estado bloqueado, esta en %d", hilo_bloqueado->pid, hilo_bloqueado->estado);
	}

	return hilo_bloqueado;
}

t_hilo * colas_finalizar_proceso_bloqueado(t_hilo * hilo_bloqueado) {
	if (hilo_bloqueado->estado == ESTADO_BLOCK) {
		colas_mover_block_finish(hilo_bloqueado);
	} else if (hilo_bloqueado->estado == ESTADO_SUSPENDED_BLOCK) {
		colas_mover_susp_block_finish(hilo_bloqueado);
	} else {
		loggear_error("[COLAS] - El hilo a finalizar no esta bloqueado");
		return NULL;
	}

	return hilo_bloqueado;
}


