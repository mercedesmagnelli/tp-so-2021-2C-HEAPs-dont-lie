/*
 * logica_almacenamiento.h
 *
 *  Created on: 20 oct. 2021
 *      Author: utnso
 */

#ifndef MANEJO_FILESYSTEM_LOGICA_ALMACENAMIENTO_H_
#define MANEJO_FILESYSTEM_LOGICA_ALMACENAMIENTO_H_

#include <unistd.h>
#include <stdio.h>
#include "../configuracion/filesystem_configuracion_guardada.h"
#include "../../../shared/logger.h"
#include <stdint.h>
#include "manejo_archivos.h"


/*	particion_a_escribir(uint32_t pid_carpincho)
 * 	indica en que particion se debe escribir. Lo que hace es buscar si ya se escribio este carpincho en un archivo para
 * 	escribirlo en el mismo. SI no se escribio, se escribe en el archivo que tenga mas espacio como se definio en el tp.
 *
 */

t_archivo_swamp* particion_a_escribir(uint32_t pid_carpincho);

#endif /* MANEJO_FILESYSTEM_LOGICA_ALMACENAMIENTO_H_ */
