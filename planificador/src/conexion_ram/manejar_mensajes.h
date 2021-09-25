
#ifndef CONEXION_RAM_MANEJAR_MENSAJES_H_
#define CONEXION_RAM_MANEJAR_MENSAJES_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/estructura_compartida.h"
#include "conexion_ram.h"

/**
 * @NAME: recibir_mensaje
 * @DESC: Espera un mensaje, y actua segun corresponda.
 * @RETURN:
 * Llama a "manejar_mensajes"
 */
int recibir_mensaje(int socket_ram);

/**
 * @NAME: manejar_mensajes
 * @DESC: Se le envia un mensaje recibido y ejecuta el código acorde.
 * @RETURN:
 *   0 salió bien
 * 	-1 desconexion
 *	-2 error al recibir
 *	-3 mensaje inentendible
 */
int manejar_mensajes(t_prot_mensaje * mensaje);

#endif /* CONEXION_RAM_MANEJAR_MENSAJES_H_ */
