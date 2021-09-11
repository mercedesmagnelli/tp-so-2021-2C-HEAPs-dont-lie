
#ifndef CONEXION_PLANIFICADOR_COORDINAR_HILOS_H_
#define CONEXION_PLANIFICADOR_COORDINAR_HILOS_H_

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"

#include "manejar_mensajes_planificador.h"

sem_t semaforo_detener_ejecucion;

/**
 * @NAME: conectar_planificador
 * @DESC: Bloquea el hilo en donde se ejecute para escuchar conexiones del planificador y manejar mensajes.
 * Solo se va a detener cuando maten el hilo desde donde se invoca o el planificador se desconecte.
 */
int conectar_planificador();

#endif /* CONEXION_PLANIFICADOR_COORDINAR_HILOS_H_ */
