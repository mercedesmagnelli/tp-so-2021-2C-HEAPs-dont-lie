#ifndef PAGINACION_PAGINACION_H_
#define PAGINACION_PAGINACION_H_

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct{
	//índice: frame
	uint32_t estado;
	uint32_t proceso;
	uint32_t pagina;
}t_frame;


typedef struct {
	t_list* tablaPaginas; //t_list pagina
	uint32_t miss_proceso;
	uint32_t hits_proceso;
}t_proceso;


/**
 * @NAME: inicializar_estructuras_administrativas_paginacion
 * @DESC: Inicializa los diccionarios y las listas
 **/

void inicializar_estructuras_administrativas_paginacion();




#endif /* PAGINACION_PAGINACION_H_ */
