#ifndef COLAS_H_
#define COLAS_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#include <commons/collections/queue.h>
#include <commons/collections/list.h>

#include "../../../shared/logger.h"

#include "estructuras.h"
#include "algoritmos.h"
#include "hilos_planificador.h"

/**
 * Aca se guardaran todas las colas, listas y/o pilas que sea necesaria para mover entre hilos
 * NEW, READY, EXEC, FINISH, BLOCK, BLOCK-SUSP, BLOCK-READY
 * */

/**
 * @NAME: colas_iniciar
 * @DESC: Inicia las estructuras de memoria necesarias para administrar los procesos
 * @RETURN: 0 si salio todos bien
 * */
int colas_iniciar();

/**
 * @NAME: colas_destruir
 * @DESC: Vacia y destruye todas las estructuras de memoria
 * @RETURN: 0 si salio todos bien
 * */
int colas_destruir();

/**
 * @NAME: colas_insertar_new
 * @DESC: Recibe un hilo nuevo y lo agrega en la cola de NEW
 *
 * Necesita que le pasen el numero de proceso
 * */
t_hilo * colas_insertar_new(uint32_t pid);

/**
 * @NAME: colas_mover_new_ready
 * @DESC: Toma el primero en la cola de NEW y lo mueve a READY, siempre se hace en orden FIFO
 * ya que lo gestiona el planificador de LARGO PLAZO
 *
 * No necesita que le pasen quien se va a mover, porque funciona como FIFO
 * */
t_hilo * colas_mover_new_ready();

/**
 * @NAME: colas_mover_ready_exec
 * @DESC: Dependiendo el algoritmo, hay que recorrer la lista de hilos en READY y elegir cual mover a EXEC
 * */
t_hilo * colas_mover_ready_exec();

/**
 * @NAME: colas_mover_ready_finish
 * @DESC: Recibe un hilo y lo mueve de EXEC a FINISH
 *
 * */
t_hilo * colas_mover_exec_finish(uint32_t pid_mover);

/**
 * @NAME: colas_obtener_finalizado
 * @DESC: Elimina de la cola de finalizado el primer hilo que encuentre y lo retorna
 */
t_hilo * colas_obtener_finalizado();

/**
 * @NAME: colas_mover_exec_blocked
 * @DESC: Recibe un hilo y lo mueve de EXEC a BLOCKED
 * */
t_hilo * colas_mover_exec_block(t_dispositivo_bloqueante dispositivo_bloqueante, char * nombre_bloqueante, uint32_t pid);

/**
 * @NAME: colas_obtener_hilo_en_exec
 * @DESC: Retorna el hilo en exec
 */
t_hilo * colas_obtener_hilo_en_exec(uint32_t pid);

/**
 * @NAME: colas_agregar_wait_semaforo
 * @DESC: Agrega el semaforo que le pasen como un semaforo consumido por el proceso
 */
void colas_agregar_wait_semaforo(uint32_t pid, void * semaforo);

/**
 * @NAME: colas_mover_block_ready
 * @DESC: Mueve un hilo de block a ready cuando termina de estar bloqueado, segun la condicion que lo bloquea
 * @CASES: 
 *  1- La lista esta vacia, llega un hilo a bloqueado porque accede a un dispositivo de I/O, cuando pase el tiempo se desbloquea
 *  2- La lista tiene [N] hilos bloqueados por semaforo [A], llega 1 hilo bloqueado por semaforo [B]. Se hace sem_post de [B],
 *      el ultimo hilo en entrar se desbloquea primero.
 *  3- La lista esta bloqueada por un hilo 1 [IO-1], llega un hilo 2 bloqueado por [IO-2]. Termina el tiempo de [IO-2] sale el hilo 2
 *  4- La lista esta bloqueada por un hilo 1 [IO-1], llegan 2 hilos (2 y 3) esperando [IO-1], finaliza hilo 1 con [IO-1].
 *      Hilo 1 se desbloquea, hilo 2 [IO-1] queda bloqueado, esperanzo que termine su rafaga.
 *      Hilo 3 queda bloqueando esperando que [IO-1] pueda ser utilizado
 * */
t_hilo * colas_mover_block_ready(t_hilo * hilo_mover);

/**
 * @NAME: colas_mover_block_block_susp
 * @DESC: Cuando se cumplen ciertas condiciones, al mover un hilo a bloqueado. Verificia si no debe suspenderlo.
 * De ser asi, lo suspende, avisa a la RAM
 * */
t_hilo * colas_mover_block_block_susp();

/**
 * @NAME: colas_mover_block_susp_block_ready
 * @DESC: Mueve un hilo de BLOCKED SUSPENDED a BLOCKED READY.
 * */
t_hilo * colas_mover_block_susp_block_ready(t_hilo * hilo_mover);

/**
 * @NAME: colas_mover_block_ready_ready
 * @DESC: Mueve un hilo de BLOCKED READY a READY, se deberia hacer cuando el grado de multiprocesamiento lo permita.
 * */
t_hilo * colas_mover_block_ready_ready();

/**
 * @NAME: colas_bloquear_listas_bloqueados
 * @DESC: Bloquea el mutex de la lista de BLOQUEADO y SUSPENDIDO-BLOQUEADO
 */
void colas_bloquear_listas_bloqueados();

/**
 * @NAME: colas_desbloquear_listas_bloqueados
 * @DESC: Desbloquea el mutex de la lista de BLOQUEADO y SUSPENDIDO-BLOQUEADO
 */
void colas_desbloquear_listas_bloqueados();

/**
 * @NAME: colas_obtener_listas_bloqueados
 * @DESC: Retorna una lista con la concatenacion de las listas de BLOQUEADO y SUSPENDIDO-BLOQUEADO,
 * que estna bloqueados por el dispositivo que se le pase
 */
t_list * colas_obtener_listas_bloqueados(t_dispositivo_bloqueante dispositivo_bloqueante);

/**
 * @NAME: deberia_suspenderse_procesos
 * @DESC: Avisa el grado de multiprogramacion esta copado por procesos que no usan la CPU
 */
bool deberia_suspenderse_procesos();

/**
 * @NAME: hay_procesos_en_suspendido_ready
 * @DESC: Retorna true si hay proceso en SUSPENDIDO-READY
 */
bool hay_procesos_en_suspendido_ready();

/**
 * @NAME: colas_desbloquear_hilo_concreto
 * @DESC: Se fija en que lista esta el hilo y dependiendo de donde sea, lo mueve a READY o SUSP-READY
 */
t_hilo * colas_desbloquear_hilo_concreto(t_hilo * hilo_bloqueado);

/**
 * @NAME: colas_finalizar_proceso_bloqueado
 * @DESC: Si el proceso esta BLOQUEADO o BLOQUEADO-SUSPENDIDO lo mueve a finalizado y avisa a los semaforos
 * Caso contrario muestra error y retorna NULL
 */
t_hilo * colas_finalizar_proceso_bloqueado(t_hilo * hilo_bloqueado);

#endif
