
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
			//TODO deserealizar mensaje  t_matelib_memoria_alloc*
			uint32_t alloc_PID = 0;
			uint32_t alloc_memoria_size = 0;

			int32_t ptroAlloc = memalloc(alloc_PID, alloc_memoria_size);

			if(ptroAlloc>=0){
				loggear_info("[MATELIB_MEM_ALLOC], proceso %d se le asigna espacio solicitado", alloc_PID);
				enviar_mensaje_protocolo(mensaje->socket, 200, 4, &ptroAlloc);//TODO suplantar 200 con header de Alloc realizado correctamente
			}else{
				loggear_info("[MATELIB_MEM_ALLOC], proceso %d NO se le asigna espacio solicitado", alloc_PID);
				enviar_mensaje_protocolo(mensaje->socket, 100, 0, NULL);//TODO suplantar 100 con header de Alloc ERROR
			}

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_FREE:
			loggear_info("[MATELIB_MEM_FREE], un proceso libera memoria de la RAM");
			//TODO deserealizar mensaje  t_matelib_memoria_free*
			uint32_t free_PID = 0;
			int32_t free_memoria_mate_pointer = 0;

			int32_t rtaFree = memfree(free_memoria_mate_pointer, free_PID);
			uint32_t headerF;

			if(rtaFree>=0){
				loggear_info("[MATELIB_MEM_FREE], proceso %d se libero el espacio seleccionado", free_PID);
				headerF = 400;//TODO suplantar 400 con header de Free realizado correctamente
			}else{
				loggear_info("[MATELIB_MEM_FREE], proceso %d NO se libero el espacio seleccionado", free_PID);
				headerF = 300;//TODO suplantar 300 con header de Free ERROR
			}

			enviar_mensaje_protocolo(mensaje->socket, headerF, 0, NULL);

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_READ:
			loggear_info("[MATELIB_MEM_READ], un proceso quiere leer algo de la RAM");
			//TODO deserealizar mensaje t_matelib_memoria_read*
			uint32_t read_PID = 0;
			uint32_t read_memoria_size = 0;
			int32_t read_memoria_mate_pointer = 0;

			void* ptroLectura = NULL;
			int32_t rtaRead = memread(read_memoria_mate_pointer, read_PID, read_memoria_size, ptroLectura);

			if(rtaRead>=0){
				loggear_info("[MATELIB_MEM_READ], proceso %d pudo leer el espacio seleccionado", read_PID);
				enviar_mensaje_protocolo(mensaje->socket, 600, read_memoria_size, ptroLectura);//TODO suplantar 600 con header de READ realizado correctamente
			}else{
				loggear_info("[MATELIB_MEM_READ], proceso %d NO pudo leer el espacio seleccionado", read_PID);
				enviar_mensaje_protocolo(mensaje->socket, 500, 0, NULL);//TODO suplantar 500 con header de READ ERROR
			}

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_WRITE:
			loggear_info("[MATELIB_MEM_WRITE], un proceso quiere escribir en la RAM");
			//TODO deserealizar mensaje t_matelib_memoria_write*
			uint32_t write_PID = 0;
			uint32_t write_memoria_size = 0;
			int32_t write_memoria_mate_pointer = 0;
			void * write_memoria_write = NULL;

			int32_t rtaWrite = memwrite(write_memoria_write, write_memoria_mate_pointer, write_PID, write_memoria_size);
			uint32_t headerW;
			if(rtaWrite>=0){
				loggear_info("[MATELIB_MEM_WRITE], proceso %d pudo escribir en el espacio seleccionado", write_PID);
				headerW = 800;//TODO suplantar 800 con header de WRITE realizado correctamente
			}else{
				loggear_info("[MATELIB_MEM_WRITE], proceso %d NO pudo escribir el espacio seleccionado", write_PID);
				headerW=700;//TODO suplantar 700 con header de WRITE realizado correctamente
			}

			enviar_mensaje_protocolo(mensaje->socket, headerW, 0, NULL);

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

