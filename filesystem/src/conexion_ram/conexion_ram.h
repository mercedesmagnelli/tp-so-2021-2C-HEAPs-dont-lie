
#ifndef CONEXION_RAM_H_
#define CONEXION_RAM_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../configuracion/filesystem_configuracion_guardada.h"

#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/logger.h"

#include "manejar_mensajes.h"

/**
 * @NAME: ram_enviar_handshake
 * @DESC: Antes de empezar a trabajar con la ram, se debe llamar esta funcion y verificar que la ram nos acepta.
 * De ser asi podemos empezar a manipular tripulantes
 * @RETURN:
 * 	0 nos acepta
 * 	X Error, no nos acepta
 */
int ram_enviar_handshake();

/**
 * @NAME: ram_cerrar_conexiones
 * @DESC: Cierra los hilos y libera toda la memoria ocupada
 * @PARAM:
 * safe_close = true, hace pthread_join
 * safe_close = false, hace pthread_detach
 */
void ram_cerrar_conexiones(bool safe_close);

/**
 * @NAME: iniciar_conexion_ram
 * @DESC: Genera y devuelve un socket de la conexion con la ram
 */
int iniciar_conexion_ram();

#endif /* CONEXION_RAM_H_ */
