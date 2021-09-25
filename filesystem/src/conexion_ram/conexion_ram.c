#include "conexion_ram.h"

int iniciar_conexion_ram();

void avisar_ram_desconexion();

// Publica
int ram_enviar_handshake() {
	int socket_ram = iniciar_conexion_ram();
	if (socket_ram < 0) {
		return socket_ram;
	}

	int resultado = enviar_mensaje_protocolo(socket_ram, HANDSHAKE_F_R, 0, NULL);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al enviar el Handshake a ram, Error: %d", resultado);

		return resultado;
	}

	loggear_trace("Enviado handshake a la ram");

	int error = recibir_mensaje(socket_ram);
	if (error != 0) {
		loggear_info("La RAM nos desconoce, no podemos trabajar");
		return error;
	}

	pthread_exit(NULL);
	return 0;
}

// Publica
void ram_cerrar_conexiones(bool safe_close) {
	avisar_ram_desconexion();

	loggear_trace("Cerrado los threads y sockets");
}

void avisar_ram_desconexion() {
	int socket_avisar = iniciar_conexion_ram();

	enviar_mensaje_protocolo(socket_avisar, DESCONEXION_TOTAL, 0, NULL);

	close(socket_avisar);
}

int iniciar_conexion_ram() {
	char * ram_ip = get_ip();
	int ram_puerto = get_puerto();

	int socket_server = conectar_a_servidor(ram_ip, ram_puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor de ram, Ip: %s, Puerto: %d, Error: %d", ram_ip, 8082, socket_server);

		return socket_server;
	}

	return socket_server;
}
