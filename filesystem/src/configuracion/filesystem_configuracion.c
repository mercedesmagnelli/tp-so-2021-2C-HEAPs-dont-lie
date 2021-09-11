#include "filesystem_configuracion.h"

// Declaracion de funciones privadas
int set_variable_str(t_config * config, char * param_leer, char ** param); // Se usa para setear un string leyendo del archivo config

int set_variable_int(t_config * config, char * param_leer, int * param); // Se usa para setear un int leyendo del archivo config

int cargar_archivo(); // carga en config guardada la info que se adquire del .config

int set_variable_list_config(t_config * config, char * param_leer, t_list** param);

void liberar_strings(void* elString);


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
	free(config_guardada.punto_montaje);
	list_destroy_and_destroy_elements(config_guardada.posiciones_sabotaje,liberar_strings);
	free(config_guardada.log_route);
	free(config_guardada.log_app_name);
}

int cargar_archivo(char * path) {
	t_config * config = config_create(path);
	int error = 0;

	error += set_variable_str(config, "PUNTO_MONTAJE", 			&config_guardada.punto_montaje);
	error += set_variable_int(config, "PUERTO", 		        &config_guardada.puerto);
	config_guardada.posiciones_sabotaje = list_create();
	list_add(config_guardada.posiciones_sabotaje, NULL);
	error += set_variable_list_config(config, "POSICIONES_SABOTAJE",	&config_guardada.posiciones_sabotaje);
	error += set_variable_int(config, "TIEMPO_SINCRONIZACION", 	&config_guardada.tiempo_sincronizacion);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 				&config_guardada.log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 			&config_guardada.log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 		&config_guardada.log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 		&config_guardada.log_level_info);

	//Para iniciar los bloques y sus tamaños en el FS
	error += set_variable_int(config, "CANTIDAD_BLOQUES", &config_guardada.cantidad_bloques);
	error += set_variable_int(config, "TAMANIO_BLOQUES", &config_guardada.tamanio_bloques);

	if (error != 0) {
		return -2;
	}

	config_destroy(config);

	return 0;
}

void liberar_strings(void* elString){
	free(elString);
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

int set_variable_list_config(t_config * config, char * param_leer, t_list** param) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	char** variable = config_get_array_value(config, param_leer);

	int i = 0;

	if(variable[i] == NULL){
		return 0;
	}else{

	list_replace(*param, 0, variable[i]);
	i++;
		while(variable[i] != NULL){
			list_add(*param, variable[i]);
			i++;
		}

	}

	free(variable);

	return 0;
}

