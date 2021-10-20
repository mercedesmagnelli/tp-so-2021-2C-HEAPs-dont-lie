#ifndef TLB_TLB_H_
#define TLB_TLB_H_

#include <commons/collections/dictionary.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../../shared/logger.h"


typedef struct{
	uint32_t frame;
	double timestamp; //ver si es necesario, lo agrego por ahora porque no sé como vamos a hacer para seleccionar la entrada que vuela
}entrada_tlb;

/**
 * @NAME: inicializar_tlb
 * @DESC: inicializa la TLB, se reservan los espacios de memoria.
 *
 **/

void inicializar_tlb();

/**
 * @NAME: agregar_entrada_tlb
 * @DESC: agrega una entrada a la TLB a partir de un pid y numero de pagina
 *
 */

void agregar_entrada_tlb(uint32_t pid, uint32_t pagina, uint32_t frame);


/**
 * @NAME: calcular_hash_key
 * @DESC: calcula un hash(medio pelo) que va a ser usado como key del diccionario de la TLB.
 *
 * @EXAMPLE: calcular_hash_key(1, 2);
 * 			 hash = "1-2"
 *
 **/

char* calcular_hash_key(uint32_t pid, uint32_t pag);
/**
 * @NAME: esta_en_tlb
 * @DESC: funcion que dado un pid y una pagina, retorna si esta presente en la tlb
 *
 **/

bool esta_en_tlb(uint32_t pid, uint32_t pag);

/**
 * @NAME: reemplazar_entrada_tlb
 * @DESC: funcion que dado un indice y una entrada, coloca esa entrada en dicha posicion
 *
 **/

void reemplazar_entrada_tlb(entrada_tlb* entradaNueva, uint32_t indice);

/**
 * @NAME: obtener_frame_de_tlb
 * @DESC: se retorna el frame de una pagina de un proceso dada
 *
 **/

uint32_t obtener_frame_de_tlb(char* key);

/**
 * @NAME:obtener_timestamp_actual
 * @DESC: se obtiene la timestamp
 *
 **/

double obtener_timestamp_actual();

/**
 * @NAME: limpiar_tlb
 * @DESC: se eliminan todas las entadas de la TLB. Esta funcion se invoca cuando se llama a la signal sigurs2
 **/

void limpiar_tlb();


#endif /* TLB_TLB_H_ */


