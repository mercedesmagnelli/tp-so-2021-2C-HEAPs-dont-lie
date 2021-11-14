
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
			//Nos mandan un uint32_t PID para eliminar el proceso

			uint32_t PID_a_liberar=0;

			int32_t rtaClose = close_PID(PID_a_liberar);
			uint32_t headerC;

			if(rtaClose){
				loggear_info("[MATELIB_CLOSE], proceso %d fue eliminado de memoria", PID_a_liberar);
				headerC = CLOSE_SUC_R_P;
			}else{
				loggear_info("[MATELIB_CLOSE], proceso %d NO fue eliminado de memoria", PID_a_liberar);
				headerC = CLOSE_ERR_R_P;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerC, 0, NULL);

			desconexion(mensaje);
			destruir_mensaje(mensaje);
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
			t_matelib_memoria_alloc* alloc = deserializar_memoria_alloc(mensaje->payload);

			int32_t ptroAlloc = memalloc(alloc->pid, alloc->memoria_size);

			if(ptroAlloc>=0){
				loggear_info("[MATELIB_MEM_ALLOC], proceso %d se le asigna espacio solicitado", alloc->pid);
				enviar_mensaje_protocolo(mensaje->socket, ALLOC_SUC_R_P, 4, &ptroAlloc);
			}else{
				loggear_info("[MATELIB_MEM_ALLOC], proceso %d NO se le asigna espacio solicitado", alloc->pid);
				enviar_mensaje_protocolo(mensaje->socket, ALLOC_ERR_R_P, 0, NULL);
			}

			free(alloc);

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_FREE:
			loggear_info("[MATELIB_MEM_FREE], un proceso libera memoria de la RAM");

			t_matelib_memoria_free* free1 = deserializar_memoria_free(mensaje->payload);

			int32_t rtaFree = memfree(free1->memoria_mate_pointer, free1->pid);
			uint32_t headerF;

			if(rtaFree>=0){
				loggear_info("[MATELIB_MEM_FREE], proceso %d se libero el espacio seleccionado", free1->pid);
				headerF = FREE_SUC_R_P;
			}else{
				loggear_info("[MATELIB_MEM_FREE], proceso %d NO se libero el espacio seleccionado", free1->pid);
				headerF = FREE_ERR_R_P;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerF, 0, NULL);

			free(free1);

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_READ:
			loggear_info("[MATELIB_MEM_READ], un proceso quiere leer algo de la RAM");
			t_matelib_memoria_read* read = deserializar_memoria_read(mensaje->payload);


			void* ptroLectura = NULL;
			int32_t rtaRead = memread(read->memoria_mate_pointer, read->pid, read->memoria_size, ptroLectura);

			if(rtaRead>=0){
				loggear_info("[MATELIB_MEM_READ], proceso %d pudo leer el espacio seleccionado", read->pid);
				enviar_mensaje_protocolo(mensaje->socket, READ_SUC_R_P, read->memoria_size, ptroLectura);
			}else{
				loggear_info("[MATELIB_MEM_READ], proceso %d NO pudo leer el espacio seleccionado", read->pid);
				enviar_mensaje_protocolo(mensaje->socket, READ_ERR_R_P, 0, NULL);
			}

			free(ptroLectura);

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_WRITE:
			loggear_info("[MATELIB_MEM_WRITE], un proceso quiere escribir en la RAM");
			t_matelib_memoria_read* escritura = deserializar_memoria_read(mensaje->payload);

			void * write_memoria_write = NULL;

			int32_t rtaWrite = memwrite(write_memoria_write, escritura->memoria_mate_pointer, escritura->pid, escritura->memoria_size);
			uint32_t headerW;

			if(rtaWrite>=0){
				loggear_info("[MATELIB_MEM_WRITE], proceso %d pudo escribir en el espacio seleccionado", escritura->pid);
				headerW = WRITE_SUC_R_P;
			}else{
				loggear_info("[MATELIB_MEM_WRITE], proceso %d NO pudo escribir el espacio seleccionado", escritura->pid);
				headerW=WRITE_ERR_R_P;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerW, 0, NULL);

			free(escritura);

			desconexion(mensaje);
			destruir_mensaje(mensaje);
			return 0;
		case SUSPENDER_PROCESO:
			loggear_info("[MATELIB_CLOSE], hay que cerrar el proceso");
			//Nos mandan un uint32_t PID para eliminar el proceso

			uint32_t PID_a_suspender=0;

			int32_t rtaSuspender = suspender_PID(PID_a_suspender);
			uint32_t headerS;

			if(rtaSuspender){
				loggear_info("[SUSPENDER_PROCESO], proceso %d fue suspendido", PID_a_suspender);
				headerS = SUSPEN_SUC_R_P;
			}else{
				loggear_info("[SUSPENDER_PROCESO], proceso %d NO fue suspendido", PID_a_suspender);
				headerS = SUSPEN_ERR_R_P;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerS, 0, NULL);

			desconexion(mensaje);
			destruir_mensaje(mensaje);
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

