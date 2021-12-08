
#ifndef PLANIFICADORES_H_
#define PLANIFICADORES_H_

#include <stddef.h>
#include <pthread.h>

#include "deadlocks.h"
#include "colas.h"
#include "estructuras.h"
#include "hilos_planificador.h"
#include "semaforo.h"
#include "dispositivo_io.h"
#include "../conexion_ram/conexion_ram.h"

#include "../../../shared/logger.h"

const float alfa; // Definido por archivo de configuracion para calcular estimaciones por HRRN.

/**
 * @NAME: suspender_hilo
 * @DESC: Luego de cada movimiento entre colas (algunas, no todas), 
 * analiza si se cumplen las condiciones para moverlo a suspendido y avisa a RAM
 * */
bool suspender_hilo();

/**
 * @NAME: planificadores_iniciar
 * @DESC: Inicia todos los semaforos, hilos y demas cosas necesarias para hacer funcionar el planificador
 * @RETURN: 0 si no hay error. Cualquier otro numero, ocurrio un error.
 * */
int planificadores_iniciar();

/**
 * @NAME: planificadores_destruir
 * @DESC: Destruye todos los recursos del planificador
 * */
void planificadores_destruir();

/**
 * @NAME: planificadores_proceso_iniciar
 * @DESC: Encola un nuevo proceso en el planificador de largo plazo como NEW
 * @RETURN: 0 si salio todos bien
 * */
int planificadores_proceso_iniciar(uint32_t pid);

/**
 * @NAME: planificadores_proceso_cerrar
 * @DESC: Elimina un proceso del sistema, moviendolo a FINISh
 * @RETURN: 0 si salio todos bien
 * */
int planificadores_proceso_cerrar(uint32_t ppid);

#endif /* PLANIFICADORES_H_ */
