
#include "configuracion_guardada.h"

char * get_ip_ram() { return config_guardada.ip_ram; }
int get_puerto_ram() { return config_guardada.puerto_ram; }

char * get_ip_filesystem() { return config_guardada.ip_filesystem; }
int get_puerto_filesystem() { return config_guardada.puerto_filesystem; }

int get_grado_multitarea() { return config_guardada.grado_multitarea; }
t_algoritmo get_algoritmo() { return config_guardada.algoritmo; }
int get_quantum() { return config_guardada.quantum; }
int get_duracion_sabotaje() { return config_guardada.duracion_sabotaje; }
int get_retardo_ciclo_cpu() { return config_guardada.retardo_ciclo_cpu; }

char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }
