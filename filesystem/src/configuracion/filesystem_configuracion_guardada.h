
#ifndef CONFIGURACION_IFILESYSTEM_CONFIG_GUARDADA_H_
#define CONFIGURACION_IFILESYSTEM_CONFIG_GUARDADA_H_

// STRUCTS
#include <commons/collections/list.h>
typedef struct{
	int pos_x;
	int pos_y;
}t_posicion;

typedef struct {
	char * ip;
	int puerto;

	int tamanio_swap;
	int tamanio_pagina;
	int marcos_maximos;
	int retardo_swap;

	int cantidad_archivos;
	char ** archivos_swap;

	// Loggin
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;

} t_info_config;


// GETTERS
char * get_ip();
int get_puerto();
int get_tamanio_swap();
char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();
int get_tamanio_pagina();
int get_marcos_maximos();
int get_retardo_swap();
int get_cantidad_archivos();
char ** get_archivos_swap();


/**
 * @NAME: config_guardada
 * @DESC: Contiene toda la informacion leida del archivo de configuracion del i-filesystem-store
 */
t_info_config config_guardada;

#endif /* CONFIGURACION_IFILESYSTEM_CONFIG_GUARDADA_H_ */
