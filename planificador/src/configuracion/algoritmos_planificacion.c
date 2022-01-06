
#include "algoritmos_planificacion.h"


t_algoritmo obtener_algoritmo(char * algoritmo) {
	if (strcmp(algoritmo, PLANIFICACION_FIFO) == 0) { return FIFO; }
	if (strcmp(algoritmo, PLANIFICACION_SJF) == 0) { return SJF; }
	if (strcmp(algoritmo, PLANIFICACION_HRRN) == 0) { return HRRN; }
	if (strcmp(algoritmo, PLANIFICACION_RR) == 0) { return RR; }

	return -1;
}

