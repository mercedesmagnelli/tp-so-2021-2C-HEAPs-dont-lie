
#ifndef CONFIGURACION_RAM_CONFIG_GUARDADA_H_
#define CONFIGURACION_RAM_CONFIG_GUARDADA_H_

#define ALGORITMO_REEMPLAZO_MMU_CLOCK_M "CLOCK-M"

#define TIPO_ASIGNACION_FIJA "FIJA"
#define TIPO_ASIGNACION_DINAMICA "DINAMICA"

#define ALGORITMO_REEMPLAZO_TLB_FIFO "FIFO"
#define ALGORITMO_REEMPLAZO_TLB_LRU "LRU"

typedef enum {
	FIJA,
	DINAMICA
} t_tipo_asignacion;

typedef enum {
	CLOCKM
} t_algoritmo_reemplazo_mmu;

typedef enum {
	FIFO,
	LRU
} t_algoritmo_reemplazo_tlb;

// GETTERS
int get_puerto();
int get_tamanio_memoria();
int get_tamanio_pagina();

t_algoritmo_reemplazo_mmu get_algoritmo_reemplazo_mmu();
t_tipo_asignacion get_tipo_asignacion();
t_algoritmo_reemplazo_tlb get_algoritmo_reemplazo_tlb();

int get_marcos_maximos();
int get_cantidad_entradas_tlb();
int get_retardo_acierto_tlb();
int get_retardo_fallo_tlb();

char * get_path_dump_tlb();

char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();

typedef struct {
	int puerto;
	int tamanio_memoria;
	int tamanio_pagina;

	int algoritmo_reemplazo_mmu;
	int tipo_asignacion;
	int algoritmo_reemplazo_tlb;

	int marcos_maximos;
	int cantidad_entradas_tlb;
	int retardo_acierto_tlb;
	int retardo_fallo_tlb;

	char * path_dump_tlb;

	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;

} t_info_config;

/**
 * @NAME: config_guardada
 * @DESC: Contiene toda la informacion leida del archivo de configuracion de la ram
 */
t_info_config config_guardada;

#endif /* CONFIGURACION_RAM_CONFIG_GUARDADA_H_ */
