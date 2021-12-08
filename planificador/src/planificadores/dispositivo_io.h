#ifndef DISPOSITIVO_IO_H_
#define DISPOSITIVO_IO_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <commons/string.h>
#include <commons/collections/dictionary.h>

#include "colas.h"

#include "../../../shared/logger.h"

#include "../configuracion/configuracion_guardada.h"

/**
 * Funciona muy similar a como funciona `semaforo.h` con la diferencia que:
 * 1. Al llamarse a un dispositivo, el hilo que llama siempre se bloquea
 * 2. Cada dispositivo tiene un tiempo para desbloquear el hilo
 * 3. Apenas se bloquea, se inicia un pthread aparte, con un sleep para que al terminar desbloquee el hilo
 * */

typedef struct {
	char * nombre;
	int duracion;
	pthread_mutex_t mutex;
} t_io;

/**
 * @NAME: dispositivo_io_estructuras_crear
 * @DESC: Crea las estructuras de memoria necesarias para administrar los hilos y los dispositivos de IO
 * @RETURN: 0 si salio todos bien.
 * */
int dispositivo_io_estructuras_crear();

/**
 * @NAME: dispositivo_io_estructuras_destruir
 * @DESC: Vacia y destruye todas las estructuras de memoria que se usen para manejar los dispositivos de IO
 * @RETURN: 0 si salio todos bien.
 * */
int dispositivo_io_estructuras_destruir();

/**
 * @NAME: dispositivo_io_usar
 * @NOTE: Similar a semaforo_wait
 * @DESC: Avisa para bloquear el hilo que realizo la llamada al dispositivo
 * Al mismo tiempo, crea un pthread con un contador que al pasar los segundos de espera, avisa que hay que desbloquear a alguien
 * 1. No se esta usando y se llama. Bloquea el hilo, crea el pthread para desbloquearlo.
 * 2. Se esta usando por [N] hilos (en espera) y se llama. Bloquea el hilo, agrega [1] al contador del dispositivo
 * para que al terminar con el proceso inicie otra vez ya que el hilo ya estaba bloqueado
 * */
int dispositivo_io_usar(uint32_t pid, char * nombre);


#endif
