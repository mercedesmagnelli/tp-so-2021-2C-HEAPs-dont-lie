#include "conexiones.h"

int conexiones_iniciar();

void avisar_ram_desconexion();

// Publica
int enviar_mate_init(t_matelib_nuevo_proceso * nuevo_proceso) {
	int socket = conexiones_iniciar();
	if (socket < 0) {
		return socket;
	}

	// TODO: Cambiar este mensaje por un t_mensaje
	size_t * size = malloc(sizeof(size_t));
	void * mensaje = serializiar_crear_proceso(nuevo_proceso, size);


	int resultado = enviar_mensaje_protocolo(socket, MATELIB_INIT, *size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al realizar el MATELIB_INIT, Error: %d", resultado);

		return resultado;
	}
	free(size);

	loggear_trace("Enviado handshake al señor X");

	int error = recibir_mensaje(socket);
	if (error != 0) {
		loggear_info("Nos descnocimos, no podemos trabajar");
		return error;
	}

	close(socket);

	pthread_exit(NULL);
	return 0;
}

// Publica
void conexiones_cerrar_conexiones(bool safe_close) {
	avisar_ram_desconexion();

	loggear_trace("Cerrado los threads y sockets");
}

void avisar_ram_desconexion() {
	int socket_avisar = conexiones_iniciar();

	enviar_mensaje_protocolo(socket_avisar, DESCONEXION_TOTAL, 0, NULL);

	close(socket_avisar);
}

int conexiones_iniciar() {
	char * ram_ip = get_ip();
	int ram_puerto = get_puerto();

	int socket_server = conectar_a_servidor(ram_ip, ram_puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor de ram, Ip: %s, Puerto: %d, Error: %d", ram_ip, 8082, socket_server);

		return socket_server;
	}

	return socket_server;
}
