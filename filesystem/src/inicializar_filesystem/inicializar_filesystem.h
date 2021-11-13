/*
 * inicializar_filesystem.h
 *
 *  Created on: 21 sep. 2021
 *      Author: utnso
 */

#ifndef INICIALIZAR_FILESYSTEM_INICIALIZAR_FILESYSTEM_H_
#define INICIALIZAR_FILESYSTEM_INICIALIZAR_FILESYSTEM_H_

#include <unistd.h>
#include <stdio.h>
#include "../configuracion/filesystem_configuracion_guardada.h"
#include "../../../shared/logger.h"
#include <stdint.h>


typedef struct {
	uint32_t pid_carpincho;
	t_list * marcos_reservados;
	t_list * marcos_usados;
	t_list * dupla;//en dupla esta la pagina del proceso que nos pasa la ram y el marco que esta en el swamp.
	int estado_carpincho; //si es 1 es que el carpincho no se pudo crear por lo tanto hay que borrarlo de la memoria.
} t_carpincho_swamp;

typedef struct {
	uint32_t pagina; //pagina de la ram
	uint32_t marco; //marco del fs
} t_dupla_pagina_marco;

t_list* lista_carpinchos;


/*
 * Esta funcion inicializa el swamp creando las particiones del mismo con la configuracion correspondiente.
 * Inicia los archivos vacios.
 */
void iniciar_swamp();

void destroy_lista_carpinchos();

void destroy_carpinchos_swamp(t_carpincho_swamp* carpincho);

#endif /* INICIALIZAR_FILESYSTEM_INICIALIZAR_FILESYSTEM_H_ */
