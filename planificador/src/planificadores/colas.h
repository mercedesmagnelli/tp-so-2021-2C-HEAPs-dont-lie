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
 * @RETURN: 0 si salio todo bien
 * */
int colas_iniciar();

/**
 * @NAME: colas_destruir
 * @DESC: Vacia y destruye todas las estructuras de memoria
 * @RETURN: 0 si salio todo bien
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
 *
 * TODO: Se necesita una funcion de orden superior que retorne el que hay que elegir
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
// TODO: Igual que la funcion anterior, tenemos que quitar uno especifico, no siempre es en cola.
// TODO: Quizas conviene analizar la idea de una "cola especial fantasma" a donde van los hilos bloqueados, cuando ya deben salir
// para analizar si deben seguir bloqueados o salen.
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
 * @NAME: colas_desbloquear_1_hilo
 * @DESC: Primero revisa la lista de bloqueados si hay algun hilo bloqueado para desbloquear, lo mueve a ready y retorna.
 * SI no encuentra en bloqueados, revisa la lista de SUSPENDIDO-BLOQUEADO, lo mueve a SUSPENDIDO-LISTO y retorna;
 * SI no encuentra ninguno, retorna NULL;
 */
t_hilo * colas_desbloquear_1_hilo(t_dispositivo_bloqueante dispositivo_bloqueante, char * semaforo_nombre);

/**
 * @NAME: colas_desbloquear_todos_hilos
 * @DESC: Recorre las listas de BLOQUEADO y SUSPENDIDO-BLOQUEADO y mueve los hilos a donde corresponda
 */
void colas_desbloquear_todos_hilos(t_dispositivo_bloqueante dispositivo_bloqueante, char * semaforo_nombre);

#endif
