/*
 * logger.h
 *
 *  Created on: 3 may. 2021
 *      Author: utnso
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>

#include <commons/log.h>
#include <commons/string.h>
#include "codigo_error.h"

/**
 * @NAME: init_mutex_log
 * @DESC: Inicializa el loguer con su mutex
 *
 * @param: archivo - Nombre del archivo a guardar
 * @param: nombre_archivo - Nombre del programa con el que se loguea
 * @param: activar_consola - Mostrar log en la consola ademas del archivo
 * @param: level - Nivel de logueo
 *
 * @return: Codigo de error
 */
int init_mutex_log(char* archivo, char* nombre_programa, int activar_consola, t_log_level level);

/**
 * @NAME: loggear_info
 * @DESC: Loguea en el archivo que se haya definido el mensaje
 */
int loggear_info(const char* mensaje, ...);

/**
 * @NAME: loggear_debug
 * @DESC: Loguea en el archivo que se haya definido el mensaje
 */
int loggear_debug(const char* mensaje, ...);

/**
 * @NAME: loggear_trace
 * @DESC: Loguea en el archivo que se haya definido el mensaje
 */
int loggear_trace(const char* mensaje, ...);

/**
 * @NAME: loggear_warning
 * @DESC: Loguea en el archivo que se haya definido el mensaje
 */
int loggear_warning(const char* mensaje, ...);

/**
 * @NAME: loggear_error
 * @DESC: Loguea en el archivo que se haya definido el mensaje
 */
int loggear_error(const char* mensaje, ...);

/**
 * @NAME: destroy_log
 * @DESC: Limpia de la memoria el logger y el mutex
 *
 * @return: Codigo de error
 */
int destroy_log();

#endif /* LOGGER_H_ */
