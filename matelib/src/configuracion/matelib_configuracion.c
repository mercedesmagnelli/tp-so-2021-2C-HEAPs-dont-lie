#include "matelib_configuracion.h"

// Declaracion de funciones privadas
int set_variable_str(t_config * config, char * param_leer, char ** param); // Se usa para setear un string leyendo del archivo config

int set_variable_int(t_config * config, char * param_leer, int * param); // Se usa para setear un int leyendo del archivo config

int cargar_archivo(t_instance_metadata * lib_ref, char * path); // carga en config guardada la info que se adquire del .config



int cargar_archivo(t_instance_metadata * lib_ref, char * path) {
	t_config * config = config_create(path);
	int error = 0;

	// Lectura del archivo config
	error += set_variable_str(config, "IP", 					&lib_ref->ip);
	error += set_variable_int(config, "PUERTO", 		        &lib_ref->port);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 				&lib_ref->log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 			&lib_ref->log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 		&lib_ref->log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 		&lib_ref->log_level_info);

	if (error != 0) {
		return -2;
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

