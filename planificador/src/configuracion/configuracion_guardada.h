/*
 * configuracion_guardada.h
 *
 *  Created on: 2 may. 2021
 *      Author: utnso
 */

#ifndef CONFIGURACION_CONFIGURACION_GUARDADA_H_
#define CONFIGURACION_CONFIGURACION_GUARDADA_H_

#include <stdbool.h>

#define ROUND_ROBIN "RR"

typedef enum {
	FIFO,
	RR
} t_algoritmo;

// GETTERS
char * get_ip_ram();
int get_puerto_ram();
char * get_ip_filesystem();
int get_puerto_filesystem();
int get_grado_multitarea();
t_algoritmo get_algoritmo();
int get_quantum();
int get_duracion_sabotaje();
int get_retardo_ciclo_cpu();
char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();

typedef struct {
	char * ip_ram;
	int puerto_ram;

	char * ip_filesystem;
	int puerto_filesystem;

	int grado_multitarea;
	t_algoritmo algoritmo;
	int quantum;
	int duracion_sabotaje;
	int retardo_ciclo_cpu;

	// Loggin
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;
} t_info_config;

/**
 * @NAME: config_guardada
 * @DESC: Contiene toda la informacion leida del archivo de configuracion del planificador
 */
t_info_config config_guardada;

#endif /* CONFIGURACION_CONFIGURACION_GUARDADA_H_ */
