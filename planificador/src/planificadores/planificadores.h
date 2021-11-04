
#ifndef PLANIFICADORES_H_
#define PLANIFICADORES_H_

#include <stddef.h>
#include <pthread.h>

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
 * @RETURN: 0 si salio todo bien
 * */
int planificadores_proceso_iniciar(uint32_t pid);

/**
 * @NAME: planificadores_proceso_cerrar
 * @DESC: Elimina un proceso del sistema, moviendolo a FINISh
 * @RETURN: 0 si salio todo bien
 * */
int planificadores_proceso_cerrar(uint32_t ppid);

/**
 * @NAME: planificadores_semaforos_iniciar
 * @DESC: Crea un semaforo con el valor que se le pase
 * @RETURN: 0 si salio todo bien
 * */
int planificadores_semaforos_iniciar(void * proceso, void * semaforo);

/**
 * @NAME: planificadores_semaforos_wait
 * @DESC: Reduce en 1 el contador del semaforo para que lo tome el proceso.
 * Si el semaforo queda en un valor inferior a 1, bloquea el proceso.
 * Si al bloquear el proceso se cumplen ciertas condiciones, tambien suspende al proceso.
 * @RETURN: 0 si salio todo bien y el proceso se bloqueo correctamente
 * */
int planificadores_semaforos_wait(void * proceso, void * semaforo);

/**
 * @NAME: planificadores_semaforos_post
 * @DESC: Incrementa en 1 el contador del semaforo y si hay algun proceso bloqueado por ese semaforo, lo desbloquea.
 * @RETURN: 0 si salio todo bien y se incremento correctamente (no espera a desbloquear para retornar)
 * */
int planificadores_semaforos_post(void * proceso, void * semaforo);

/**
 * @NAME: planificadores_semaforos_destruir
 * @DESC: Destruye un semaforo del Kernel y desbloquea todos los procesos que estan bloqueados por el mismo
 * (TODO: Preguntar si esto ultimo es correcto, o deberiamos de cancelar hasta que todos los procesos esten libres)
 * @RETURN: 0 cuando se hizo todo bien y el sistema quedo en un estado estable
 * */
int planificadores_semaforos_destruir(void * proceso, void * semaforo);

/**
 * @NAME: planificadores_io_llamar
 * @DESC: Hace que un proceso llame a un recurso de IO por cierta cantidad de tiempo, al mismo tiempo lo bloquea.
 * Si el dispositivo ya esta siendo usado por otro, lo bloquea en espera a que termine el que ya estaba.
 * @RETURN: 0 Cuando el proceso se bloquea
 * */
int planificadores_io_llamar(void * proceso, void * dispositivo_io);

/**
 * @NAME: planificadores_memoria_alloc
 * @DESC: Si el proceso esta en ejecucion, envia un mensaje a la memoria para pedirle que reserve memoria.
 * Espera el mensaje con el puntero de la memoria.
 * @RETURN: Retorna el puntero que envie la memoria
 * */
void * planificadores_memoria_alloc(void * proceso, void * llamada_memoria);

/**
 * @NAME: planificadores_memoria_free
 * @DESC: Si el proceso esta en ejecucion, envia un mensaje a la memoria para pedirle que libere la memoria.
 * Espera que la memoria retorne un mensaje de exito o de fallo
 * @RETURN: Retoran lo que retorne la memoria
 * */
int planificadores_memoria_free(void * proceso, void * llamada_memoria);

/**
 * @NAME: planificadores_memoria_read
 * @DESC: Si el proceso esta en ejecucion, envia un mensaje a la memoria para pedirle leer una porcion de memoria.
 * Espera a que la memoria retorne un mensaje con el contenido de la memoria o un fallo.
 * @RETURN: Retorna lo que la memoria devolvio, puede ser un mensaje de exito con el void* del contenido o un mensaje de fallo
 * */
void * planificadores_memoria_read(void * proceso, void * llamada_memoria);

/**
 * @NAME: planificadores_memoria_write
 * @DESC: Si el proceso esta en ejecucion, envia un mensaje a la memoria para pedirle escribir un texto en una porcion de memoria.
 * Espera a que la memoria retorne un mensaje con el contenido de la memoria o un fallo.
 * @RETURN: 0 cuando todo se ejecuto exitosamente.
 * */
int planificadores_memoria_write(void * proceso, void * llamada_memoria);


#endif /* PLANIFICADORES_H_ */
