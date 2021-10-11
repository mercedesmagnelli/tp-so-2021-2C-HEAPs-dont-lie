#ifndef SEMAFORO_H_
#define SEMAFORO_H_

/**
 * Recibe los hilos y sus peticiones hacia los semaforos
 * Analiza si hay que bloquear o no al hilo que ejecuta un WAIT a un semaforo
 * Al hacer un POST a un hilo, avisa que hay que quitar un hilo de la cola de bloqueados
 * */

/**
 * @NAME: semaforo_estructuras_crear
 * @DESC: Crea todas las estructuras necesarias para manejar los semaforos del kernel.
 * @RETURN: 0 si salio todo bien.
 * */
int semaforo_estructuras_crear();

/**
 * @NAME: semaforo_estructuras_destruir
 * @DESC: Vacia y destruye todas las estructuras de memoria que se usen para manejar los semaforos del kernel.
 * @RETURN: 0 si salio todo bien.
 * */
int semaforo_estructuras_destruir();

/**
 * @NAME: semaforo_iniciar
 * @DESC: Crea un semaforo con el valor que se le pase por parametro.
 * */
int semaforo_iniciar(void * proceso, void * semaforo);

/**
 * @NAME: semaforo_wait
 * @DESC: Reduce en 1 el contador del semaforo.
 *  Si el semaforo tiene un valor mayor a 1, no hace nada mas.
 *  Si el semaforo tiene un valor menor a 1, avisa para bloquear el hilo esperando al semaforo.
 * */
int semaforo_wait(void * proceso, void * semaforo);

/**
 * @NAME: semaforo_post
 * @DESC: Incrementa en 1 el contador del semaforo.
 * Ademas, avisa para desbloquear cualquier hilo en la cola de BLOCKED que esta esperando por el semaforo.
 * */
int semaforo_post(void * proceso, void * semaforo);


/**
 * @NAME: semaforo_destruir
 * @DESC: Destruye un semaforo y avisa para liberar todos los que lo esten usando.
 * */
int semaforo_destruir(void * proceso, void * semaforo);

#endif
