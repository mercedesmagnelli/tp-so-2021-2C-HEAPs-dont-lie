#ifndef PAGINACION_PAGINACION_H_
#define PAGINACION_PAGINACION_H_

#include <stdint.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "../../../shared/codigo_error.h"
#include "../../src/conexion_swap/conexion_swap.h"
#include "../../src/configuracion/ram_config_guardada.h"
#include "../../src/memoria/memoria.h"
#include <stdlib.h>
#include <string.h>

t_list* listaProcesos;
t_list* listaFrames;
t_dictionary* cant_frames_por_proceso;

typedef struct{
	//índice: frame
	uint32_t estado;
	uint32_t proceso;
	uint32_t pagina;
}t_frame;

typedef struct{
    uint32_t en_mp; //me sirve de flag para comprobar cuando realmente hacer el swap
    uint32_t en_mv; // me sirve para tener el control de la cant maxima por proceso
}asignacion_marcos_fijos;

typedef struct{
	uint32_t currAlloc;
	uint32_t prevAlloc;
	uint32_t nextAlloc;
	uint8_t isFree;
}heap_metadata;


typedef struct {
	uint32_t PID;
	t_list* tabla_paginas; //t_list pagina
	t_list* lista_hmd;
	uint32_t miss_proceso;
	uint32_t hits_proceso;
}t_proceso;

typedef struct{
	uint32_t bit_presencia;
	uint32_t frame;
	uint32_t timestamp;
	uint32_t bit_uso;
	uint32_t bit_modificacion;
}t_pagina;


/**
 * @NAME: inicializar_estructuras_administrativas_paginacion
 * @DESC: Inicializa los diccionarios y las listas
 **/
void inicializar_estructuras_administrativas_paginacion();





// FUNCIONES PUBLICAS

/**
 * @NAME: existe_proceso
 * @DESC: Se fija si existe el proceso en la lista administrativa
 * @RET:
 *   1 existe el proceso
 *   0 no existe el proceso
 **/
int32_t existe_proceso(uint32_t PID);

/**
 * @NAME: ptro_donde_entra_data
 * @DESC: Dado un proceso(ya allocado) y el tamaño de la data te devuelve el alloc donde puede entrar, sin pedir memoria a la RAM
 * @RET:
 *   >=0 Devuelve el puntero al alloc donde se puede guardar la data
 *   <0 Devuelve el puntero al ultimo alloc con valor negativo
 **/
int32_t ptro_donde_entra_data(uint32_t PID, uint32_t tam);

/**
 * @NAME: actualizo_proceso
 * @DESC: Actualiza el HEAP seleccionado del proceso y, en caso necesario, agrega uno nuevo
 **/
void actualizar_proceso(uint32_t PID, int32_t ptro, uint32_t tamanio);

/**
 * @NAME: agrego_proceso
 * @DESC: Agrega el proceso a la lista de procesos y crea los 2 HEAPs necesarios
 * @RET: devuelve el puntero en el cual entra el tamaño solicitado
 **/
int32_t agregar_proceso(uint32_t PID, uint32_t tam);

/**
 * @NAME: ptro_valido
 * @DESC: Retorna si la el puntero pertenece a la direccion de punteros asignados al proceso
*/
bool ptro_valido(uint32_t PID, uint32_t ptro);

/**
 * @NAME: ptro_liberado
 * @DESC: Avisa si el puntero del proceso esta liberado
 **/

bool ptro_liberado(uint32_t PID, uint32_t ptro);

/**
 * @NAME: liberar_memoria
 * @DESC: libera la memoria del ptro seleccionado
 **/
void liberar_memoria(uint32_t PID, uint32_t ptro);

/**
 * @NAME: consolidar_memoria
 * @DESC: Se fija los cambios administrativos que se tengan que hacer por liberar memoria
 **/
void consolidar_memoria(uint32_t PID);

/**
 * @NAME: leer_de_memoria
 * @DESC: Lectura de la data de un proceso de memoria
 * @PARAM:
 *   data: ptro donde se guardara la data leida de memoria
 * @RET:
 *   >0 tamanio del dato leido
 **/
uint32_t leer_de_memoria(uint32_t PID, uint32_t ptroMem, void* data);

/**
 * @NAME: entra_data
 * @DESC: Se fija si en el alloc apuntado puede entrar el tamanio del dato seleccionado
 * @RET:
 *    0 ptro permite el tamanio de la data
 *   -3 ptro no permite el tamanio de la data
 **/
uint32_t entra_data(uint32_t PID, uint32_t ptroMem, uint32_t tamanioData);

/**
 * @NAME: tamanio_de_direccion
 * @DESC: retorna el tamanio de una direccion, obtenido mediante el hmd
 */
uint32_t tamanio_de_direccion(uint32_t direccionLogicaALeer, uint32_t pid);

/**
* @NAME: traducir_a_dir_fisica
* @DESC: traduce de direccion logica a fisica
* @RET:  la direccion logica + frame * tam_pag + offset (si lo hay)
*/
uint32_t traducir_a_dir_fisica(uint32_t logica);


/**
 * @NAME: escribir_en_memoria
 * @DESC: Escribe en el ptro de un proceso los datos
 **/
void escribir_en_memoria(int32_t PID, int32_t ptroLogicoMem, int32_t tamanioData, void* data);

/**
 * @NAME: inicializar_estructuras_administrativas()
 * @DESC: inicializa las estructuras administrativas
 **/
void inicializar_estructuras_administrativas();

 /**
 * @NAME: destruir_estructuras_administativas
 * @DESC: libera la memoria reservada por las estructuras administrativas
 **/
void destruir_estructuras_administativas();

/**
* @NAME: se_puede_almacenar_el_alloc_para_proceso
* @DESC: Dado un proceso  (nuevo o existente) y un tamanio, se establece si puede ser guardado en memoria. Se usa para preguntarle a
* 		la swap si tiene espacion para guardarlo.
* @RET: 0 si no puede
* 		1 si puede
**/
int32_t se_puede_almacenar_el_alloc_para_proceso(t_header header, uint32_t pid, uint32_t size);

/**
* @NAME: no_se_asigna_proceso
* @DESC: Retorno la razon por la cual no pude asignar la memoria
* @RET: -42, se posee asignacion fija y se alcanza el maximo de marcos por proceso
* 		-43, en asignacion global, puede no haber mas lugar
**/
int32_t no_se_asigna_proceso(uint32_t pid, uint32_t size);

/**
* @NAME: puedo_pedir_mas_memoria
* @DESC: Retorna si un proceso puede pedir mas memoria
* @RET: 1, si puede
* 		0, no puede
**/
uint32_t puedo_pedir_mas_memoria(uint32_t pid, uint32_t size);

/**
* @NAME: destruir_proceso
* @DESC: funcion de orden superior que invoca la destruccion de la
* lista de procesos. Libera la memoria de las listas del struct y el resto de la memoria asociada.
**/
void destruir_proceso(void* proceso);



// FUNCIONES PRIVADAS DE USO INTERNO
/**
* @NAME: get_proceso
* @DESC: devuelve el proceso con el PID asociado
* @RET:
*    t_proceso*		si el proceso buscado existe
*    NULL			si no existe el proceso buscado
**/
t_proceso* get_proceso_PID(uint32_t PID);

/*
 * @NAME: get_ptro_con_tam_min
 * @DES: busca el metadata en el cual entra el dato de tamaño solicitado
 * @RET: devuelve el metadata que puede contenerlo; caso que ninguno pueda: devuelve el ultimo HEAP
 * */
int32_t get_ptro_con_tam_min(t_list* listaHMD, uint32_t tam);

/*
 * @NAME: get_HEAP
 * @DES: busca el heap del ptro asociado del PID
 * */
heap_metadata* get_HEAP(uint32_t PID, int32_t ptro);

/*
 * @NAME: conseguir_listaHMD_mediante_PID
 * @DES: busca la lista de HEAPs asociado al PID
 * */
t_list* conseguir_listaHMD_mediante_PID(uint32_t PID);

/*
 * @NAME: agregar_HEAP_a_PID
 * @DES: agrega el HEAP en la posicion correcta dentro de la lista de HMD del PID
 * */
void agregar_HEAP_a_PID(uint32_t PID, heap_metadata* heap);

/*
 * @NAME: actualizar_HEAP
 * @DES: realiza las acciones necesarias para guardar HEAP en la memoria
 * */
void guardar_HEAP_en_memoria(uint32_t PID, heap_metadata* heap);

/*
 * @NAME: actualizar_HEAP_en_memoria
 * @DES: Se encarga de mandar el msj a memoria de efectivamente guardar data.
 * 		 En caso de que el dato este partido en dos o mas paginas, esta funcion se encarga de traer las pags a memoria y mandar la señal a memoria para
 * 		 que guarde los datos.
 * */
void guardar_en_memoria_paginada(uint32_t PID, int nroPag, int offset, void* data, int tamDato);

/*
 * @NAME: obtener_pagina_de_memoria
 * @DES: se fija si la pagina esta en memoria RAM (se usa TLB), en caso de no ser asi la trae del SWAP. Actualiza TLB y Pag para algoritmo de reemplazo
 * 		 Luego devuelvo la pagina en cuestion
 * */
t_pagina* obtener_pagina_de_memoria(uint32_t PID, int pag, uint32_t bit_modificado);

/*
 * @NAME: serializar_HEAP
 * @DES: devuelve un ptro donde esta serializada toda la data del HEAP.
 * 		 Se debe liberar el puntero devuelto
 * */

void * serializar_HEAP(heap_metadata* nuevoHeapPrimero);


/*
 * @NAME: obtener_tabla_paginas_mediante_PID
 * @DES: Informa la tabla de paginas del proceso asociado al PID
 * */

t_list* obtener_tabla_paginas_mediante_PID(uint32_t PID);

/*
 * @NAME: cantidad_de_frees_contiguos
 * @DESC: retorna la cantidad de frees seguidos posbles para consolidar
 * @RET: 1 - No hay que consolidar
 * 		 2 o 3: hay para consolidar

*/
uint32_t cantidad_de_frees_contiguos(t_list* lista_heaps, uint32_t* indice);

/**
 * @NAME: calcular_seguidos
 * @DESC: calcula la cantidad de frees seguidos
*/

uint32_t calcular_seguidos(t_list* lista, uint32_t indice);

/**
 * @NAME: modificar_heaps
 * @DESC: hace efectivos los cambios de los heaps en memoria y elimina aquellos heaps que se liberaron
*/
void modificar_heaps(t_list* heaps, uint32_t indice, uint32_t pid, uint32_t cantidad);

/**
 * @NAME: el_ultimo_heap_libera_paginas
 * @DESC: retorna si el ultimo heap es más grande que el tamaño de pagina
*/
bool el_ultimo_heap_libera_paginas(heap_metadata* ultimo_heap);


/**
 * @NAME: liberar_páginas
 * @DESC: libera la cantidad de paginas que estan de mas en el proceos porque
 * el último heap es mas gande que el restante dentro de una pagina
*/
void liberar_paginas(heap_metadata* ultimo_heap, t_list* tabla_paginas);

#endif /* PAGINACION_PAGINACION_H_ */
