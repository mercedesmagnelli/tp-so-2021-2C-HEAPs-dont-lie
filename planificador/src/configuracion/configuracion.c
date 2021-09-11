#include "configuracion.h"

t_algoritmo obtener_algoritmo(char * algoritmo);

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
	free(config_guardada.ip_ram);
	free(config_guardada.ip_filesystem);
	free(config_guardada.log_app_name);
	free(config_guardada.log_route);
}

int cargar_archivo(char * path) {
	t_config * config = config_create(path);
	int error = 0;

	error += set_variable_str(config, "IP_MI_RAM_HQ", 			&config_guardada.ip_ram);
	error += set_variable_int(config, "PUERTO_MI_RAM_HQ", 		&config_guardada.puerto_ram);
	error += set_variable_str(config, "IP_I_FILESYSTEM_STORE", 		&config_guardada.ip_filesystem);
	error += set_variable_int(config, "PUERTO_I_FILESYSTEM_STORE", 	&config_guardada.puerto_filesystem);
	error += set_variable_int(config, "GRADO_MULTITAREA", 		&config_guardada.grado_multitarea);
	error += set_variable_int(config, "QUANTUM", 				&config_guardada.quantum);
	error += set_variable_int(config, "DURACION_SABOTAJE", 		&config_guardada.duracion_sabotaje);
	error += set_variable_int(config, "RETARDO_CICLO_CPU", 		&config_guardada.retardo_ciclo_cpu);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 				&config_guardada.log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 			&config_guardada.log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 		&config_guardada.log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 		&config_guardada.log_level_info);

	if (error != 0) { // TODO: Traer codigo de error
		return -2;
	}

	char * algoritmo_planificacion = config_get_string_value(config, "ALGORITMO");
	config_guardada.algoritmo = obtener_algoritmo(algoritmo_planificacion);

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

t_algoritmo obtener_algoritmo(char * algoritmo) {
	if (strcmp(algoritmo, ROUND_ROBIN) == 0 && config_guardada.quantum>0) {
		return RR;
	}

	return FIFO;
}

