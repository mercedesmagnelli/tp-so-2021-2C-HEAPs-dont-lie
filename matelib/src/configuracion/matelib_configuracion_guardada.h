
#ifndef CONFIGURACION_IMATELIB_CONFIG_GUARDADA_H_
#define CONFIGURACION_IMATELIB_CONFIG_GUARDADA_H_

#include <commons/collections/list.h>

typedef struct{
	int pos_x;
	int pos_y;
}t_posicion;

typedef struct {
	char * ip;
	int puerto;

	// Loggin
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;

} t_info_config;


// GETTERS
char * get_ip();
int get_puerto();

char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();

/**
 * @NAME: config_guardada
 * @DESC: Contiene toda la informacion leida del archivo de configuracion del i-matelib-store
 */
t_info_config config_guardada;

#endif /* CONFIGURACION_IMATELIB_CONFIG_GUARDADA_H_ */
