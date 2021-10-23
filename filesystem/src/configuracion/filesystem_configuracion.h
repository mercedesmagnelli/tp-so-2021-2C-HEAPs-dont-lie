
#ifndef IFILESYSTEM_CONFIGURACION_H_
#define IFILESYSTEM_CONFIGURACION_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <commons/config.h>
#include <commons/string.h>

#include "filesystem_configuracion_guardada.h"

#include "../../../shared/codigo_error.h"
#include "../../../shared/logger.h"

typedef struct {
	char* ruta_archivo;
	t_list * carpinchos;
	int espacio_libre;
} t_archivo_swamp;


t_list* lista_swamp;

/**
 * @NAME: son_validos_los_parametros
 * @DESC: Retorna true si hay 2 argumentos y el 2do termina en ".config"
 *
 * Ejemplo:
 * son_validos_los_parametros(2, ["...", "a.config"]) = true
 * son_validos_los_parametros(2, ["...", "a.no_config"]) = false
 * son_validos_los_parametros(1, ["..."]) = false
 */
bool son_validos_los_parametros(int argc, char ** argv);

/**
 * @NAME: iniciar_configuracion
 * @DESC: Lee el archivo de configuracion que pasan por parametro e inicializa las variables
 */
int iniciar_configuracion(int argc, char ** argv);

/**
 * @NAME: destroy_configuracion
 * @DESC: Libera la memoria de todos los punteros de la variable "global" config_guardada
 */

void destroy_configuracion();

/**
 * @NAME: destroy_lista_swamp
 * @DESC: Destruye la variable global lista_swamp que es una lista.
*/
void destroy_lista_swamp();

/**
 * @NAME: destruir_archivo_swamp
 * @DESC: Destruye la estructura t_archivo_swamp.
*/
void destruir_archivo_swamp(t_archivo_swamp* swamp);

/**
 * @NAME: destruir_string
 * @DESC: Sirve para hacer los free en una lista.
*/
void destruir_string(void* el_String);

#endif /* IFILESYSTEM_CONFIGURACION_H_ */
