
#ifndef PLANIFICADORES_EJECUTAR_HILO_H_
#define PLANIFICADORES_EJECUTAR_HILO_H_

#include <pthread.h>
#include <stddef.h>

#include <unistd.h> // Eliminar cuando se borre el sleep

#include "estructuras.h"
#include "hilos_planificador.h"

#include "../../../shared/logger.h"

/**
 * @NAME: ejecutar_hilo_iniciar_ejecucion
 * @DESC: Se crea un pthread para ejecutar el hilo que entro en EJECUCION
 */
void ejecutar_hilo_iniciar_ejecucion(t_hilo * hilo);

#endif /* PLANIFICADORES_EJECUTAR_HILO_H_ */
