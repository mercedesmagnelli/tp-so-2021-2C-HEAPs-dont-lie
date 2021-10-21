
#include "matelib_configuracion_guardada.h"

// GETTERS//
char * get_ip() {return config_guardada.ip; }
int get_puerto() { return config_guardada.puerto; }

char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }
