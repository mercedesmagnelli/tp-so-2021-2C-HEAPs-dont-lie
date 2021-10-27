
#include "ram_config_guardada.h"

int get_puerto() {return config_guardada.puerto; }
int get_tamanio_memoria() { return config_guardada.tamanio_memoria; }
int get_tamanio_pagina() {return config_guardada.tamanio_pagina;}

t_algoritmo_reemplazo_mmu get_algoritmo_reemplazo_mmu(){ return config_guardada.algoritmo_reemplazo_mmu; }
t_tipo_asignacion get_tipo_asignacion(){ return config_guardada.tipo_asignacion;}
t_algoritmo_reemplazo_tlb get_algoritmo_reemplazo_tlb(){ return config_guardada.algoritmo_reemplazo_tlb;}

int get_marcos_maximos(){ return config_guardada.marcos_maximos;};
int get_cantidad_entradas_tlb(){ return config_guardada.cantidad_entradas_tlb;};
int get_retardo_acierto_tlb(){ return config_guardada.retardo_acierto_tlb;};
int get_retardo_fallo_tlb(){ return config_guardada.retardo_fallo_tlb;};

char * get_path_dump_tlb(){return config_guardada.path_dump_tlb;}

char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }


