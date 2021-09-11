
#ifndef CONEXION_FILESYSTEM_H_
#define CONEXION_FILESYSTEM_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../configuracion/configuracion.h"

#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/logger.h"
#include "../../../shared/estructura_compartida.h"

#include "manejar_mensajes_filesystem.h"

void filesystem_mock_pruebas_enviar(void * mensaje, size_t * size);

/**
 * @NAME: filesystem_enviar_handshake
 * @DESC: Envia un handshake a filesystem para verificar la conexion
 * @RETURN:
 * 	0 salio bien
 * 	X Error, no podemos conectar
 */
int filesystem_enviar_handshake();

/**
 * @NAME: filesystem_cerrar_conexiones
 * @DESC: Cierra los hilos y libera toda la memoria ocupada
 * @PARAM:
 * safe_close = true, hace pthread_join
 * safe_close = false, hace pthread_detach
 */
void filesystem_cerrar_conexiones(bool safe_close);


#endif /* CONEXION_FILESYSTEM_H_ */
