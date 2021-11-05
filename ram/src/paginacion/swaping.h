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

uint32_t traer_pagina_de_SWAP(uint32_t PID, int nroPag);
t_list* obtener_lista_paginas_en_memoria(uint32_t pid);
t_pagina* obtener_pagina_victima(t_list* lista_paginas);
t_pagina* obtener_victima_LRU(t_list* lista_paginas);
t_pagina* obtener_victima_Clock_Modificado(t_list* lista_paginas);
int32_t hay_que_hacer_swap(uint32_t PID);
uint32_t obtener_frame_libre(uint32_t PID);
void* pedir_a_swamp_info_pagina(uint32_t PID, int nroPag);
void* traer_y_controlar_consistencia_paginas(t_pagina* pagina, int nro_pag, uint32_t pid);

#endif /* PAGINACION_SWAPING_H_ */
