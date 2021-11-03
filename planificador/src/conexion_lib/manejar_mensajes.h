
#ifndef CONEXION_PLANIFICADOR_MANEJAR_MENSAJES_H_
#define CONEXION_PLANIFICADOR_MANEJAR_MENSAJES_H_

#include <string.h>

#include "../planificadores/planificadores.h"
#include "../planificadores/semaforo.h"
#include "../planificadores/dispositivo_io.h"

#include "../conexion_ram/conexion_ram.h"

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/thread_safe_value.h"
#include "../../../shared/estructura_compartida.h"

/**
 * @NAME: lib_recibir_mensaje
 * @DESC: Espera un mensaje, y actua segun corresponda.
 * @RETURN:
 * Llama a "manejar_mensaje"
 */
int lib_recibir_mensaje(int socket);

/**
 * @NAME: manejar_mensaje
 * @DESC: Se le envia un mensaje recibido y ejecuta el código acorde.
 * @RETURN:
 *   0 salió bien
 * 	-1 desconexion
 *	-2 error al recibir
 *	-3 mensaje inentendible
 */
int manejar_mensaje(t_prot_mensaje * mensaje);

#endif /* CONEXION_PLANIFICADOR_MANEJAR_MENSAJES_H_ */
