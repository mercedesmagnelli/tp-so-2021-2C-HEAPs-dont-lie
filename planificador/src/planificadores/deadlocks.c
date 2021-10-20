#include "deadlocks.h"

/**
 * TODOs:
 * 1. Crear estructuras para manejar los deadlocks
 * 2. Crear hilo que periodicamente revise deadlocks
 * 3. Comunicarse con el archivo de colas para saber que hilos pueden causar interbloqueo
 * 4. Crear funcion de orden superior para terminar con el proceso de mayor ID
 * */

int deadlocks_iniciar();

int deadlocks_destruir();

int deadlocks_ejecutar(int tiempo);
