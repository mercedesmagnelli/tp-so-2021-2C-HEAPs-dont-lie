
#include "manejar_mensajes.h"

void desconexion(t_prot_mensaje * mensaje);

int recibir_mensaje(int socket) {
	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket);

	return manejar_mensaje(mensaje);
}

int manejar_mensaje(t_prot_mensaje * mensaje) {
	bool seguir_esperando_mensajes = true;
	void dejar_de_esperar_mensajes() { seguir_esperando_mensajes = false; }

	while (seguir_esperando_mensajes) {

		switch (mensaje->head) {
		case HANDSHAKE_P_R:
			enviar_mensaje_protocolo(mensaje->socket, HANDSHAKE_R_P, 0, NULL);

			loggear_info("Llego un handshake de la ram, devolvemos el saludo");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case HANDSHAKE_F_R:
			enviar_mensaje_protocolo(mensaje->socket, HANDSHAKE_R_F, 0, NULL);

			loggear_info("Llego un handshake del filesystem, devolvemos el saludo");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_INIT:
			loggear_info("[MATELIB_INIT], hay que crear un proceso");

			return 0;
		case MATELIB_CLOSE:
			loggear_info("[MATELIB_CLOSE], hay que cerrar el proceso");

			return 0;
		case MATELIB_SEM_INIT:
			loggear_info("[MATELIB_SEM_INIT], se crea un semaforo");

			return 0;
		case MATELIB_SEM_WAIT:
			loggear_info("[MATELIB_SEM_WAIT], reducir en uno el contador del semaforo y tal vez bloquear un proceso");

			return 0;
		case MATELIB_SEM_POST:
			loggear_info("[MATELIB_SEM_POST], incrementar en uno el contador del semaforo y tal vez desbloquear un proceso");

			return 0;
		case MATELIB_SEM_DESTROY:
			loggear_info("[MATELIB_SEM_DESTROY], destruir un semaforo y eliminar todos los bloqueos que existan");

			return 0;
		case MATELIB_CALL_IO:
			loggear_info("[MATELIB_CALL_IO], bloquear un proceso porque llama a IO");

			return 0;
		case MATELIB_MEM_ALLOC:
			loggear_info("[MATELIB_MEM_ALLOC], un proceso pide espacio en la RAM");
			/*
			 * Llega PID y la cantidad de memoria a solicitar (deserealizar)
			 *
			 * usamos la funcion int32_t solicitar_espacio_memoria(int32_t PID,int32_t cantMemoria) de paginación
			 *
			 * Enviamos a PLANI el puntero logico donde se otorgo la memoria solicitada
			 */
			return 0;
		case MATELIB_MEM_FREE:
			loggear_info("[MATELIB_MEM_FREE], un proceso libera memoria de la RAM");
			/*
			 * Llega PID y el puntero donde esta el alloc a liberar (deserealizar)
			 *
			 * usamos la funcion int memfree(int32_t PID, int32_t ptroMem) de memoria.c
			 *
			 * Enviamos el HANDSHAKE diciendo que se pudo liberar la memoria
			 */
			return 0;
		case MATELIB_MEM_READ:
			loggear_info("[MATELIB_MEM_READ], un proceso quiere leer algo de la RAM");
			/*
			 * Llega PID y puntero de lectura (deserealizar)
			 *
			 * usamos la funcion int32_t leer_espacio_memoria(int32_t PID, int32_t ptroMem, void* data)  de paginación
			 *
			 * Enviamos tamanio de dato leido y la data
			 */
			return 0;
		case MATELIB_MEM_WRITE:
			loggear_info("[MATELIB_MEM_WRITE], un proceso quiere escribir en la RAM");
			/*
			 * Llega PID, puntero donde escribir, tamanio data y data a escribir (deserealizar)
			 *
			 * usamos la funcion int escribir_espacio_memoria(int32_t PID, int32_t ptroLogicoMem, int32_t tamanioData, void* data) de paginacion
			 *
			 * Enviamos un HANDSHAKE diciendo que se pudo escribir en memoria
			 */
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

