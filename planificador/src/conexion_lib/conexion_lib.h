
#ifndef CONEXION_PLANIFICADOR_CONEXION_PLANIFICADOR_H_
#define CONEXION_PLANIFICADOR_CONEXION_PLANIFICADOR_H_

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <commons/collections/queue.h>

#include "../configuracion/configuracion_guardada.h"

#include "../../../shared/logger.h"
#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/thread_safe_value.h"

#include "coordinar_hilos.h"
#include "manejar_mensajes.h"

/**
 * @NAME: levantar_servidor
 * @DESC: Crea un hilo para esperar la conexión del planificador y guardar un socket para notificarlo de sabotajes,
 * una vez llamada esta función, se puede ejecutar "enviar_sabotaje_planificador()" en cualquier momento y lugar que
 * importe este header.
 * Al guardar el socket correctamente, se auto-destruye el hilo.
 */
int levantar_servidor();

/**
 * @NAME: cerrar_conexiones
 * @DESC: Cierra todos los sockets y libera la memoria de las variables. Llamar siempre
 */
void lib_cerrar_conexiones(bool safe_close);

#endif /* CONEXION_PLANIFICADOR_CONEXION_PLANIFICADOR_H_ */
