#include "filesystem_configuracion.h"



// Declaracion de funciones privadas
int set_variable_str(t_config * config, char * param_leer, char ** param); // Se usa para setear un string leyendo del archivo config

int set_variable_int(t_config * config, char * param_leer, int * param); // Se usa para setear un int leyendo del archivo config

int cargar_archivo(); // carga en config guardada la info que se adquire del .config

int set_variable_array_str(t_config * config, char * param_leer, char *** param, int * cantidad_elementos);

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
	free(config_guardada.ip);
	for (int i = 0; i < get_cantidad_archivos(); ++i) { //ESTO PUEDE SER QUE ESTE MEDIO RANCIO PERO COMO NO VA A HABER 1 MILLON DE PARTICIONES EN LA PRUEBA ENTIENDO QUE ESTA BIEN.
		free(config_guardada.archivos_swap[i]);
		}
	free(config_guardada.archivos_swap);
	free(config_guardada.log_route);
	free(config_guardada.log_app_name);
}

int cargar_archivo(char * path) {
	t_config * config = config_create(path);
	int error = 0;

	loggear_trace("cargo la config");
	// Lectura del archivo config
	error += set_variable_str(config, "IP", 					&config_guardada.ip);
	error += set_variable_int(config, "PUERTO", 		        &config_guardada.puerto);
	error += set_variable_int(config, "TAMANIO_SWAP", 		    &config_guardada.tamanio_swap);
	error += set_variable_int(config, "TAMANIO_PAGINA", 		&config_guardada.tamanio_pagina);
	error += set_variable_array_str(config, "ARCHIVOS_SWAP", 	&config_guardada.archivos_swap, &config_guardada.cantidad_archivos);
	error += set_variable_int(config, "MARCOS_MAXIMOS", 		&config_guardada.marcos_maximos);
	error += set_variable_int(config, "RETARDO_SWAP", 		    &config_guardada.retardo_swap);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 				&config_guardada.log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 			&config_guardada.log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 		&config_guardada.log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 		&config_guardada.log_level_info);

	if (error != 0) {
		return -2;
	}

	//para probar
	lista_swamp = list_create();
	for(int j = 0; j < config_guardada.cantidad_archivos; j ++){
		t_archivo_swamp * archivo = malloc(sizeof(t_archivo_swamp));
		archivo->carpinchos = list_create();
		archivo->ruta_archivo = config_guardada.archivos_swap[j];
		archivo->espacio_libre = config_guardada.tamanio_swap;
		list_add(lista_swamp, archivo);
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

