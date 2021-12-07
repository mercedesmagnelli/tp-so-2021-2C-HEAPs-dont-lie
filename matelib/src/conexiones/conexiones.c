#include "conexiones.h"

void avisar_desconexion();

// Publica
int enviar_mate_init(t_instance_metadata* instancia, t_matelib_nuevo_proceso * nuevo_proceso) {
	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_INIT] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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
	t_prot_mensaje* mensaje_respuesta = recibir_mensaje_protocolo(socket);
	if (mensaje_respuesta->head == EXITO_EN_LA_TAREA) {
		loggear_info("El proceso [PID: %zu] se creó exitosamente", nuevo_proceso->pid);
	}else if(mensaje_respuesta->head == FALLO_EN_LA_TAREA){
		loggear_info("El proceso [PID: %zu] no se pudo crear", nuevo_proceso->pid);
		error = 1;
	}else {
		loggear_warning("El proceso [PID: %zu] devolvió un código extraño: %d al hacer MATE_INIT", nuevo_proceso->pid, mensaje_respuesta->head);
		error =2;
	}
	free(size);
	close(socket);

	return error;
}

int enviar_mate_close(t_instance_metadata* instancia, t_matelib_nuevo_proceso * nuevo_proceso) {
	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_CLOSE] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
		return socket;
	}

	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializiar_crear_proceso(nuevo_proceso, size);


	int resultado = enviar_mensaje_protocolo(socket, MATELIB_CLOSE, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_CLOSE, Error: %d", resultado);

		return resultado;
	}
	free(size);

	int error = 0;
	t_prot_mensaje* mensaje_respuesta = recibir_mensaje_protocolo(socket);
	if (mensaje_respuesta->head == EXITO_EN_LA_TAREA) {
		loggear_info("El proceso [PID: %zu] terminó exitosamente", nuevo_proceso->pid);
	}else if(mensaje_respuesta->head == FALLO_EN_LA_TAREA){
		loggear_info("El proceso [PID: %zu] no se pudo terminar", nuevo_proceso->pid);
		error = 1;
	}else {
		loggear_warning("El proceso [PID: %zu] devolvió un código extraño: %d al hacer MATE_CLOSE", nuevo_proceso->pid, mensaje_respuesta->head);
		error = 2;
	}

	close(socket);

	return error;
}

int enviar_mate_sem_init(t_instance_metadata* instancia, t_matelib_semaforo* nuevo_semaforo){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_SEM_INIT] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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


	int error = 0;
	t_prot_mensaje* mensaje_respuesta = recibir_mensaje_protocolo(socket);
	if (mensaje_respuesta->head == EXITO_EN_LA_TAREA) {
		loggear_info("El proceso [PID: %zu] creó el semáforo %s con valor inicial %d", nuevo_semaforo->pid, nuevo_semaforo->semaforo_nombre, nuevo_semaforo->semaforo_valor);
	}else if(mensaje_respuesta->head == FALLO_EN_LA_TAREA){
		loggear_info("El proceso [PID: %zu] no pudo crear el semáforo %s", nuevo_semaforo->pid, nuevo_semaforo->semaforo_nombre);
		error = 1;
	}else {
		loggear_warning("El proceso [PID: %zu] devolvió un código extraño: %d al hacer MATE_SEM_INIT", nuevo_semaforo->pid, mensaje_respuesta->head);
		error = 2;
	}

	close(socket);

	return error;

}

int enviar_mate_sem_wait(t_instance_metadata* instancia, t_matelib_semaforo* semaforo){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_SEM_WAIT] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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

	int error = 0;
	t_prot_mensaje* mensaje_respuesta = recibir_mensaje_protocolo(socket);
	if (mensaje_respuesta->head == EXITO_EN_LA_TAREA) {
		loggear_info("El proceso [PID: %zu] hizo wait al semáforo %s", semaforo->pid, semaforo->semaforo_nombre);
	} else if(mensaje_respuesta->head == FALLO_EN_LA_TAREA){
		loggear_info("El proceso [PID: %zu] no pudo hacer wait al semáforo %s", semaforo->pid, semaforo->semaforo_nombre);
		error = 1;
	} else if (mensaje_respuesta->head == EXITO_PROCESO_ELIMINADO) {
		loggear_info("El proceso [PID: %zu] fue desalojado por un deadlock", semaforo->pid);
		error = EXITO_PROCESO_ELIMINADO;
	} else if (mensaje_respuesta->head == DESCONEXION) {
		loggear_debug("Desconeccion [PID: %zu]", semaforo->pid);
	} else {
		loggear_warning("El proceso [PID: %zu] devolvió un código extraño: %d al hacer MATE_SEM_WAIT", semaforo->pid, mensaje_respuesta->head);
		error = 2;
	}

	close(socket);

	return error;
}

int enviar_mate_sem_post(t_instance_metadata* instancia, t_matelib_semaforo* semaforo){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_SEM_POST] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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

	int error = 0;
	t_prot_mensaje* mensaje_respuesta = recibir_mensaje_protocolo(socket);
	if (mensaje_respuesta->head == EXITO_EN_LA_TAREA) {
		loggear_info("El proceso [PID: %zu] hizo post al semáforo %s", semaforo->pid, semaforo->semaforo_nombre);
	}else if(mensaje_respuesta->head == FALLO_EN_LA_TAREA){
		loggear_info("El proceso [PID: %zu] no pudo hacer post al semáforo %s", semaforo->pid, semaforo->semaforo_nombre);
		error = 1;
	}else {
		loggear_warning("El proceso [PID: %zu] devolvió un código extraño: %d al hacer MATE_SEM_POST", semaforo->pid, mensaje_respuesta->head);
		error = 2;
	}

	close(socket);

	return error;
}

int enviar_mate_sem_destroy(t_instance_metadata* instancia, t_matelib_semaforo* semaforo){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_SEM_DESTROY] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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

	int error = 0;
	t_prot_mensaje* mensaje_respuesta = recibir_mensaje_protocolo(socket);
	if (mensaje_respuesta->head == EXITO_EN_LA_TAREA) {
		loggear_info("El proceso [PID: %zu] hizo destroy al semáforo %s", semaforo->pid, semaforo->semaforo_nombre);
	}else if(mensaje_respuesta->head == FALLO_EN_LA_TAREA){
		loggear_info("El proceso [PID: %zu] no pudo hacer destroy al semáforo %s", semaforo->pid, semaforo->semaforo_nombre);
		error = 1;
	}else {
		loggear_warning("El proceso [PID: %zu] devolvió un código extraño: %d al hacer MATE_SEM_DESTROY", semaforo->pid, mensaje_respuesta->head);
		error = 2;
	}

	close(socket);

	return error;
}

int enviar_mate_call_io(t_instance_metadata* instancia, t_matelib_io* entrada_salida){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_CALL_IO] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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

	int error = 0;
	t_prot_mensaje* mensaje_respuesta = recibir_mensaje_protocolo(socket);
	if (mensaje_respuesta->head == EXITO_EN_LA_TAREA) {
		loggear_info("El proceso [PID: %zu] llamó al recurso %s", entrada_salida->pid, entrada_salida->io_nombre);
	}else if(mensaje_respuesta->head == FALLO_EN_LA_TAREA){
		loggear_info("El proceso [PID: %zu] no pudo llamar al recurso %s", entrada_salida->pid, entrada_salida->io_nombre);
		error = 1;
	}else {
		loggear_warning("El proceso [PID: %zu] devolvió un código extraño: %d al hacer MATE_CALL_IO", entrada_salida->pid, mensaje_respuesta->head);
		error = 2;
	}

	close(socket);

	return error;
}

int32_t enviar_mate_memalloc(t_instance_metadata* instancia, t_matelib_memoria_alloc* alloc){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_MEM_ALLOC] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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

	t_prot_mensaje * mensajito = recibir_mensaje_protocolo(socket);
	t_matelib_nuevo_proceso * el_pipi = deserializar_crear_proceso(mensajito->payload);
	destruir_mensaje(mensajito);
	uint32_t pasamanos_horrible = el_pipi->pid;
	free(el_pipi);

//	if (error != 0) {
//		loggear_info("Nos descnocimos, no podemos trabajar");
//		return error;
//	}

	close(socket);

	return pasamanos_horrible;
}

int enviar_mate_memfree(t_instance_metadata* instancia, t_matelib_memoria_free* liberar){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_MEM_FREE] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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

int enviar_mate_memread(t_instance_metadata* instancia, t_matelib_memoria_read* leer){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_MEM_READ] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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


int enviar_mate_memwrite(t_instance_metadata* instancia, t_matelib_memoria_write* escribir){

	int socket = conexiones_iniciar(instancia);
	if (socket < 0) {
		loggear_error("[MATE_MEM_WRITE] [PID: %zu] [IP: %s] [PUERTO: %d] Error al conectar al servidor", instancia->pid, instancia->ip, instancia->port);
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
/*
// Publica
void conexiones_cerrar_conexiones() {
	avisar_desconexion();

	loggear_trace("Cerrado los threads y sockets");
}

void avisar_desconexion() {
	int socket_avisar = conexiones_iniciar();

	enviar_mensaje_protocolo(socket_avisar, DESCONEXION_TOTAL, 0, NULL);

	close(socket_avisar);
}
*/
int conexiones_iniciar(t_instance_metadata* instancia) {
	char * ip = instancia->ip;
	int puerto = instancia->port;

	int socket_server = conectar_a_servidor(ip, puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor, Ip: %s, Puerto: %d, Error: %d", ip, puerto, socket_server);

		return socket_server;
	}

	return socket_server;
}
