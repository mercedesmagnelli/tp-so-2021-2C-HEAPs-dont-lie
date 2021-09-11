
#ifndef CONFIGURACION_IFILESYSTEM_CONFIG_GUARDADA_H_
#define CONFIGURACION_IFILESYSTEM_CONFIG_GUARDADA_H_

// STRUCTS
#include <commons/collections/list.h>
typedef struct{
	int pos_x;
	int pos_y;
}t_posicion;

typedef struct {
	char * punto_montaje;
	int puerto;

	int tiempo_sincronizacion;


	// Loggin
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;

	int cantidad_posiciones;
	t_list* posiciones_sabotaje;

	//inicializacion FS
	int cantidad_bloques;
	int tamanio_bloques;

} t_info_config;


// GETTERS
char * get_punto_montaje();
int get_puerto();

int get_tiempo_sincronizacion();
char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();
int get_cantidad_posiciones();
int get_cantidad_bloques();
int get_tamanio_bloques();
char* get_posicion_sabotaje(int i);


/**
 * @NAME: config_guardada
 * @DESC: Contiene toda la informacion leida del archivo de configuracion del i-filesystem-store
 */
t_info_config config_guardada;

#endif /* CONFIGURACION_IFILESYSTEM_CONFIG_GUARDADA_H_ */
