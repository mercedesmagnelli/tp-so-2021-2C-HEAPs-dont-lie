
#include "conexion_filesystem.h"

int filesystem_iniciar_conexion();

void filesystem_avisar_desconexion();

pthread_t thread_escuchar_filesystem_sabotajes;
int socket_filesystem_sabotajes;


// Publica
int filesystem_enviar_handshake() {
	int socket_filesystem = filesystem_iniciar_conexion();
	if (socket_filesystem < 0) {
		return socket_filesystem;
	}

	int resultado = enviar_mensaje_protocolo(socket_filesystem, HANDSHAKE_D_M, 0, NULL);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al enviar el Handshake a Filesystem, Error: %d", resultado);

		return resultado;
	}

	loggear_trace("Enviado handshake a Filesystem");

	int error = recibir_mensaje_filesystem(socket_filesystem);
	if (error != 0) {
		loggear_info("Filesystem nos desconoce, no podemos trabajar");
		return error;
	}

	pthread_exit(NULL);
	return 0;
}

// Publica
void filesystem_cerrar_conexiones(bool safe_close) {
	filesystem_avisar_desconexion();

	// TODO: Si no se hace el join por X tiempo hacer detach
	if (safe_close) {
		pthread_join(thread_escuchar_filesystem_sabotajes, NULL);
	} else {
		pthread_detach(thread_escuchar_filesystem_sabotajes);
	}

	close(socket_filesystem_sabotajes);

	loggear_trace("Cerrado los threads y sockets");
}

int filesystem_iniciar_conexion() {
	char * filesystem_ip = get_ip_filesystem();
	int filesystem_puerto = get_puerto_filesystem();

	int socket_server = conectar_a_servidor(filesystem_ip, filesystem_puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor de Filesystem, Ip: %s, Puerto: %d, Error: %d", filesystem_ip, 8081, socket_server);

		return socket_server;
	}

	return socket_server;
}

void filesystem_avisar_desconexion() {
	int socket_avisar = filesystem_iniciar_conexion();

	enviar_mensaje_protocolo(socket_avisar, DESCONEXION_PLANIFICADOR, 0, NULL);

	close(socket_avisar);
}
