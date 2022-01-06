#ifndef MANEJO_HILOS_H_
#define MANEJO_HILOS_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "codigo_error.h"
#include "logger.h"

/**
 * @NAME: thread_join
 * @DESC: Espera que finalice el hilo y retorna su valor
 */
int thread_join_and_free(pthread_t thread_id);

/**
 * @NAME: thread_detach
 * @DESC: Cierra instantaneamente el hilo, liberando la memoria
 */
int thread_detach_and_free(pthread_t thread_id);

/**
 * @NAME: thread_ejecutar_funcion
 * @DESC: Ejecuta una funcion en un hilo y retorna el hilo donde se ejecuta
 */
pthread_t thread_ejecutar_funcion( int (*log_function)() );

#endif
