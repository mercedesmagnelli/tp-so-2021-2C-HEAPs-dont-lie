#include "conexiones.h"

int conexiones_iniciar();

void avisar_desconexion();

// Publica
int enviar_mate_init(t_matelib_nuevo_proceso * nuevo_proceso) {
	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}


	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializiar_crear_proceso(nuevo_proceso, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_INIT, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_INIT, Error: %d", resultado);

		return resultado;
	}

	int error = 0;
	t_prot_mensaje * mensaje_respuesta = recibir_mensaje_protocolo(socket);
	if (mensaje_respuesta->head == EXITO_EN_LA_TAREA) {
		loggear_info("El proceso [PID: %zu] se creo exitosamente", nuevo_proceso->pid);
	} else if (mensaje_respuesta->head == FALLO_EN_LA_TAREA) {
		loggear_error("El proceso [PID: %zu] no se pudo crear", nuevo_proceso->pid);
		error = 1;
	} else if (mensaje_respuesta->head == DESCONEXION) {
		// A VECES, llega antes la desconexion del receptor que la propia
	} else {
		loggear_warning("El proceso [PID: %zu] devolvio un codigo extraño: %d al hacer MATE_INIT", nuevo_proceso->pid, mensaje_respuesta->head);
		error = 2;
	}

	free(size);
	close(socket);

	return error;
}

int enviar_mate_close(t_matelib_nuevo_proceso * nuevo_proceso) {
	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	// TODO: Cambiar este mensaje por un t_mensaje
	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializiar_crear_proceso(nuevo_proceso, size);


	int resultado = enviar_mensaje_protocolo(socket, MATELIB_CLOSE, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_CLOSE, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;
}

int enviar_mate_sem_init(t_matelib_semaforo* nuevo_semaforo){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_semaforo(nuevo_semaforo, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_SEM_INIT, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_SEM_INIT, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;

}

int enviar_mate_sem_wait(t_matelib_semaforo* semaforo){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_semaforo(semaforo, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_SEM_WAIT, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_SEM_WAIT, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;
}

int enviar_mate_sem_post(t_matelib_semaforo* semaforo){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_semaforo(semaforo, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_SEM_POST, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_SEM_POST, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;
}

int enviar_mate_sem_destroy(t_matelib_semaforo* semaforo){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_semaforo(semaforo, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_SEM_DESTROY, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_SEM_DESTROY, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;
}

int enviar_mate_call_io(t_matelib_io* entrada_salida){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_io(entrada_salida, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_CALL_IO, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_CALL_IO, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;
}

int32_t enviar_mate_memalloc(t_matelib_memoria_alloc* alloc){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_memoria_alloc(alloc, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_MEM_ALLOC, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_MEM_ALLOC, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int32_t error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return error;
}

int enviar_mate_memfree(t_matelib_memoria_free* liberar){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_memoria_free(liberar, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_MEM_FREE, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_MEM_FREE, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;

}

int enviar_mate_memread(t_matelib_memoria_read* leer){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_memoria_read(leer, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_MEM_READ, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_MEM_READ, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;
}


int enviar_mate_memwrite(t_matelib_memoria_write* escribir){

	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializar_memoria_write(escribir, size);

	int resultado = enviar_mensaje_protocolo(socket, MATELIB_MEM_WRITE, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_MEM_WRITE, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	return 0;
}

// Publica
void conexiones_cerrar_conexiones(bool safe_close) {
	avisar_desconexion();

	loggear_trace("Cerrado los threads y sockets");
}

void avisar_desconexion() {
	int socket_avisar = conexiones_iniciar();

	enviar_mensaje_protocolo(socket_avisar, DESCONEXION_TOTAL, 0, NULL);

	close(socket_avisar);
}

int conexiones_iniciar() {
	char * ip = get_ip();
	int puerto = get_puerto();

	int socket_server = conectar_a_servidor(ip, puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor, Ip: %s, Puerto: %d, Error: %d", ip, puerto, socket_server);

		return socket_server;
	}

	return socket_server;
}
