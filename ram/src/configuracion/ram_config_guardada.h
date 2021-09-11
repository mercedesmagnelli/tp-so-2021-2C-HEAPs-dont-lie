/*
 * ram_config_guardada.h
 *
 *  Created on: 5 may. 2021
 *      Author: utnso
 */

#ifndef CONFIGURACION_RAM_CONFIG_GUARDADA_H_
#define CONFIGURACION_RAM_CONFIG_GUARDADA_H_

#define LEAST_RECENTLY_USED "LRU"
#define ESQUEMA_PAGINACION "PAGINACION"
#define CRITERIO_BEST_FIT "BEST_FIT"

typedef enum {
	BEST_FIT,
	FIRST_FIT
}t_criterio;

typedef enum {
	PAGINACION,
	SEGMENTACION
} t_esquema;

typedef enum {
	LRU,
	CLOCK
} t_algoritmo_ram;

// GETTERS
int get_tamanio_memoria();
t_esquema get_esquema_memoria();
int get_tamanio_pagina();
int get_tamanio_swap();
char * get_path_swap();
t_algoritmo_ram get_algoritmo();
int get_puerto();
t_criterio get_criterio();
char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();

typedef struct {

	int tamanio_memoria;
	t_esquema esquema_memoria;
	int tamanio_pagina;
	int tamanio_swap;
	char * path_swap;
	t_algoritmo_ram algoritmo;
	int puerto;
	t_criterio criterio;

	// Loggin
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;

} t_info_config;

/**
 * @NAME: config_guardada
 * @DESC: Contiene toda la informacion leida del archivo de configuracion de la ram
 */
t_info_config config_guardada;

#endif /* CONFIGURACION_RAM_CONFIG_GUARDADA_H_ */
