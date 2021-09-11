#include "ram_configuracion.h"

t_algoritmo_ram obtener_algoritmo(char * algoritmo);

t_esquema obtener_esquema(char * esquema);

t_criterio obtener_criterio(char * criterio);

int set_variable_str(t_config * config, char * param_leer, char ** param);

int set_variable_int(t_config * config, char * param_leer, int * param);

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
	free(config_guardada.path_swap);
	free(config_guardada.log_route);
	free(config_guardada.log_app_name);
}

int cargar_archivo(char * path) {
	t_config * config = config_create(path);
	int error = 0;


	error += set_variable_str(config, "PATH_SWAP", 				&config_guardada.path_swap);
	error += set_variable_int(config, "PUERTO", 				&config_guardada.puerto);
	error += set_variable_int(config, "TAMANIO_MEMORIA", 		&config_guardada.tamanio_memoria);
	error += set_variable_int(config, "TAMANIO_PAGINA", 		&config_guardada.tamanio_pagina);
	error += set_variable_int(config, "TAMANIO_SWAP", 			&config_guardada.tamanio_swap);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 				&config_guardada.log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 			&config_guardada.log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 		&config_guardada.log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 		&config_guardada.log_level_info);


	if (error != 0) { // TODO: Traer codigo de error
		return -2;
	}

	char * algoritmo_reemplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	config_guardada.algoritmo = obtener_algoritmo(algoritmo_reemplazo);

	char * esquema_memoria = config_get_string_value(config, "ESQUEMA_MEMORIA");
	config_guardada.esquema_memoria = obtener_esquema(esquema_memoria);

	char * criterio_seleccion = config_get_string_value(config, "CRITERIO_SELECCION");
	config_guardada.criterio = obtener_criterio(criterio_seleccion);

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

t_algoritmo_ram obtener_algoritmo(char * algoritmo) {
	if (strcmp(algoritmo, LEAST_RECENTLY_USED) == 0) {
		return LRU;
	}

	return CLOCK;
}

t_esquema obtener_esquema(char * esquema) {
	if (strcmp(esquema, ESQUEMA_PAGINACION) == 0) {
		return PAGINACION;
	}

	return SEGMENTACION;
}

t_criterio obtener_criterio(char * criterio) {
	if (strcmp(criterio, CRITERIO_BEST_FIT) == 0) {
		return BEST_FIT;
	}

	return FIRST_FIT;
}
