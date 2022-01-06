#ifndef THREAD_SAFE_VALUE_H_
#define THREAD_SAFE_VALUE_H_

#include <pthread.h>
#include <stdlib.h>
#include "logger.h"

typedef struct {
	pthread_mutex_t mutex_t;
	void * value;
} t_safe_value;

/**
 * @NAME: crear_value_safe
 * @DESC: Retorna un valor y su estructura asociada para que sea thread_safe
 */
t_safe_value * crear_value_safe(void * value);

/**
 * @NAME: crear_value_safe_int
 * @DESC: Wrapper para crear_value_safe para int
 */
t_safe_value * crear_value_safe_int(int new_value);

/**
 * @NAME: crear_value_safe
 * @DESC: Cambia el valor del campo de manera thread_safe
 */
void set_value_safe(t_safe_value * safe_value, void * value);

/**
 * @NAME: set_value_safe_int
 * @DESC: Wrapper para settear ints
 */
void set_value_safe_int(t_safe_value * safe_value, int value);

/**
 * @NAME: get_value_safe
 * @DESC: Retorna el valor de manera thread_safe. Hay que castearlo al obtenerlo.
 * @Ejemplo: Al obtener el valor hay que castearlo
 * *((bool *) get_value_safe(mutex_usado))
 */
void * get_value_safe(t_safe_value * safe_value);

/**
 * @NAME: get_value_safe_int
 * @DESC: Wrapper para obtener ints
 */
int get_value_safe_int(t_safe_value * safe_value);

/**
 * @NAME: destroy_value_safe
 * @DESC: Elimina de la memoria el mutex
 */
void destroy_value_safe(t_safe_value * safe_value);

#endif /* THREAD_SAFE_VALUE_H_ */
