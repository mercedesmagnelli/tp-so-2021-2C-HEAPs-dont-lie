#include "ram_configuracion.h"

int set_variable_enum(t_config * config, char * param_leer, int * param, int (*transformar)(char* variable));

int set_variable_int(t_config * config, char * param_leer, int * param);

int set_variable_str(t_config * config, char * param_leer, char ** param);


int obtener_algoritmo_reemplazo_mmu(char * algoritmo);
int obtener_tipo_asignacion(char * tipo_asignacion);
int obtener_algoritmo_reemplazo_tlb(char * tipo_asignacion);

int cargar_archivo();

// Publica
int iniciar_configuracion(int argc, char ** argv) {

	const bool es_config_valida = son_validos_los_parametros(argc, argv);

	const char * path_config = argv[1];

	if (!es_config_valida) {
		return CONFIG_ARGUMENTOS_INVALIDOS;
	}

	int error = cargar_archivo(path_config);
	if (error != 0) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	return 0;
}

// Publica
bool son_validos_los_parametros(int argc, char ** argv) {
	return argc >= 2 && string_ends_with(argv[1], ".config");
}

// Publica
void destroy_configuracion() {
	free(config_guardada.log_route);
	free(config_guardada.log_app_name);
}

int cargar_archivo(char * path) {
	t_config * config = config_create(path);
	int error = 0;

	error += set_variable_int(config, "PUERTO", 				&config_guardada.puerto);
	error += set_variable_int(config, "TAMANIO", 				&config_guardada.tamanio_memoria);
	error += set_variable_int(config, "TAMANIO_PAGINA",			&config_guardada.tamanio_pagina);

	error += set_variable_enum(config, "ALGORITMO_REEMPLAZO_MMU", 	&config_guardada.algoritmo_reemplazo_mmu, obtener_algoritmo_reemplazo_mmu);
	error += set_variable_enum(config, "TIPO_ASIGNACION", 			&config_guardada.tipo_asignacion, obtener_tipo_asignacion);
	error += set_variable_enum(config, "ALGORITMO_REEMPLAZO_TLB",	&config_guardada.algoritmo_reemplazo_tlb, obtener_algoritmo_reemplazo_tlb);

	error += set_variable_int(config, "MARCOS_MAXIMOS", 		&config_guardada.marcos_maximos);
	error += set_variable_int(config, "CANTIDAD_ENTRADAS_TLB",	&config_guardada.cantidad_entradas_tlb);
	error += set_variable_int(config, "RETARDO_ACIERTO_TLB", 	&config_guardada.retardo_acierto_tlb);
	error += set_variable_int(config, "RETARDO_FALLO_TLB", 		&config_guardada.retardo_fallo_tlb);
	error += set_variable_str(config, "PATH_DUMP_TLB",			&config_guardada.path_dump_tlb);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 				&config_guardada.log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 			&config_guardada.log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 		&config_guardada.log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 		&config_guardada.log_level_info);

	if (error != 0) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	config_destroy(config);

	return 0;
}



int set_variable_int(t_config * config, char * param_leer, int * param) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	*param = config_get_int_value(config, param_leer);

	return 0;
}

int set_variable_str(t_config * config, char * param_leer, char ** param) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	char * variable_auxiliar = config_get_string_value(config, param_leer);

	*param = malloc(sizeof(char) * (string_length(variable_auxiliar) + 1));

	strcpy( *param, variable_auxiliar );

	return 0;
}

int set_variable_enum(t_config * config, char * param_leer, int * param, int (*transformar)(char* variable)) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	char * parametro_guardar;
	int error = set_variable_str(config, param_leer, &parametro_guardar);
	if (error < 0) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	int resultado = transformar(parametro_guardar);

	if (resultado < 0) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	*param = resultado;

	return 0;
}

int obtener_algoritmo_reemplazo_mmu(char * algoritmo) {
	if (strcmp(algoritmo, ALGORITMO_REEMPLAZO_MMU_CLOCK_M) == 0) {
		return (int) CLOCKM;
	}else if(strcmp(algoritmo, ALGORITMO_REEMPLAZO_MMU_LRU) == 0) {
		return (int) LRUM;
	}

	return (int) CONFIG_ERROR_EN_ARCHIVO;
}

int obtener_tipo_asignacion(char * tipo_asignacion) {
	if (strcmp(tipo_asignacion, TIPO_ASIGNACION_FIJA) == 0) {
		return (int) FIJA;
	} else if (strcmp(tipo_asignacion, TIPO_ASIGNACION_DINAMICA) == 0) {
		return (int) DINAMICA;
	}

	return (int) CONFIG_ERROR_EN_ARCHIVO;
}

int obtener_algoritmo_reemplazo_tlb(char * tipo_asignacion) {
	if (strcmp(tipo_asignacion, ALGORITMO_REEMPLAZO_TLB_FIFO) == 0) {
		return (int) FIFO;
	} else if (strcmp(tipo_asignacion, ALGORITMO_REEMPLAZO_TLB_LRU) == 0) {
		return (int) LRU;
	}

	return (int) CONFIG_ERROR_EN_ARCHIVO;
}

