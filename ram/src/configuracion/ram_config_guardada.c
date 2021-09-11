
#include "ram_config_guardada.h"

int get_tamanio_memoria() { return config_guardada.tamanio_memoria; }
t_esquema get_esquema_memoria() { return config_guardada.esquema_memoria; }
int get_tamanio_pagina() { return config_guardada.tamanio_pagina; }
int get_tamanio_swap() { return config_guardada.tamanio_swap; }
char * get_path_swap() { return config_guardada.path_swap; }
t_algoritmo_ram get_algoritmo() { return config_guardada.algoritmo; }
int get_puerto() {return config_guardada.puerto; }
t_criterio get_criterio() {return config_guardada.criterio; }

char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }


