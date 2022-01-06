#include "configuracion.h"

t_algoritmo obtener_algoritmo(char * algoritmo);

int set_variable_str(t_config * config, char * param_leer, char ** param);

int set_variable_int(t_config * config, char * param_leer, int * param);

int set_variable_array_str(t_config * config, char * param_leer, char *** param, int * cantidad_elementos);

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
	free(config_guardada.ip_ram);
	free(config_guardada.log_app_name);
	free(config_guardada.log_route);

	for (int i = 0; i < config_guardada.cantidad_dispositivos_io; ++i) { free(config_guardada.dispositivos_io[i]); }
	for (int i = 0; i < config_guardada.cantidad_duraciones_io; ++i) { free(config_guardada.duraciones_io[i]); }

	free(config_guardada.dispositivos_io);
	free(config_guardada.duraciones_io);
}

int cargar_archivo(char * path) {
	t_config * config = config_create(path);
	int error = 0;

	error += set_variable_str(config, "IP_MEMORIA", 			&config_guardada.ip_ram);
	error += set_variable_int(config, "PUERTO_MEMORIA", 		&config_guardada.puerto_ram);

	error += set_variable_int(config, "PUERTO_ESCUCHA", 		&config_guardada.puerto_escucha);

	char * algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	config_guardada.algoritmo = obtener_algoritmo(algoritmo_planificacion);
	if (config_guardada.algoritmo < 0) {
		return -1;
	}

	error += set_variable_int(config, "ESTIMACION_INICIAL", 	&config_guardada.estimacion_inicial);
	error += set_variable_int(config, "ALFA", 					&config_guardada.alfa);

	error += set_variable_array_str(config, "DISPOSITIVOS_IO", 	&config_guardada.dispositivos_io, &config_guardada.cantidad_dispositivos_io);
	error += set_variable_array_str(config, "DURACIONES_IO", 	&config_guardada.duraciones_io, &config_guardada.cantidad_duraciones_io);

	error += set_variable_int(config,"GRADO_MULTIPROGRAMACION",	&config_guardada.grado_multiprogramacion);
	error += set_variable_int(config,"GRADO_MULTIPROCESAMIENTO",&config_guardada.grado_multiprocesamiento);

	error += set_variable_int(config, "TIEMPO_DEADLOCK",		&config_guardada.tiempo_deadlock);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 				&config_guardada.log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 			&config_guardada.log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 		&config_guardada.log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 		&config_guardada.log_level_info);

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

int set_variable_array_str(t_config * config, char * param_leer, char *** param, int * cantidad_elementos) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	*param = config_get_array_value(config, param_leer);

	char * ultimo_elemento = (*param)[0];
	*cantidad_elementos = 0;
	while(ultimo_elemento != NULL) {
		(*cantidad_elementos)++;
		ultimo_elemento = (*param)[*cantidad_elementos];
	}

	return 0;
}

