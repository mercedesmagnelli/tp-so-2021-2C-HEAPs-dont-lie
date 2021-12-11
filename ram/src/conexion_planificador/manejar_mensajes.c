
#include "manejar_mensajes.h"

void desconexion(t_prot_mensaje * mensaje);

void imprimir_frames2();

void imprimir_tlb2();

int recibir_mensaje(int socket) {
	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket);

	return manejar_mensaje(mensaje);
}

uint32_t get_nuevo_pid() {
	static uint32_t id = 0;
	static pthread_mutex_t mutex_t = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&mutex_t);
	id++;
	pthread_mutex_unlock(&mutex_t);

	return id;
}

int manejar_mensaje(t_prot_mensaje * mensaje) {
	bool seguir_esperando_mensajes = true;
	void dejar_de_esperar_mensajes() { seguir_esperando_mensajes = false; }

	while (seguir_esperando_mensajes) {

		t_matelib_nuevo_proceso * nuevo_proceso;

		switch (mensaje->head) {
		case GENERAR_PID:
			nuevo_proceso = shared_crear_nuevo_proceso(get_nuevo_pid());

			size_t size = sizeof(uint32_t);
			void * serializado = serializiar_crear_proceso(nuevo_proceso, &size);

			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, size, serializado);

			free(serializado);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case HANDSHAKE_P_R:
			enviar_mensaje_protocolo(mensaje->socket, HANDSHAKE_R_P, 0, NULL);

			loggear_info("Llego un handshake del planificador, devolvemos el saludo");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case HANDSHAKE_F_R:

			socket_swap = mensaje->socket;
			loggear_info("Llego un handshake del filesystem, devolvemos el saludo");

			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_INIT:
			loggear_info("[MATELIB_INIT], hay que crear un proceso");
			t_matelib_nuevo_proceso* PID_proceso_nuevo = deserializar_crear_proceso(mensaje->payload);

			int rtaInit = inicializar_proceso(PID_proceso_nuevo->pid);
			int32_t headerI;

			if(rtaInit == 0){
				loggear_info("[MATELIB_INIT], proceso %d fue inicializado en la memoria", PID_proceso_nuevo->pid);
				headerI = EXITO_EN_LA_TAREA;
			}else{
				loggear_error("[MATELIB_INIT], proceso %d NO fue inicializado en la memoria", PID_proceso_nuevo->pid);
				headerI = FALLO_EN_LA_TAREA;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerI, 0, NULL);

			free(PID_proceso_nuevo);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_CLOSE:

			//imprimir_frames2();
			//imprimir_tlb2();

			loggear_info("[MATELIB_CLOSE] - Se solicita eliminar el proceso de memoria");
			t_matelib_nuevo_proceso* PID_proceso_eliminar = deserializar_crear_proceso(mensaje->payload);

			int32_t rtaClose = close_PID(PID_proceso_eliminar->pid);
			//close_pid: 1 lo hace bien, 0 lo hace mal
			uint32_t headerC;

			if(rtaClose){
				loggear_info("[MATELIB_CLOSE], proceso %d fue eliminado de memoria", PID_proceso_eliminar->pid);
				headerC = EXITO_EN_LA_TAREA;
			}else{
				loggear_info("[MATELIB_CLOSE], proceso %d NO fue eliminado de memoria", PID_proceso_eliminar->pid);
				headerC = FALLO_EN_LA_TAREA;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerC, 0, NULL);

			free(PID_proceso_eliminar);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_SEM_INIT:
			loggear_info("[MATELIB_SEM_INIT], se crea un semaforo");

			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_SEM_WAIT:
			loggear_info("[MATELIB_SEM_WAIT], reducir en uno el contador del semaforo y tal vez bloquear un proceso");

			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_SEM_POST:
			loggear_info("[MATELIB_SEM_POST], incrementar en uno el contador del semaforo y tal vez desbloquear un proceso");

			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_SEM_DESTROY:
			loggear_info("[MATELIB_SEM_DESTROY], destruir un semaforo y eliminar todos los bloqueos que existan");

			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_CALL_IO:
			loggear_info("[MATELIB_CALL_IO], bloquear un proceso porque llama a IO");

			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_ALLOC:
			loggear_info("[MATELIB_MEM_ALLOC], un proceso pide espacio en la RAM");
			t_matelib_memoria_alloc* alloc = deserializar_memoria_alloc(mensaje->payload);

			int32_t ptroAlloc = memalloc(alloc->pid, alloc->memoria_size);

			if(ptroAlloc>=0){
				loggear_warning("[MATELIB_MEM_ALLOC], proceso %d se le asigna espacio solicitado en el ptro %d", alloc->pid, ptroAlloc);
				void* paquete_enviar = malloc(sizeof(int32_t));
				memcpy(paquete_enviar, &ptroAlloc, sizeof(int32_t));
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, sizeof(int32_t), paquete_enviar);
				free(paquete_enviar);
			}else{
				loggear_error("[MATELIB_MEM_ALLOC], proceso %d NO se le asigna espacio solicitado", alloc->pid);
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}

			//imprimir_frames2();
			//imprimir_tlb2();

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
				headerF = EXITO_EN_LA_TAREA;
			}else{
				loggear_info("[MATELIB_MEM_FREE], proceso %d NO se libero el espacio seleccionado", free1->pid);
				headerF = FALLO_EN_LA_TAREA;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerF, 0, NULL);

			free(free1);
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_READ:
			loggear_info("[MATELIB_MEM_READ], un proceso quiere leer algo de la RAM");
			t_matelib_memoria_read* read = deserializar_memoria_read(mensaje->payload);


			void* ptroLectura;
			int32_t rtaRead = memread(read->memoria_mate_pointer, read->pid, read->memoria_size, &ptroLectura);

			//loggear_warning("[MATELIB_MEM_READ] TEXTO A LEER: %s con un tamanio de %d", ((char *) ptroLectura),read->memoria_size);

			if(rtaRead>=0){
				t_ram_read* estructuraRead = shared_crear_ram_read(read->memoria_size, ptroLectura);
				size_t* tamanioBuffer = malloc(sizeof(size_t));
				void* readSerializado = serializar_ram_read(estructuraRead, tamanioBuffer);
				loggear_info("[MATELIB_MEM_READ], proceso %d pudo leer el espacio seleccionado", read->pid);
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, *tamanioBuffer, readSerializado);

				//t_ram_read* asdasdasd = deserializar_ram_read(readSerializado);
				//loggear_warning("[MATELIB_MEM_READ] SE ENVIO: %d", *((int *) asdasdasd->mem_read));

				free(readSerializado);
				free(estructuraRead); // LO AGREGO ALAN PARA ELIMINAR VALGRINDS
				free(tamanioBuffer); // LO AGREGO ALAN PARA ELIMINAR VALGRINDS
			}else{
				loggear_error("[MATELIB_MEM_READ], proceso %d NO pudo leer el espacio seleccionado", read->pid);
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}

			//imprimir_frames2();
			//imprimir_tlb2();

			free(ptroLectura);
			free(read); // LO AGREGO ALAN PARA ELIMINAR VALGRINDS
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case MATELIB_MEM_WRITE:
			loggear_info("[MATELIB_MEM_WRITE], un proceso quiere escribir en la RAM");
			 t_matelib_memoria_write* escritura = deserializar_memoria_write(mensaje->payload);

			void * write_memoria_write = escritura->memoria_write;

			loggear_warning("[MATELIB_MEM_WRITE] TEXTO A ESCRIBIR: %s", ((char *) write_memoria_write));

			int32_t rtaWrite = memwrite(write_memoria_write, escritura->memoria_mate_pointer, escritura->pid, escritura->memoria_size);
			uint32_t headerW;

			if(rtaWrite>=0){
				loggear_info("[MATELIB_MEM_WRITE], proceso %d pudo escribir en el espacio seleccionado", escritura->pid);
				headerW = EXITO_EN_LA_TAREA;
			}else{
				loggear_info("[MATELIB_MEM_WRITE], proceso %d NO pudo escribir el espacio seleccionado", escritura->pid);
				headerW = FALLO_EN_LA_TAREA;

			}

			enviar_mensaje_protocolo(mensaje->socket, headerW, 0, NULL);

			//imprimir_frames2();
			//imprimir_tlb2();

			free(write_memoria_write);
			free(escritura);
			desconexion(mensaje);
			destruir_mensaje(mensaje);
			return 0;
		case SUSPENDER_PROCESO:
			loggear_info("[SUSPENDER_PROCESO], hay que cerrar un proceso");
			t_matelib_nuevo_proceso* PID_proceso_suspender = deserializar_crear_proceso(mensaje->payload);

			int32_t rtaSuspender = suspender_PID(PID_proceso_suspender->pid);
			uint32_t headerS;

			if(rtaSuspender){
				loggear_info("[SUSPENDER_PROCESO], proceso %d fue suspendido", PID_proceso_suspender->pid);
				headerS = EXITO_EN_LA_TAREA;
			}else{
				loggear_error("[SUSPENDER_PROCESO], proceso %d NO fue suspendido", PID_proceso_suspender->pid);
				headerS = FALLO_EN_LA_TAREA;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerS, 0, NULL);

			desconexion(mensaje);
			destruir_mensaje(mensaje);
			return 0;
		case PROCESO_EN_READY:
			loggear_info("[PROCESO_EN_READY], Se movio a ready el proceso");
			t_matelib_nuevo_proceso* PID_proceso_ready = deserializar_crear_proceso(mensaje->payload);

			int32_t rtaReady = PID_listo(PID_proceso_ready->pid);
			uint32_t headerR;

			if(rtaReady){
				loggear_info("[PROCESO_EN_READY], proceso %d fue puesto en ready", PID_proceso_ready->pid);
				headerR = EXITO_EN_LA_TAREA;
			}else{
				loggear_error("[PROCESO_EN_READY], proceso %d NO fue puesto ready", PID_proceso_ready->pid);
				headerR = FALLO_EN_LA_TAREA;
			}

			enviar_mensaje_protocolo(mensaje->socket, headerR, 0, NULL);

			free(PID_proceso_ready);
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

void imprimir_frames2() {

	for (int i = 0; i < list_size(listaFrames); i++) {
		t_frame* frame = (t_frame*)list_get(listaFrames, i);
		loggear_debug("FRAME:%d | ESTADO: %d | PROCESO: %d | PAGINA : %d", frame->nroFrame, frame->estado, frame->proceso, frame->pagina);

	}

}

void imprimir_tlb2(){

	loggear_trace("----------------------------------------------------------");
	loggear_trace("-----------------VOY A IMPRIMIR LA TLB--------------------");
	loggear_trace("----------------------------------------------------------");
	for(int i = 0; i < list_size(TLB); i++) {
	entrada_tlb* entrada = (entrada_tlb*) list_get(TLB,i);
	loggear_trace("ENTRADA: %d | KEY: %s | FRAME: %d | TIMESTAMP %d", i, entrada->hash_key, entrada->frame, entrada->timestamp);

}
	loggear_trace("----------------------------------------------------------");
	loggear_trace("-----------------------I'M DONE---------------------------");
	loggear_trace("----------------------------------------------------------");
}
