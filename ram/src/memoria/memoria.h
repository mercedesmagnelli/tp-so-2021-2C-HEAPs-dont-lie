#ifndef MEMORIA_MEMORIA_C_
#define MEMORIA_MEMORIA_C_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>

/*Variables globales*/

void* memoria_principal;
t_dictionary* cant_frames_proceso;
t_list* adm_memoria;



typedef struct{
	uint32_t comienzo;
	bool esta_libre;

}t_datos;

typedef struct{
	uint32_t en_mp; //me sirve de flag para comprobar cuando realmente hacer el swap
	uint32_t en_mv; // me sirve para tener el control de la cant maxima por proceso
}asignacion_marcos_fijos;
// Lo pondría para saber cuándo tengo que mandar a MV, a no ser que mandemos siempre a MV de una
/* CODIGOS DE ERROR
 *
 * - Frames maximos alcanzados
 * - No hay memoria disponible
 * */

/*Funciones que pide el TP obligatorias*/

/**
 * @NAME: memalloc
 * @DESC: Asigna un espacio dentro de la memoria principal. Si tiene que generar un espacio, lo hace al final de la
 * lista de direcciones. Si usa un espacio entre lugares ocupados y sobra, genera dos divisiones: la ocupada y la del espacio estante.
 * @RET: Retorna la direccion logica del comienzo de la asignación y NULL en caso de que no pueda asignarlo.
 *
 **/

uint32_t memalloc(uint32_t pid, uint32_t size);

/**
 * @NAME: memfree
 * @DESC: Libera espacios de memoria. Se encarga de consolidar en caso de que queden dos espacios contiguos libres
 * y de liberar paginas en caso de que con la liberación una página quede vacía.
 *
 **/

void memfree(uint32_t* direccionLogicaALiberar);

/**
 * @NAME: memread
 * @DESC: A partir de una direccion lógica a leer, se retorna lo que está guardado en esa posición.
 *
 **/

void* memread(uint32_t* direccionLogicaALeer);


/**
 * @NAME: memwrite
 * @DESC: Se encarga de escribir informacion en memoria a partir de una direccion lógica
 * Puede llegar a llamar a memalloc si lo que quiere escribirse excede.
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
