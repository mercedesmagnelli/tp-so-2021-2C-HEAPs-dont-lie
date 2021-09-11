
#ifndef CONEXION_RAM_MANEJAR_MENSAJES_RAM_H_
#define CONEXION_RAM_MANEJAR_MENSAJES_RAM_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/estructura_compartida.h"
#include "conexion_ram.h"

/**
 * @NAME: recibir_mensaje_ram
 * @DESC: Espera un mensaje de la ram, actua segun corresponda.
 * @RETURN:
 * Llama a "manejar_mensajes_ram"
 */
int recibir_mensaje_ram(int socket_ram);

/**
 * @NAME: manejar_mensajes_ram
 * @DESC: Se le envia un mensaje recibido de la memoria ram y ejecuta el código acorde.
 * @RETURN:
 *   0 salió bien
 * 	-1 desconexion
 *	-2 error al recibir
 *	-3 mensaje inentendible
 */
int manejar_mensajes_ram(t_prot_mensaje * mensaje);

#endif /* CONEXION_RAM_MANEJAR_MENSAJES_RAM_H_ */
