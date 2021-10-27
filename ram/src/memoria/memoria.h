#ifndef MEMORIA_MEMORIA_C_
#define MEMORIA_MEMORIA_C_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "../paginacion/paginacion.h"


/*Funciones que pide el TP obligatorias*/


/**
 * @NAME: memalloc
 * @DESC: Asigna un espacio dentro de la memoria principal. Si tiene que generar un espacio, lo hace al final de la
 * lista de direcciones. Si usa un espacio entre lugares ocupados y sobra, genera dos divisiones: la ocupada y la del espacio estante.
 * @RET:
 *   >=0 puntero lógico dirigido al espacio de memoria solicitado
 *   -41  se solicito un valor erroneo de memoria (negativo o 0)
 *   -42  el proceso no tiene mas espacio del cual solicitar (Asig Fija)
 *   -43  No hay mas espacio de memoria del cual solicitar (Asig Global)
 **/

int32_t memalloc(uint32_t pid, uint32_t size);

/**
 * @NAME: memfree
 * @DESC: Libera espacios de memoria. Se encarga de consolidar en caso de que queden dos espacios contiguos libres
 * y de liberar paginas en caso de que con la liberación una página quede vacía.
  * @RET:
 *   -5, hubo un error en la lectura
 *   0, no hay error pipi
 */

int32_t memfree(uint32_t direccionLogicaALiberar, uint32_t pid);

/**
 * @NAME: memread
 * @DESC: A partir de una direccion lógica a leer, se retorna lo que está guardado en esa posición.
 * @RET: -6, error al leer
 * 		  0, esta to' bien
 **/


void* memread(uint32_t direccionLogicaALeer, uint32_t pid, uint32_t size);

/**
 * @NAME: memwrite
 * @DESC: Se encarga de escribir informacion en memoria a partir de una direccion lógica
  * @RET:
 *  -7, hubo algun tipo de error
 *   0, no hay error de ningun tipo
 *
 **/


int32_t memwrite(void* valorAEscribir, uint32_t direccionLogicaAEscribir, uint32_t pid, uint32_t size);

/*Funciones desarrolladas extras para correcto funcionamiento de la memoria*/


/**
 * @NAME: inicializar_memoria_principal
 * @DESC: Se reserva el espacio para la memoria principal
 *
 **/


void inicializar_memoria_principal();

/**
 * @NAME: liberar_estructuras_administrativas
 * @DESC: Se liberan las estructuras administrativas
 *
 **/


bool cantidad_valida(uint32_t size);


/**
 * @NAME: escribir_directamente_en_memoria
 * @DESC: funcion que escribe en la memoria a partir de una direccion logica (es la que tiene delegada la logica del memcpy)
 *
 **/


void escribir_directamente_en_memoria(void* valorAEscribir, int32_t tamanio, uint32_t direccionLogica);


#endif /* MEMORIA_MEMORIA_C_ */
