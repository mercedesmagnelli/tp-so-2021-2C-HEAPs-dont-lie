#ifndef PAGINACION_SWAPING_H_
#define PAGINACION_SWAPING_H_

#include <stdint.h>
#include <commons/collections/list.h>
#include "../../src/configuracion/ram_config_guardada.h"
#include "estructuras_paginacion.h"
#include "../../src/memoria/memoria.h"
#include "paginacion.h"
#include "../../../shared/estructura_compartida.h"
uint32_t socket_swap;

/**
 * @NAME: crear_proceso_SWAP
 * @DESC: solicitamos la creacion del proceso en SWAP
 * @RET:
 * 		1 Se pudo inicializar el proceso en SWAP
 * 		0 No se pudo inicializar el proceso en SWAP
*/
uint32_t crear_proceso_SWAP(uint32_t PID);

/**
 * @NAME: traer_pagina_de_SWAP
 * @DESC: Trae la pagina a RAM y devuelve el marco donde se guardo.
 * 		  Se fija si es necesario hacer swaping o no.
 **/
uint32_t traer_pagina_de_SWAP(uint32_t PID, int nroPag);

/**
 * @NAME: obtener_lista_frames_en_memoria
 * @DESC: devuelve una lista con los frames que estan siendo ocupados por un proceso
 * 		FIJA: Devuelve los frames reservados
 * 		DINAMICA: Devuelve la global (porque es independeinte del proceso)
 **/

t_list* obtener_lista_frames_en_memoria(uint32_t pid) ;

/**
 * @NAME: obtener_lista_paginas_de_frames
 * @DESC: Dado una lista de frames devuelve su lista equivalente en paginas
 **/

t_list* obtener_lista_paginas_de_frames(t_list* lista_frames);

/**
 * @NAME: obtener_pagina_victima
 * @DESC: funcion visagra que deriva en los algoritmos de reemplazo de paginas
 **/
t_pagina* obtener_pagina_victima(t_list* lista_paginas, uint32_t pid);

/**
 * @NAME: obtener_victima_LRU
 * @DESC: obtiene la pagina a intercambiar segun el algoritmo LRU
 **/
t_pagina* obtener_victima_LRU(t_list* lista_paginas);

/**
 * @NAME: obtener_victima_Clock_Modificado
 * @DESC: obtiene la pagina a intercambiar segun el algortimo Clock Modificado
 **/
t_pagina* obtener_victima_Clock_Modificado(t_list* lista_paginas,uint32_t pid);

/**
 * @NAME: hay_que_hacer_swap
 * @DESC: Avisa si es necesario hacer el swap o simplemente traer la pagina de SWAP
 * @RET:
 * 		0 No hay que hacer swaping
 * 		1 Si hay que hacer swaping
 **/
int32_t hay_frame_disponible_en_RAM(uint32_t PID);

/**
 * @NAME: obtener_frame_libre
 * @DESC: Obtiene un frame libre para el proceso
 **/
uint32_t obtener_frame_libre(uint32_t PID);


/**
 * @NAME: traer_y_controlar_consistencia_paginas
 * @DESC: se encarga de controlar si la pagina que esta en memoria esta modificada:
 * en caso de que este modificada entonces tiene que mandarle a sweap la informacion antes de
 * recibir nueva
 **/
void* traer_y_controlar_consistencia_paginas(t_pagina* pagina, int nro_pag, uint32_t pid);

/**
 * @NAME: frame_disponible
 * @DESC: funcion usada para buscar un frame disponible en las listas. Avisa si el elemento frame esta disponible o no
 **/
bool frame_disponible(void* element);

/**
 * @NAME: comunicar_eliminacion_proceso_SWAP
 * @DESC: Avisa al modulo SWAP de la eliminacion de un proceso asociado al PID
 **/
void comunicar_eliminacion_proceso_SWAP(uint32_t PID);

/**
 * @NAME: modificar_bit_uso
 * @DESC: se encarga de mapear la lista de paginas y colocarle en 0 el bit de uso a todas (funcion de clock_modificado)
 **/

void modificar_bit_uso(t_list* lista_paginas);

/**
 * @NAME: calcular_indice
 * @DESC: calcula el indice que tengo que leer dentro de la lista de paginas
 * 		  para el clock modificado. Si se pasa del rango, calcula la division resto
 **/

uint32_t calcular_indice(uint32_t puntero, uint32_t cantidad_it, uint32_t tamanio_lista);

/**
 * @NAME: buscar_combinacion
 * @DESC: se encarga de buscar determinada combiancion de bit de uso y modificado para
 * 		 el algoritmo de clock modificado
 **/

bool buscar_combinacion(t_list* paginas, uint32_t uso, uint32_t mod, uint32_t puntero,  uint32_t* indice_encontrado);

/**
 * @NAME: obtener_valor_puntero
 * @DESC: se carga en una variable local el valor del putneor que depende del
 *		  tipo de asignacion que se tenga
 **/

uint32_t obtener_valor_puntero(t_proceso* proc);

/**
 * @NAME: actualizar_puntero
 * @DESC: se actualiza el punteor que se utiliza para saber donde arrancar a leer la proxima vez
 * que se requiera hacer un swaping y usar el algoritmo de cm
 **/

void actualizar_puntero(t_proceso* proc, uint32_t indice_encontrado);

/**
 * @NAME: enviar_pagina_a_SWAP
 * @DESC: Se le envia a SWAP para que guarde la pagina del PID asociado
 **/
void enviar_pagina_a_SWAP(uint32_t PID, uint32_t nroPag, void* dataPag);


/*falta poner los headers*/

void* recibir_info_en_pagina(uint32_t pag_a_pedir, uint32_t pid_a_pedir);
void* obtener_info_en_frame(uint32_t pagina_victima);
uint32_t obtener_pid_en_frame(uint32_t frame);
uint32_t obtener_pag_en_frame(uint32_t frame);
#endif /* PAGINACION_SWAPING_H_ */
