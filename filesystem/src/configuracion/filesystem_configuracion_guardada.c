
#include "filesystem_configuracion_guardada.h"

// GETTERS//
char * get_punto_montaje() {return config_guardada.punto_montaje; }
int get_puerto() { return config_guardada.puerto; }
int get_tiempo_sincronizacion() { return config_guardada.tiempo_sincronizacion; }
char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }
int get_cantidad_posiciones(){ return config_guardada.cantidad_posiciones; }
int get_cantidad_bloques(){ return config_guardada.cantidad_bloques; }
int get_tamanio_bloques(){ return config_guardada.tamanio_bloques; }
char* get_posicion_sabotaje(int i){return list_get(config_guardada.posiciones_sabotaje, i);}

