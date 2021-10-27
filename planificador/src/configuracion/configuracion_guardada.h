
#ifndef CONFIGURACION_CONFIGURACION_GUARDADA_H_
#define CONFIGURACION_CONFIGURACION_GUARDADA_H_

#include <stdbool.h>
#include "algoritmos_planificacion.h"

// GETTERS
char * get_ip_ram();
int get_puerto_ram();

int get_puerto_escucha();

t_algoritmo get_algoritmo();

int get_estimacion_inicial();
int get_alfa();

int get_cantidad_dispositivos_io();
char ** get_dispositivos_io();

int get_cantidad_duraciones_io();
char ** get_duraciones_io();

int get_retardo_cpu();

int get_grado_multiprogramacion();
int get_grado_multiprocesamiento();

char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();

typedef struct {
	char * ip_ram;
	int puerto_ram;

	int puerto_escucha;

	t_algoritmo algoritmo;

	int estimacion_inicial;
	int alfa;

	int cantidad_dispositivos_io;
	char ** dispositivos_io;

	int cantidad_duraciones_io;
	char ** duraciones_io;

	int retardo_cpu;

	int grado_multiprogramacion;
	int grado_multiprocesamiento;

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
