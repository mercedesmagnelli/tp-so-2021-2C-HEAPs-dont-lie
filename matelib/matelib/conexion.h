
#ifndef CONEXION_H_
#define CONEXION_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "codigo_error.h"

/**
 * @NAME: conectar_a_servidor
 * @DESC: Dado un IP y un PUERTO, se comunica mediante sockets con el servidor.
 * @RETURN: El numero de socket.
 * @AFTER: Después de usar la función se debe hacer close() al retorno de esta funcion, para cerrar el socket.
 */
int conectar_a_servidor(const char* ip, const int puerto);

/**
 * @NAME: crear_servidor_y_escuchar
 * @DESC: Dado un PUERTO, ocupa el puerto.
 * @RETURN: El numero de socket, asociado al puerto.
 * @AFTER: Después de usar la función se debe hacer close() al retorno de esta funcion, para cerrar el socket.
 */
int crear_servidor(const int puerto);

#endif /* CONEXION_H_ */
