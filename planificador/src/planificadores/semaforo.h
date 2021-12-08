#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <pthread.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

#include "colas.h"

#include "../../../shared/estructura_compartida.h"
#include "../../../shared/logger.h"

typedef struct {
	char * nombre;
	int32_t valor;
	pthread_mutex_t mutex;
	t_list * list_procesos_retienen;
	t_list * list_procesos_bloqueados;
	int show;
} t_semaforo;

typedef enum {
	SEM_OK,
	SEM_BLOQUEAR,
	SEM_ERROR_YA_EXISTIA,
	SEM_ERROR_NO_EXISTE,
	SEM_ERROR_MENOR_CERO,
	SEM_PROCESO_FINALIZADO,
	SEM_ERROR
} t_estado_ejecucion;

/**
 * @NAME: semaforo_get
 * @DESC: Retorna el semaforo
 */
t_semaforo * semaforo_get(char * nombre_semaforo);

/**
 * @NAME: semaforo_estructuras_crear
 * @DESC: Crea todas las estructuras necesarias para manejar los semaforos del kernel.
 * @RETURN: 0 si salio todos bien.
 * */
int semaforo_estructuras_crear();

/**
 * @NAME: semaforo_estructuras_destruir
 * @DESC: Vacia y destruye todas las estructuras de memoria que se usen para manejar los semaforos del kernel.
 * @RETURN: 0 si salio todos bien.
 * */
int semaforo_estructuras_destruir();

/**
 * @NAME: semaforo_iniciar
 * @DESC: Crea un semaforo con el valor que se le pase por parametro.
 * */
t_estado_ejecucion semaforo_iniciar(t_matelib_semaforo * nuevo_semaforo);

/**
 * @NAME: semaforo_wait
 * @DESC: Reduce en 1 el contador del semaforo.
 *  Si el semaforo tiene un valor mayor a 1, no hace nada mas.
 *  Si el semaforo tiene un valor menor a 1, avisa para bloquear el hilo esperando al semaforo.
 * */
t_estado_ejecucion semaforo_wait(t_matelib_semaforo * sem);

/**
 * @NAME: semaforo_post
 * @DESC: Incrementa en 1 el contador del semaforo.
 * Ademas, avisa para desbloquear cualquier hilo en la cola de BLOCKED que esta esperando por el semaforo.
 * */
t_estado_ejecucion semaforo_post(t_matelib_semaforo * sem);

/**
 * @NAME: semaforo_deadlock_post
 * @DESC: INcrementa en 1 el contador del semaforo, y ademas desbloquea cualquier hilo que este bloqueado esperando por el semaforo
 */
void semaforo_deadlock_post(t_semaforo * semaforo);

/**
 * @NAME: semaforo_destruir
 * @DESC: Destruye un semaforo y avisa para liberar todos los que lo esten usando.
 * */
t_estado_ejecucion semaforo_destruir(t_matelib_semaforo * sem);

/**
 * @NAME: semaforo_eliminar_proceso
 * @DESC: Elimina el semaforo que puede bloquear o no al hilo y hace post a todos los recursos que tenga reservados
 */
void semaforo_eliminar_proceso(t_hilo * hilo);

#endif
