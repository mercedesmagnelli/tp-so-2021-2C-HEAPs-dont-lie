#ifndef MANEJO_SIGNALS_MANEJO_SIGNALS_H_
#define MANEJO_SIGNALS_MANEJO_SIGNALS_H_

#include "../paginacion/estructuras_paginacion.h"
#include <stdint.h>
#include "../../../shared/logger.h"
/**
 * @NAME: manejar_sigint
 * @DESC: Esta funcion se desencadena tras apretar ctrl + c y lo que hace es hacer un reporte de la smetricas de hit y miss
 *  Cantidad de TLB Hit totales
	Cantidad de TLB Hit por proceso indicando proceso y cantidad.
	Cantidad de TLB Miss totales.
	Cantidad de TLB Miss por proceso indicando proceso y cantidad.

 **/

void manejar_sigint();

/**
 * @NAME: manejar_sisr1
 * @DESC: Esta funcion se desencadena mandar sigusr1 lo que hace es realizar un archivo de dump
 * de la memoria, donde se vea la siguiente informacion:
 *  Número de entrada
	Estado
	PID
	Número de página
	Marco asignado en la memoria.
 **/

void manejar_sigusr1();

/**
 * @NAME: manejar_sigur2
 * @DESC: se limpian todas entradas de la TLB
 *
 **/

void manejar_sigusr2();
void cerrar_todo();
void imprimir_entrada_proceso(t_proceso* p);

#endif /* MANEJO_SIGNALS_MANEJO_SIGNALS_H_ */
