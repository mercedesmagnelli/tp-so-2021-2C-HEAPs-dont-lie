
#ifndef PLANIFICADORES_ALGORITMOS_H_
#define PLANIFICADORES_ALGORITMOS_H_

#include <commons/collections/list.h>

#include "../configuracion/configuracion_guardada.h"

#include "estructuras.h"

t_hilo * hilo_obtener_siguiente_hilo_a_mover_exec(t_list * procesos_ready);

void algoritmos_iniciar();

void algoritmos_destruir();



#endif /* PLANIFICADORES_ALGORITMOS_H_ */
