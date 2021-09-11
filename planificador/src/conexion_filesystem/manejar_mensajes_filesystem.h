
#ifndef CONEXION_FILESYSTEM_MANEJAR_MENSAJES_FILESYSTEM_H_
#define CONEXION_FILESYSTEM_MANEJAR_MENSAJES_FILESYSTEM_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/estructura_compartida.h"

#include "conexion_filesystem.h"

/**
 * @NAME: recibir_mensaje_filesystem
 * @DESC: Espera un mensaje de filesystem, actua segun corresponda.
 * @RETURN:
 * Llama a "manejar_mensajes_filesystem"
 */
int recibir_mensaje_filesystem(int socket_filesystem);

/**
 * @NAME: manejar_mensajes_filesystem
 * @DESC: Se le envia un mensaje recibido de la memoria ram y ejecuta el código acorde.
 * @RETURN:
 *   0 salió bien
 * 	-1 desconexion
 *	-2 error al recibir
 *	-3 mensaje inentendible
 */
int manejar_mensajes_filesystem(t_prot_mensaje * mensaje);

/**
 * @NAME: iniciar_conexion_ram
 * @DESC: Genera y devuelve un socket de la conexion con la ram
 */
int filesystem_iniciar_conexion();

#endif /* CONEXION_FILESYSTEM_MANEJAR_MENSAJES_FILESYSTEM_H_ */
