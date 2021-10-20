#ifndef MEMORIA_MEMORIA_C_
#define MEMORIA_MEMORIA_C_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>

typedef struct{
	 uint32_t prevAlloc;
	 uint32_t nextAlloc;
	 uint8_t isFree;
}heap_metadata​;



/*Funciones que pide el TP obligatorias*/

/**
 * @NAME: memalloc
 * @DESC: Asigna un espacio dentro de la memoria principal. Si tiene que generar un espacio, lo hace al final de la
 * lista de direcciones. Si usa un espacio entre lugares ocupados y sobra, genera dos divisiones: la ocupada y la del espacio estante.
 * @RET:
 *   >=0 puntero lógico dirigido al espacio de memoria solicitado
 *   -1  se solicito un valor erroneo de memoria (negativo o 0)
 *   -2  el proceso no tiene mas espacio del cual solicitar (Asig Fija)
 *   -3  No hay mas espacio de memoria del cual solicitar (Asig Global)
 **/

uint32_t memalloc(uint32_t pid, uint32_t size);

/**
 * @NAME: memfree
 * @DESC: Libera espacios de memoria. Se encarga de consolidar en caso de que queden dos espacios contiguos libres
 * y de liberar paginas en caso de que con la liberación una página quede vacía.
  * @RET:
 *   0 se pudo liberar el espacio de memoria
 *  -1 espacio no accedible/existente
 *  -2 el espacio estaba liberado
 **/

void memfree(uint32_t* direccionLogicaALiberar);

/**
 * @NAME: memread
 * @DESC: A partir de una direccion lógica a leer, se retorna lo que está guardado en esa posición.

 **/


void* memread(uint32_t* direccionLogicaALeer);


/**
 * @NAME: memwrite
 * @DESC: Se encarga de escribir informacion en memoria a partir de una direccion lógica
  * @RET:
 *   0 se guardo exitosamente la data en memoria
 *  -1 espacio no accedible/ existente
 *  -2 el espacio estaba liberado
 *
 **/


void memwrite(void* valorAEscribir, uint32_t* direccionLogicaAEscribir /*t_list* tablaPaginas*/);

/*Funciones desarrolladas extras para correcto funcionamiento de la memoria*/


/**
 * @NAME: inicializar_memoria_principal
 * @DESC: Se reserva el espacio para la memoria principal
 *
 **/


void inicializar_memoria_principal();
/**
 * @NAME: inicializar_estructuras_administrativas
 * @DESC: Se inicializan las estructuras auxiliares
 *
 **/
void inicializar_estructuras_administrativas();
/**
 * @NAME: liberar_estructuras_administrativas
 * @DESC: Se liberan las estructuras administrativas
 *
 **/
void liberar_estructuras_administrativas();
/**
 * @NAME: encontrar_espacio_disponible
 * @DESC: Busca en la estructura administrativa de la memoria el lugar donde puede
 * guardar información.
 * @RET: Retorna la direccion lógica de donde encontró el espacio
 *
 **/
uint32_t* encontrar_espacio_disponible(uint32_t tamanio_a_guardar);


#endif /* MEMORIA_MEMORIA_C_ */
