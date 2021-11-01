
#ifndef MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_
#define MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_


#include <unistd.h>
#include <stdio.h>
#include "../configuracion/filesystem_configuracion_guardada.h"
#include "../configuracion/filesystem_configuracion.h"
#include "../../../shared/logger.h"
#include <stdint.h>


/*
 * escribir_particion
 *
 * Escribe en la pagina que se le pasa en el primer parametro, el texto
 * que se pasa en el segundo parametro
 *
 */

int escribir_particion(uint32_t pid_carpincho, uint32_t pagina, char* texto_escribir, t_archivo_swamp* swamp);

/*
 * leer_particion
 *
 * Lee la pagina que se le pasa en el primer parametro de la ruta
 * que se pasa en el segundo parametro.
 *
 */

char* leer_particion(uint32_t pagina, t_archivo_swamp* swamp);


/*
 * archivo_a_escribir
 * Indica en que archivo se debe escribir la pagina que llega desde la RAM
 * Tambien agrega al struct de los archivso de swamp el pid_carpincho a la lista para saber que se encuentra en este archivo.
 *
 */
t_archivo_swamp* archivo_a_escribir(uint32_t pid_carpincho);

#endif /* MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_ */
