#ifndef TLB_TLB_H_
#define TLB_TLB_H_

#include <commons/collections/list.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../../src/configuracion/ram_config_guardada.h"
#include "../paginacion/estructuras_paginacion.h"

#include "../../../shared/logger.h"
t_list* TLB;
uint32_t max_entradas;
typedef struct{
	char* hash_key;
	uint32_t frame;
	double timestamp; //ver si es necesario, lo agrego por ahora porque no sé como vamos a hacer para seleccionar la entrada que vuela
}entrada_tlb;

int tiempo;


//	FUNCIONES PUBLICAS

/**
 * @NAME: inicializar_tlb
 * @DESC: inicializa la TLB, se reservan los espacios de memoria.
 *
 **/

void imprimir_tlb();

void inicializar_tlb();

/**
 * @NAME: limpiar_tlb
 * @DESC: se eliminan todas las entadas de la TLB. Esta funcion se invoca cuando se llama a la signal sigurs2
 **/
void limpiar_tlb();

/**
 * @NAME: agregar_entrada_tlb
 * @DESC: agrega una entrada a la TLB a partir de un pid y numero de pagina
 *
 */
void agregar_entrada_tlb(uint32_t pid, uint32_t pagina, uint32_t frame);

/**
 * @NAME: esta_en_tlb
 * @DESC: funcion que dado un pid y una pagina, retorna si esta presente en la tlb
 *
 **/
bool esta_en_tlb(uint32_t pid, uint32_t pag);

/**
 * @NAME: obtener_frame_de_tlb
 * @DESC: se retorna el frame de una pagina de un proceso dada
 *
 **/
uint32_t obtener_frame_de_tlb(uint32_t proceso, uint32_t pagina);

/**
 * @NAME: actualizar_datos_TLB
 * @DESC: actualiza los datos necesarios en la TLB cuando se usa la entrada
 *
 **/
void actualizar_datos_TLB(uint32_t PID, uint32_t nroPag);

/**
 * @NAME: destruir_tlb
 * @DESC: elimina la tlb y sus elementos de la memoria
 *
 **/
void destruir_tlb();



//	FUNCIONES PRIVADAS

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
 * @NAME: reemplazar_entrada_tlb
 * @DESC: funcion que dado un indice y una entrada, coloca esa entrada en dicha posicion
 *
 **/
void reemplazar_entrada_tlb(entrada_tlb* entradaNueva, uint32_t indice);

/**
 * @NAME:obtener_timestamp_actual
 * @DESC: se obtiene la timestamp
 *
 **/
double obtener_timestamp_actual();

/**
 * @NAME: conseguir_victima_entrada_LRU
 * @DESC: Encuentra el índice de la entrada que fue la que hace mas tiempo
 * permanece igual
*/

uint32_t conseguir_victima_entrada_LRU();

/**
 * @NAME: elimina_entrada
 * @DESC: se elimina una entrada en un índice
 */

void eliminar_entrada(uint32_t indice_victima);

/**
 * @NAME: obtener_entrada_victima
 * @DESC: retorna el indice de la entrada a eliminar. Llama a, si es necesario, a
 * una funcion de calculo de victima
 */

uint32_t obtener_entrada_victima();

/**
 * @NAME: destructor_de_entradas
 * @DESC: funcion intermedia para eliminar una entrada. Se usa para eliminar entrada con las funciones de listas de las commons
 */
void destructor_de_entradas(void* entrada);

#endif /* TLB_TLB_H_ */


