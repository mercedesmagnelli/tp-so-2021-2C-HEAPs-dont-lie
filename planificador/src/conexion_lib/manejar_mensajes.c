
#include "manejar_mensajes.h"

void desconexion(t_prot_mensaje * mensaje);

int lib_recibir_mensaje(int socket_ram) {
	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket_ram);

	return manejar_mensaje(mensaje);
}

int manejar_mensaje(t_prot_mensaje * mensaje) {
	bool seguir_esperando_mensajes = true;
	void dejar_de_esperar_mensajes() { seguir_esperando_mensajes = false; }

	while (seguir_esperando_mensajes) {
		t_matelib_semaforo * semaforo;
		t_estado_ejecucion ejecucion_semaforo;
		t_ram_respuesta * respuesta_ram;

		switch (mensaje->head) {
		case MATELIB_INIT:
			loggear_info("[MATELIB_INIT], hay que crear un proceso");
			t_matelib_nuevo_proceso * nuevo_proceso = deserializar_crear_proceso(mensaje->payload);

			planificadores_proceso_iniciar(nuevo_proceso->pid); // AVISA AL PLANIFICADOR DE LARGO PLAZO Y CREA ESTRUCTURAS

			respuesta_ram = ram_enviar_init(nuevo_proceso);

			hilos_wait_ejecucion(nuevo_proceso->pid); // ESPERAMOS A QUE ENTRE EN EJECUCION

			if (respuesta_ram->respuesta == EXITO_EN_LA_TAREA) {
				loggear_info("[PID: %zu] - [Mensaje] - Se creo el proceso y ya esta en ejecucion", nuevo_proceso->pid);
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			} else {
				loggear_info("[PID: %zu] - [Mensaje] - Ocurrio un problema en la RAM al hacer MATE_INIT", nuevo_proceso->pid);
				enviar_mensaje_protocolo(mensaje->socket, respuesta_ram->respuesta, 0, NULL);
			}

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_CLOSE:
			loggear_info("[MATELIB_CLOSE], hay que cerrar el proceso");

			t_matelib_nuevo_proceso * muerto_proceso = deserializar_crear_proceso(mensaje->payload);

			int respuesta_close = planificadores_proceso_cerrar(muerto_proceso->pid);

			if (respuesta_close == 0) {
				loggear_info("[MATELIB_CLOSE], se cerro el proceso");

				respuesta_ram = ram_enviar_close(muerto_proceso);

				enviar_mensaje_protocolo(mensaje->socket, respuesta_ram->respuesta, respuesta_ram->size, respuesta_ram->mensaje);
			} else {
				loggear_error("[MATELIB_CLOSE], no se pudo cerrar el proceso");
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_SEM_INIT:
			loggear_info("[MATELIB_SEM_INIT], Mensaje para crear un semaforo");

			semaforo = deserializar_semaforo(mensaje->payload);
			ejecucion_semaforo = semaforo_iniciar(semaforo);

			if (ejecucion_semaforo == SEM_OK) {
				loggear_info("[PID: %zu] - [Mensaje] - Se creo el semaforo", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			} else {
				loggear_error("[PID: %zu] - [Mensaje] - No se pudo crear el semaforo", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_SEM_WAIT:
			loggear_info("[MATELIB_SEM_WAIT], reducir en uno el contador del semaforo y bloquear cuando corresponda");

			semaforo = deserializar_semaforo(mensaje->payload);
			ejecucion_semaforo = semaforo_wait(semaforo);

			if (ejecucion_semaforo == SEM_OK) {
				loggear_info("[PID: %zu] - [Mensaje] - WAIT se hizo WAIT del semaforo", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			} else if (ejecucion_semaforo == SEM_BLOQUEAR) {
				loggear_info("[PID: %zu] - [Mensaje] - WAIT hay que bloquear el hilo", semaforo->pid);

				colas_mover_exec_block(SEMAFORO, semaforo->semaforo_nombre, semaforo->pid);

				hilos_wait_ejecucion(semaforo->pid); // ESPERAMOS A QUE ENTRE EN EJECUCION

				loggear_info("[PID: %zu] - [Mensaje] - WAIT termino el bloqueo del hilo", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			} else {
				loggear_error("[PID: %zu] - [Mensaje] - No se pudo crear el semaforo", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_SEM_POST:
			loggear_info("[MATELIB_SEM_POST], incrementar en uno el contador del semaforo y tal vez desbloquear un proceso");

			semaforo = deserializar_semaforo(mensaje->payload);
			ejecucion_semaforo = semaforo_post(semaforo);

			if (ejecucion_semaforo == SEM_OK) {
				loggear_info("[PID: %zu] - [Mensaje] - POST se envio el desbloqueo", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			} else {
				loggear_error("[PID: %zu] - [Mensaje] - POST no se pudo ejeuctar", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_SEM_DESTROY:
			loggear_info("[MATELIB_SEM_DESTROY], destruir un semaforo y eliminar todos los bloqueos que existan");

			semaforo = deserializar_semaforo(mensaje->payload);
			ejecucion_semaforo = semaforo_destruir(semaforo);

			if (ejecucion_semaforo == SEM_OK) {
				loggear_info("[PID: %zu] - [Mensaje] - SEM DESTROY se destruyo el semaforo y desbloquearon hilos asociados", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			} else {
				loggear_error("[PID: %zu] - [Mensaje] - SEM DESTROY no se pudo ejeuctar", semaforo->pid);
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_CALL_IO:
			loggear_info("[MATELIB_CALL_IO], bloquear un proceso porque llama a IO");
			t_matelib_io * io = deserializar_io(mensaje->payload);

			int respuesta_io = dispositivo_io_usar(io->pid, io->io_nombre);
			if (respuesta_io == -1) {
				loggear_error("[PID: %zu] - [Mensaje] - Ocurrio un problema al usar el dispositivo");
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);

				destruir_mensaje(mensaje);
				desconexion(mensaje);

				return 0;
			}

			hilos_wait_ejecucion(io->pid);

			loggear_info("[PID: %zu] - [Mensaje] - Retornamos a la LIB que termino con el dispositivo", io->pid);
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_MEM_ALLOC:
			loggear_info("[MATELIB_MEM_ALLOC]");

			t_matelib_memoria_alloc * alloc = deserializar_memoria_alloc(mensaje->payload);

			loggear_info("[MATELIB_MEM_ALLOC], Enviamos mensaje a la ram");

			respuesta_ram = ram_enviar_alloc(alloc);

			loggear_info("[MATELIB_MEM_ALLOC], Recibimos respuesta, respondemos a la matelib");

			enviar_mensaje_protocolo(mensaje->socket, respuesta_ram->respuesta, respuesta_ram->size, respuesta_ram->mensaje);

			free(respuesta_ram->mensaje);
			free(respuesta_ram);

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_MEM_FREE:
			loggear_info("[MATELIB_MEM_FREE]");

			t_matelib_memoria_free * free_memoria = deserializar_memoria_free(mensaje->payload);

			loggear_info("[MATELIB_MEM_FREE], Enviamos mensaje a la ram");

			respuesta_ram = ram_enviar_free(free_memoria);

			loggear_info("[MATELIB_MEM_FREE], Recibimos respuesta, respondemos a la matelib");

			enviar_mensaje_protocolo(mensaje->socket, respuesta_ram->respuesta, respuesta_ram->size, respuesta_ram->mensaje);

			free(respuesta_ram);

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_MEM_READ:
			loggear_info("[MATELIB_MEM_READ]");

			t_matelib_memoria_read * read_memoria = deserializar_memoria_read(mensaje->payload);

			loggear_info("[MATELIB_MEM_READ], Enviamos mensaje a la ram");

			respuesta_ram = ram_enviar_read(read_memoria);

			loggear_info("[MATELIB_MEM_READ], Recibimos respuesta, respondemos a la matelib");

			enviar_mensaje_protocolo(mensaje->socket, respuesta_ram->respuesta, respuesta_ram->size, respuesta_ram->mensaje);

			free(respuesta_ram->mensaje);
			free(respuesta_ram);

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case MATELIB_MEM_WRITE:
			loggear_info("[MATELIB_MEM_WRITE]");

			t_matelib_memoria_alloc * write_memoria = deserializar_memoria_alloc(mensaje->payload);

			loggear_info("[MATELIB_MEM_WRITE], Enviamos mensaje a la ram");

			respuesta_ram = ram_enviar_alloc(write_memoria);

			loggear_info("[MATELIB_MEM_WRITE], Recibimos respuesta, respondemos a la matelib");

			enviar_mensaje_protocolo(mensaje->socket, respuesta_ram->respuesta, respuesta_ram->size, respuesta_ram->mensaje);

			free(respuesta_ram);

			destruir_mensaje(mensaje);
			desconexion(mensaje);

			return 0;
		case DESCONEXION:
			loggear_info("Se desconectaron");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case FALLO_AL_RECIBIR:
		default:
			loggear_error("Error al recibir mensaje");

			loggear_error("Ocurrio un error al recibir el mensaje");
			loggear_error("Mensaje recibido Head: %d", mensaje->head);
			loggear_error("Tamaño de mensaje: %d", mensaje->tamanio_total);

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return ERROR_MENSAJE_DESCONOCIDO;
		}

		int socket_escuchar = mensaje->socket;

		if (seguir_esperando_mensajes) {
			mensaje = recibir_mensaje_protocolo(socket_escuchar);
		} else {
			loggear_warning("No escuchamos mas mensajes, Socket: %d", socket_escuchar);
		}
	}

	return 0;
}


void desconexion(t_prot_mensaje * mensaje) {
	loggear_debug("Se cierra el socket");

	close(mensaje->socket);
}

