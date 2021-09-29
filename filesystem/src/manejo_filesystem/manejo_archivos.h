
#ifndef MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_
#define MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_


#include <unistd.h>
#include <stdio.h>
#include "../configuracion/filesystem_configuracion_guardada.h"
#include "../../../shared/logger.h"
#include <stdint.h>

/*
 * escribir_particion
 *
 * Escribe en la pagina que se le pasa en el primer parametro, el texto
 * que se pasa en el segundo parametro
 *
 */

int escribir_particion(uint32_t pagina, char* texto_escribir, char* ruta_particion);

/*
 * leer_particion
 *
 * Lee la pagina que se le pasa en el primer parametro de la ruta
 * que se pasa en el segundo parametro.
 *
 */

char* leer_particion(uint32_t pagina, char* ruta_particion);

#endif /* MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_ */
