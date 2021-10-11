#ifndef DEADLOCKS_H_
#define DEADLOCKS_H_

#include "semaforo.h"

/**
 * Se encarga de leer el estado de los semaforos y revisar si ocurre algun deadlock
 * en caso de que exista, aplicar alguna politica para desbloquearlo
 * */

/**
 * @NAME: deadlocks_iniciar
 * @DESC: Crea todas las estructuras de memoria necesarias para controlar la existencia de deadlocks
 * */
int deadlocks_iniciar();

/**
 * @NAME: deadlocks_destruir
 * @DESC: Vacia y destruye todas las estructuras de memoria que se usaron para controlar la existencia de deadlocks
 * Tambien destruye los hilos en ejecucion que busquen deadlocks
 * */
int deadlocks_destruir();

/**
 * @NAME: deadlocks_ejecutar
 * @DESC: Crea un pthread, que cada [tiempo] que pasa, revisa si existe algun deadlock. 
 * De ser asi, aplica un algorimo de deteccion y recuperacion donde se termina 1 a 1 los hilos iniciando por el de mayor ID
 * */
int deadlocks_ejecutar(int tiempo);

#endif
