
#ifndef MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_
#define MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_


#include <unistd.h>
#include <stdio.h>
#include "../configuracion/filesystem_configuracion_guardada.h"
#include "../configuracion/filesystem_configuracion.h"
#include "../../../shared/logger.h"
#include <stdint.h>
#include "../inicializar_filesystem/inicializar_filesystem.h"
#include "logica_almacenamiento.h"

/*
 * escribir_particion
 *
 * Escribe en la pagina que se le pasa en el primer parametro, el texto
 * que se pasa en el segundo parametro
 *
 */

int escribir_particion(t_carpincho_swamp* pid_carpincho, uint32_t pagina, char* texto_escribir, t_archivo_swamp* swamp);

/*
 * leer_particion
 *
 * Lee la pagina que se le pasa en el primer parametro de la ruta
 * que se pasa en el segundo parametro.
 *
 */

char* leer_particion(uint32_t pagina, t_archivo_swamp* swamp, t_carpincho_swamp* carpincho, int * resultado);


/*
 * archivo_a_escribir
 * Indica en que archivo se debe escribir la pagina que llega desde la RAM
 * Tambien agrega al struct de los archivso de swamp el pid_carpincho a la lista para saber que se encuentra en este archivo.
 *
 */
t_archivo_swamp* archivo_a_escribir(uint32_t pid_carpincho);

/*
 * crear_carpincho(uint32_t pid_carpincho)
 * 	crea con el pid la estructura t_carpincho_swamp* para poder tener el registro de las paginas y en que marcos se encuentra.
 *
 */

t_carpincho_swamp* crear_carpincho(uint32_t pid_carpincho, uint32_t cantidad_paginas_reservadas);


int reservar_marcos(t_carpincho_swamp* carpincho, uint32_t cantidad_marcos, t_archivo_swamp* swamp);

int borrar_x_cantidad_de_marcos(t_carpincho_swamp* carpincho, uint32_t cantidad_paginas);

int vaciar_marco_del_archivo(uint32_t marco, char* ruta_archivo);

int eliminar_proceso(t_carpincho_swamp* carpincho);

#endif /* MANEJO_FILESYSTEM_MANEJO_ARCHIVOS_H_ */
