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

/*
 * esta_el_carpincho_en_lista_carpinchos(t_carpincho_swamp* carpincho)
 * EVALUA SI EL CARPINCHO SE ENCUENTRA EN LA LISTA LISTA_CARPINCHO
 *
 *
 */
bool esta_el_carpincho_en_lista_carpinchos(t_carpincho_swamp* carpincho);

/*
 * agregar_carpincho_a_lista_carpinchos(t_carpincho_swamp* carpincho)
 * Agrega el carpincho a la lista_carpinchos para cosas administrativas
 *
 *
 */
void agregar_carpincho_a_lista_carpinchos(t_carpincho_swamp* carpincho);

#endif /* MANEJO_FILESYSTEM_LOGICA_ALMACENAMIENTO_H_ */
