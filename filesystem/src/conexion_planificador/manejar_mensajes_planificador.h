
#ifndef CONEXION_PLANIFICADOR_MANEJAR_MENSAJES_PLANIFICADOR_H_
#define CONEXION_PLANIFICADOR_MANEJAR_MENSAJES_PLANIFICADOR_H_

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/thread_safe_value.h"
#include "../../../shared/estructura_compartida.h"


/**
 * @NAME: recibir_mensaje_planificador
 * @DESC: Espera un mensaje del planificador, actua segun corresponda.
 * @RETURN:
 * Llama a "manejar_mensaje_planificador"
 */
int recibir_mensaje_planificador(int socket_planificador);

/**
 * @NAME: manejar_mensaje_planificador
 * @DESC: Se le envia un mensaje recibido del planificador y ejecuta el código acorde.
 * @RETURN:
 *   0 salió bien
 * 	-1 desconexion
 *	-2 error al recibir
 *	-3 mensaje inentendible
 */
int manejar_mensaje_planificador(t_prot_mensaje * mensaje);

#endif /* CONEXION_PLANIFICADOR_MANEJAR_MENSAJES_PLANIFICADOR_H_ */
