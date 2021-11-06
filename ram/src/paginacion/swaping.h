#ifndef PAGINACION_SWAPING_H_
#define PAGINACION_SWAPING_H_

#include <stdint.h>
#include <commons/collections/list.h>
#include "../../src/configuracion/ram_config_guardada.h"
#include "paginacion.h"

typedef struct{
	uint32_t bit_presencia;
	uint32_t frame;
	double timestamp;
	uint32_t bit_uso;
	uint32_t bit_modificacion;
}t_pagina;


/**
 * @NAME: traer_pagina_de_SWAP
 * @DESC: Trae la pagina a RAM y devuelve el marco donde se guardo.
 * 		  Se fija si es necesario hacer swaping o no.
 **/
uint32_t traer_pagina_de_SWAP(uint32_t PID, int nroPag);

/**
 * @NAME:
 * @DESC:
 **/
t_list* obtener_lista_frames_en_memoria(uint32_t pid) ;

/**
 * @NAME: obtener_lista_paginas_de_frames
 * @DESC: Dado una lista de frames devuelve su lista equivalente en paginas
 **/
t_list* obtener_lista_paginas_de_frames(t_list* lista_frames);

/**
 * @NAME:
 * @DESC:
 **/
t_pagina* obtener_pagina_victima(t_list* lista_paginas);

/**
 * @NAME:
 * @DESC:
 **/
t_pagina* obtener_victima_LRU(t_list* lista_paginas);

/**
 * @NAME:
 * @DESC:
 **/
t_pagina* obtener_victima_Clock_Modificado(t_list* lista_paginas);

/**
 * @NAME: hay_que_hacer_swap
 * @DESC: Avisa si es necesario hacer el swap o simplemente traer la pagina de SWAP
 * @RET:
 * 		0 No hay que hacer swaping
 * 		1 Si hay que hacer swaping
 **/
int32_t hay_que_hacer_swap(uint32_t PID);

/**
 * @NAME: obtener_frame_libre
 * @DESC: Obtiene un frame libre para el proceso
 **/
uint32_t obtener_frame_libre(uint32_t PID);

/**
 * @NAME:
 * @DESC:
 **/
void* pedir_a_swamp_info_pagina(uint32_t PID, int nroPag);

/**
 * @NAME:
 * @DESC:
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

#endif /* PAGINACION_SWAPING_H_ */
