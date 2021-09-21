
#include "configuracion_guardada.h"

char * get_ip_ram() { return config_guardada.ip_ram; }
int get_puerto_ram() { return config_guardada.puerto_ram; }

t_algoritmo get_algoritmo() { return config_guardada.algoritmo; }

int get_cantidad_dispositivos_io() { return config_guardada.cantidad_dispositivos_io; }
char ** get_dispositivos_io() { return config_guardada.dispositivos_io; }

int get_cantidad_duraciones_io() { return config_guardada.cantidad_duraciones_io; }
char ** get_duraciones_io() { return config_guardada.duraciones_io; }

int get_retardo_cpu() { return config_guardada.retardo_cpu; }

int get_grado_multiprogramacion() { return config_guardada.grado_multiprogramacion; }
int get_grado_multiprocesamiento() { return config_guardada.grado_multiprocesamiento; }


char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }
