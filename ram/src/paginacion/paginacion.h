#ifndef PAGINACION_PAGINACION_H_
#define PAGINACION_PAGINACION_H_

#include <stdint.h>

#include "../../../shared/codigo_error.h"
#include "../../../shared/estructura_compartida.h"
#include "../../src/conexion_swap/conexion_swap.h"
#include "../../src/memoria/memoria.h"
#include "../../src/TLB/tlb.h"
#include "../../../shared/logger.h"
#include "swaping.h"
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "estructuras_paginacion.h"


/**
 * @NAME: inicializar_estructuras_administrativas_paginacion
 * @DESC: Inicializa los diccionarios y las listas
 **/
void inicializar_estructuras_administrativas_paginacion();


/**
 * @NAME: obtener_socket
 * @DESC: obtiene el socket de comunicacion con el swamp
*/

uint32_t obtener_socket();

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
* @NAME: se_puede_almacenar_el_alloc_para_proceso
* @DESC: Dado un proceso  (nuevo o existente) y un tamanio, se establece si puede ser guardado en memoria. Se usa para preguntarle a
*         la swap si tiene espacion para guardarlo.
* @RET: 0 si no puede
*         1 si puede
**/
int32_t se_puede_almacenar_el_alloc_para_proceso(t_header header, uint32_t pid, uint32_t size);

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
 * @NAME: tamanio_de_direccion
 * @DESC: retorna el tamanio de una direccion, obtenido mediante el hmd
 */
uint32_t tamanio_de_direccion(uint32_t direccionLogicaALeer, uint32_t pid);

/**
* @NAME: traducir_a_dir_fisica
* @DESC: traduce de direccion logica a fisica
* @RET:  frame * tam_pag + offset (si lo hay)
*/
uint32_t traducir_a_dir_fisica(uint32_t PID,  uint32_t logica, uint32_t bitModificado);

/**
 * @NAME: inicializar_estructuras_administrativas()
 * @DESC: inicializa las estructuras administrativas
 **/
void inicializar_estructuras_administrativas();

 /**
 * @NAME: destruir_estructuras_administativas
 * @DESC: libera la memoria reservada por las estructuras administrativas
 **/
void destruir_estructuras_administrativas();

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

/**
* @NAME: leer_de_memoria
* @DESC: Devuelvo un ptro con lo leido de memoria que estaba referenciado por la dirLog a leer
* 		Se encarga de invocar la funcion de memoria para la lectura
**/
void* leer_de_memoria(int32_t direccionLogicaALeer, uint32_t pid, uint32_t tamanioALeer);


/**
 * @NAME: calcular_pagina_de_puntero_logico
 * @DESC: devuelve la pagina a la cual corresponderia un determinado puntero logic
 * @EXAMPLE: calcular_pagina_de_puntero_logico(100) - con paginas de 64
 * 			 valor de retorno = 2 (pagina 2)
 */
uint32_t calcular_pagina_de_puntero_logico(uint32_t puntero);

/**
 * @NAME: calcular_offset_puntero_en_pagina
 * @DESC: calcula el offset para un puntero dentro de una pagina
 * @EXAMPLE: calcular_offset_puntero_en_pagina(100) - con paginas de 32
 * 			 valor de retorno = 4
 */

uint32_t calcular_offset_puntero_en_pagina(uint32_t puntero);

/**
 * @NAME: escribir_en_memoria
 * @DESC: funcion intermedia entre la funcion memwrite y la funcion que conoce la distribución de la memoria
 */

void escribir_en_memoria(uint32_t pid, void* valor, uint32_t size, uint32_t puntero);

/**
 * @NAME: eliminar_proceso
 * @DESC: Elimina el proceso de RAM liberando los frames ocupados y las estructuras administrativas asociadas/usadas por el proceso
 */
void eliminar_proceso(uint32_t PID);

/**
 * @NAME: suspender_proceso
 * @DESC: Saca al proceso de la RAM liberando los frames usados por la misma y enviando a SWAP las paginas modificadas no cargadas en SWAP
 */
void suspender_proceso(uint32_t PID);




//----------------------------------------------- FUNCIONES PRIVADAS DE USO INTERNO-------------------------------------------------------------
/**
* @NAME: get_proceso
* @DESC: devuelve el proceso con el PID asociado
* @RET:
*    t_proceso*		si el proceso buscado existe
*    NULL			si no existe el proceso buscado
**/
t_proceso* get_proceso_PID(uint32_t PID);

/*
 * @NAME: espacio_de_HEAP
 * @DES: Informa el espacio asociado al heap
 * */
uint32_t espacio_de_HEAP(heap_metadata* heap);

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
uint32_t obtener_marco_de_pagina_en_memoria(uint32_t PID, int pag, uint32_t bit_modificado);

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
void liberar_paginas(heap_metadata* ultimo_heap, t_list* tabla_paginas, uint32_t pid);

/**
* @NAME: actualizar_datos_pagina
* @DESC: actualiza los datos administrativos de la pagina tras su uso
**/
void actualizar_datos_pagina(uint32_t PID, uint32_t nroPag, uint32_t bitModificado, uint32_t bitTLB);

/**
* @NAME: inicializar_datos_pagina
* @DESC: inicializa los datos administrativos de la pagina traido a memoria
**/
void inicializar_datos_pagina(uint32_t PID, uint32_t nroPag, uint32_t marco, uint32_t bitModificado);

/**
* @NAME: esta_en_RAM
* @DESC: Avisa si la pagina del proceso con el PID asociado esta en la RAM
**/
bool esta_en_RAM(uint32_t PID, uint32_t nroPag);

/**
* @NAME: obtener_frame_de_RAM
* @DESC: retorna el frame de la pagina del proceso con el PID asociado
**/
uint32_t obtener_frame_de_RAM(uint32_t PID, uint32_t nroPag);

/**
* @NAME: calcular_tamanio_ultimo_HEAP
* @DESC: calcula el tamanio del ultimo heap del proceso asociado al PID
**/
uint32_t calcular_tamanio_ultimo_HEAP(uint32_t PID);

/**
* @NAME: actualizar_cantidad_frames_por_proceso_RAM
* @DESC: se encarga de mantener actualizado el valor de paginas en RAM
**/
void actualizar_cantidad_frames_por_proceso_RAM(uint32_t PID, int32_t modCant);

/**
 * @NAME: calcular_hash_key
 * @DESC: calcula un hash que va a ser usado como key del diccionario de la TLB.
 *
 * @EXAMPLE: calcular_hash_key(1);
 * 			 hash = "1"
 *
 **/
char* calcular_hash_key_dic(uint32_t pid);

/**
* @NAME: leer_de_memoria_paginada
* @DESC: Se encarga de devolver el msj a memoria.
 * 		 En caso de que el dato este partido en dos o mas paginas, esta funcion se encarga de traer las pags a memoria y mandar la señal a memoria para
 * 		 que guarde los datos.
**/
void* leer_de_memoria_paginada(uint32_t PID, int nroPag, int offset, int tamDato);

/**
 * @NAME: paginas_extras_para_proceso
 * @DESC: calcula la cantidad de paginas nuevas necesarias para ampliar el espacio de direcciones de un proceso. Se fija
 * el espacio disponible en la ultima pagina y lo tiene en cuenta para los calculos
*/

uint32_t paginas_extras_para_proceso(uint32_t pid, uint32_t size);

/**
 * @NAME: calcular_paginas_para_tam
 * @DESC: dado un determinado tamanio, calcula cuantas paginas son necesarias para almacenarlo
*/

int calcular_paginas_para_tamanio(uint32_t tam);

/**
 * @NAME: reservar_frames
 * @DESC: Toma la lista pasada por parametro y la llena con los frames reservados para el proceso dentro de la RAM
*/
void reservar_frames(t_list* lista_frames);

/**
 * @NAME: frame_no_pertenece_a_lista
 * @DESC: Avisa si el frame NO pertenece a la lista de frames pasada como parametro
*/
bool frame_no_pertenece_a_lista(t_list* lista_frames, void* elementoBuscado);

/**
 * @NAME: remover_proceso_PID_lista_procesos
 * @DESC: consigue el proceso asociado al PID y lo remueve de la lista de procesos
*/
t_proceso* remover_proceso_PID_lista_procesos(uint32_t PID);

/**
 * @NAME: liberar_frames
 * @DESC: Se encarga de liberar todos lso frames usados por las paginas del proceso
*/
void liberar_frames_eliminar_proceso(t_proceso* proceso);
/**
 * @NAME: eliminar_frame_reservados
 * @DESC: Agara la lista de frames reservados por el proceso y los remueve de la lista de frames reservados por todos los procesos
*/
void eliminar_frames_reservados(t_proceso* proceso);

#endif /* PAGINACION_PAGINACION_H_ */
