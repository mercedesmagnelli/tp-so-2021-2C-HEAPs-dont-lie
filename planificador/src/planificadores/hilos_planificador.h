
#ifndef PLANIFICADORES_HILOS_PLANIFICADOR_H_
#define PLANIFICADORES_HILOS_PLANIFICADOR_H_

#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

#include "../configuracion/configuracion_guardada.h"

typedef struct {
	sem_t sem_ejecutar;
	bool finalizo;

	void * mensaje;
} t_hilos_semaforo;


/**
 * @NAME: hilos_planificador_iniciar
 * @DESC: Crea las estructuras administrativas para manejar los hilos internos del planificador
 */
int hilos_planificador_iniciar();

/**
 * @NAME: hilos_planificador_destruir
 * @DESC: DEstruye todas las estructuras administrativas de los hilos
 */
void hilos_planificador_destruir();

/**
 * @NAME: hilos_crear_hilo
 * @DESC: Crea un hilo nuevo y lo añade a un listado de hilos, para poder limpiarlos todos juntos de ser necesario
 */
pthread_t * hilos_crear_hilo();


/**
 * @NAME: hilos_agregar_nuevo_hilo
 * @DESC: Crea una estructura para manejar los mensajes entre protocolo y la ejecucion
 */
void hilos_agregar_nuevo_hilo(uint32_t pid);

/**
 * @NAME: hilos_post_ejecucion
 * @DESC: Hace un post en el semaforo del hilo para avisar que esta en ejecucion
 */
void hilos_post_ejecucion(uint32_t pid);

/**
 * @NAME: hilos_se_movio_finalizado
 * @DESC: Marca el hilo como que esta finalizado por DEADLOCK y hace el post para avisar que puede continuar ejecucion
 */
void hilos_se_movio_finalizado(uint32_t pid);

/**
 * @NAME: hilos_wait_ejecucion
 * @DESC: HAce un wait en el semaforo del hilo para esperar que entre en ejecucion
 */
void hilos_wait_ejecucion(uint32_t pid);

/**
 * @NAME: hilos_check_finalizo_proceso
 * @DESC: Hace un wait en el semaforo del hilo para esperar a que se pueda ejecutar y retorna si finalizo o no
 */
bool hilos_check_finalizo_proceso(uint32_t pid);

/**
 * @NAME: hilos_destruir_hilo_actual
 * @DESC: Destruye el hilo actual en ejecucion y lo elimina del listado, si sigue ejecutando despues de esta linea
 * es que algo salio mal.
 */
void hilos_destruir_hilo_actual();

/**
 * @NAME: hilos_check_continuar_ejecucion
 * @DESC: Retorna true si hay que destruir el hilo
 */
int hilos_check_continuar_ejecucion();


void hilos_post_new();
void hilos_wait_new();

void hilos_post_ready();
void hilos_wait_ready();
void hilos_post_multiprogramacion();
void hilos_wait_multiprogramacion();

void hilos_post_exec();
void hilos_wait_exec();
void hilos_post_multitarea();
void hilos_wait_multitarea();

void hilos_post_nuevo_bloqueado();
void hilos_wait_nuevo_bloqueado();

void hilos_post_finalizado();
void hilos_wait_finalizado();


#endif /* PLANIFICADORES_HILOS_PLANIFICADOR_H_ */
