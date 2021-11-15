
#include "filesystem_configuracion_guardada.h"

// GETTERS//
char * get_ip() {return config_guardada.ip; }
int get_puerto() { return config_guardada.puerto; }
int get_tamanio_swap() { return config_guardada.tamanio_swap; }
char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }
int get_tamanio_pagina(){ return config_guardada.tamanio_pagina; }
int get_marcos_maximos(){ return config_guardada.marcos_maximos; }
int get_retardo_swap(){ return config_guardada.retardo_swap; }
int get_cantidad_archivos() { return config_guardada.cantidad_archivos; }
char ** get_archivos_swap() { return config_guardada.archivos_swap; }
int get_cantidad_marcos(){ return config_guardada.tamanio_swap / config_guardada.tamanio_pagina; }
t_asignacion get_asignacion() { return config_guardada.asignacion;}
