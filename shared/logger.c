
#include "logger.h"

int loggear(const char * mensaje, void (*log_function)(t_log * logg, const char* message, ...), va_list params);

pthread_mutex_t mutex_log;
t_log* logger = NULL;

// Publica
int init_mutex_log(char* archivo, char* nombre_programa, int activar_consola, t_log_level level) {
	if (logger != NULL) {
		// TODO: Loguear en modo debug
		return RECURSO_YA_DECLARADO;
	}
	pthread_mutex_init(&mutex_log, NULL);
	logger = log_create(archivo, nombre_programa, activar_consola, level);

	return STATUS_OK;
}

// Publica
int loggear_info(const char* mensaje, ...) {
	va_list arg;
	va_start(arg, mensaje);

	int error = loggear(mensaje, log_info, arg);

	va_end(arg);

	return error;
}

// Publica
int loggear_debug(const char* mensaje, ...){
	va_list arg;
	va_start(arg, mensaje);

	int error = loggear(mensaje, log_debug, arg);

	va_end(arg);

	return error;
}

// Publica
int loggear_trace(const char* mensaje, ...){
	va_list arg;
	va_start(arg, mensaje);

	int error = loggear(mensaje, log_trace, arg);

	va_end(arg);

	return error;
}

// Publica
int loggear_warning(const char* mensaje, ...){
	va_list arg;
	va_start(arg, mensaje);

	int error = loggear(mensaje, log_warning, arg);

	va_end(arg);

	return error;
}

// Publica
int loggear_error(const char* mensaje, ...){
	va_list arg;
	va_start(arg, mensaje);

	int error = loggear(mensaje, log_error, arg);

	va_end(arg);

	return error;
}

int loggear(const char * mensaje, void (*log_function)(t_log * logg, const char* message, ...), va_list arguments) {
	if (logger == NULL) {
		// TODO: Loguear en modo debug
		return RECURSO_NO_DECLARADO;
	}

	char* mensaje_armado = string_from_vformat(mensaje, arguments);

	pthread_mutex_lock(&mutex_log);
	log_function(logger, mensaje_armado);
	pthread_mutex_unlock(&mutex_log);

	free(mensaje_armado);

	return STATUS_OK;
}

int destroy_log() {
	if (logger == NULL) {
		// TODO: Loguear en modo debug
		return RECURSO_NO_DECLARADO;
	}

	log_destroy(logger);
	pthread_mutex_destroy(&mutex_log);

	return STATUS_OK;
}

