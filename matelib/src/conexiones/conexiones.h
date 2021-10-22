
#ifndef CONEXION_RAM_H_
#define CONEXION_RAM_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../configuracion/matelib_configuracion_guardada.h"

#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/logger.h"

#include "../conexiones/manejar_mensajes.h"

/**
 * @NAME: enviar_mate_init
 * @DESC: Envia el nuevo proceso a la memoria o al KErnel
 */
int enviar_mate_init(t_matelib_nuevo_proceso * nuevo_proceso);

/**
 * @NAME: conexiones_cerrar_conexiones
 * @DESC: Cierra los hilos y libera toda la memoria ocupada
 */
void conexiones_cerrar_conexiones(bool safe_close);

/**
 * @NAME: conexiones_iniciar
 * @DESC: Genera y devuelve un socket de la conexion con la ram
 */
int conexiones_iniciar();

#endif /* CONEXION_RAM_H_ */
