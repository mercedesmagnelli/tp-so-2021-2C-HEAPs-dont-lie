/*
 * conexion.c
 *
 *  Created on: 7 may. 2021
 *      Author: utnso
 */

#include "conexion.h"

const int SIN_FAMILY = AF_INET;

int crear_socket();

struct sockaddr_in crear_sockaddr(const char * ip, const int puerto);
struct sockaddr_in crear_sockaddr_any_ip(const int puerto);

int crear_conexion(int socket_cliente, struct sockaddr_in direccion_servidor);

// Pública
int conectar_a_servidor(const char* ip, const int puerto) {
	// Abrimos el socket, si no podemos retornamos el error.
	int socket_cliente = crear_socket();
	if (socket_cliente < 0){
		return socket_cliente;
	}

	// Creamos la estructura burocratica para establecer la conexión.
	struct sockaddr_in direccion_servidor = crear_sockaddr(ip, puerto);

	// Nos conectamos con el servidor, si no podemos retornamos el error.
	int conexion = crear_conexion(socket_cliente, direccion_servidor);

	if (conexion < 0){
		return conexion;
	}

	return socket_cliente;
}

// Pública
int crear_servidor(const int puerto) {
	// Abrimos el socket, si no podemos retornamos el error.
	int socket_escuchar = crear_socket();
	if (socket_escuchar < 0){
		return socket_escuchar;
	}

	// Creamos la estructura burocratica para establecer la conexión.
	struct sockaddr_in direccion_servidor = crear_sockaddr_any_ip(puerto);

	// Permite que los sockets sean reutilizados sin importar si se cerraron bien o no (acelera las pruebas)
	const int activado = 1;
	setsockopt(socket_escuchar, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	// Asociamos el socket con la direccion para escuchar mensajes
	int asociacion = bind(socket_escuchar, (void*) &direccion_servidor, sizeof(direccion_servidor));

	if (asociacion != 0) {
		close(socket_escuchar);
		return ERROR_AL_BINDEAR_EL_SERVIDOR;
	}

	// Habilitamos que se puedan conectar clientes
	int esuchar_socket = listen(socket_escuchar, SOMAXCONN);
	if (esuchar_socket < 0) {
		close(socket_escuchar);
		return ERROR_AL_ESCUCHAR_EL_SOCKET;
	}

	return socket_escuchar;
}

int crear_socket() {
	int socket_cliente = socket(SIN_FAMILY, SOCK_STREAM, 0);
	if (socket_cliente < 0){
		close(socket_cliente);
		// TODO: Print error
		return ERROR_AL_ABRIR_SOCKET;
	}

	return socket_cliente;
}

struct sockaddr_in crear_sockaddr(const char * ip, const int puerto) {
	struct sockaddr_in direccion_servidor;

	direccion_servidor.sin_family = SIN_FAMILY;
	direccion_servidor.sin_addr.s_addr = inet_addr(ip);
	direccion_servidor.sin_port = htons(puerto);

	return direccion_servidor;
}

struct sockaddr_in crear_sockaddr_any_ip(const int puerto) {
	struct sockaddr_in direccion_servidor;

	direccion_servidor.sin_family = SIN_FAMILY;
	direccion_servidor.sin_addr.s_addr = INADDR_ANY; // No queremos un ip en particular, queremos un lugar en el sistema
	direccion_servidor.sin_port = htons(puerto);

	return direccion_servidor;
}


int crear_conexion(int socket_cliente, struct sockaddr_in direccion_servidor) {
	int conexion = connect(socket_cliente, (void*) &direccion_servidor, sizeof(direccion_servidor));

	if (conexion < 0){
		close(socket_cliente);
		return ERROR_AL_CONECTAR_AL_SERVIDOR;
	}

	return conexion;
}
