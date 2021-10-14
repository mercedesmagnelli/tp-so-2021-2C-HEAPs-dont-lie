#ifndef TLB_TLB_H_
#define TLB_TLB_H_

#include <commons/collections/dictionary.h>

#include "../../../shared/logger.h"

typedef struct{
	int frame;
	int timestamp; //ver si es necesario, lo agrego por ahora porque no sé como vamos a hacer para seleccionar la entrada que vuela
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
void agregar_entrada_tlb(int proceso, int pagina, int frame);

/**
 * @NAME: calcular_hash_key
 * @DESC: calcula un hash(medio pelo) que va a ser usado como key del diccionario de la TLB.
 *
 * @EXAMPLE: calcular_hash_key(1, 2);
 * 			 hash = "1-2"
 *
 **/

char* calcular_hash_key(int pid, int pag);
/**
 * @NAME: esta_en_tlb
 * @DESC: funcion que dado un pid y una pagina, retorna si esta presente en la tlb
 *
 **/

bool esta_en_tlb(int pid, int pag);

/**
 * @NAME: reemplazar_entrada_tlb
 * @DESC: funcion que dado un indice y una entrada, coloca esa entrada en dicha posicion
 *
 **/

void reemplazar_entrada_tlb(entrada_tlb* entradaNueva, int indice);

/**
 * @NAME: leer_direccion_de_tlb
 * @DESC: se retorna la direccion de una pagina de un proceso dada
 *
 **/

int obtener_direccion_de_tlb(char* key);

#endif /* TLB_TLB_H_ */


