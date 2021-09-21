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

/*
 * Esta funcion inicializa el swamp creando las particiones del mismo con la configuracion correspondiente.
 * Inicia los archivos vacios.
 */
void iniciar_swamp();

#endif /* INICIALIZAR_FILESYSTEM_INICIALIZAR_FILESYSTEM_H_ */
