#include "filesystem_main.h"

void cerrar_todo();
void manejar_signal(int n);
void debug_variables();
void destroy_variables();
void destruir_archivo_swamp(t_archivo_swamp* swamp);
void destruir_string(void* el_String);

int main(int argc, char** argv) {
	signal(SIGUSR1, manejar_signal);

	int error = iniciar_configuracion(argc, argv);
	if (error != STATUS_OK) {
		puts("Error en los argumentos\n");
		return EXIT_FAILURE;
	}

	error = init_mutex_log(get_log_route(), get_log_app_name(), get_log_in_console(), get_log_level_info());
	if (error != STATUS_OK) {
		puts("Error al crear el logger\n");
		cerrar_todo();
		return EXIT_FAILURE;
	}

	iniciar_swamp();

	/*loggear_trace("vamos dale");
	t_archivo_swamp * mostrar = malloc(sizeof(t_archivo_swamp));
	t_archivo_swamp * mostrar2 = malloc(sizeof(t_archivo_swamp));
	mostrar = list_get(lista_swamp, 1);

	mostrar2 = list_get(lista_swamp, 0);

	loggear_trace("EL PRIMERO ES %s", mostrar->ruta_archivo);

	loggear_trace("EL SEGUNDO ES %s", mostrar->ruta_archivo);

	list_add(mostrar->carpinchos, "proceso");
	list_add(mostrar2->carpinchos, "procesa");
	list_add(mostrar2->carpinchos, "procesa2");

	loggear_info("EL PRIMERO PROCESOS %s", list_get(mostrar->carpinchos, 0));
	loggear_info("EL SEGUNDO PROCESOS %s", list_get(mostrar2->carpinchos, 1));

*/
	debug_variables();

	/*pthread_t ram_handshake = thread_ejecutar_funcion(ram_enviar_handshake);


	error = thread_join_and_free(ram_handshake);
	if (error != 0) {
		loggear_error("RAM no nos acepto, Error: %d", error);
		cerrar_todo();
		thread_detach_and_free(ram_handshake);
		return EXIT_FAILURE;
	}*/



	cerrar_todo();

	return EXIT_SUCCESS;
}

void cerrar_todo() {
	destroy_configuracion();
	destroy_log();
	destroy_variables();
}

void destroy_variables(){
	for(int i = 0; i < list_size(lista_swamp); i++){
		destruir_archivo_swamp(list_get(lista_swamp, i));
	}

	list_destroy(lista_swamp);
}

void destruir_archivo_swamp(t_archivo_swamp* swamp){ //TODO ver luego donde conviene moverlo.
	list_clean_and_destroy_elements(swamp->carpinchos, destruir_string);
	list_destroy(swamp->carpinchos);
	free(swamp);
	//free(swamp->ruta_archivo);
}

void destruir_string(void* el_String){
	free(el_String);
}

void manejar_signal(int n){
	loggear_warning("Llego la signal para mandar sabotaje, TODO: Cambiar la funcion en filesystem_main.c:35 para que cumpla la funcion");
}

void debug_variables() {
	loggear_debug("IP: %s", get_ip());
	loggear_debug("PUERTO: %d", get_puerto());
	loggear_debug("TAMANIO_SWAP: %d", get_tamanio_swap());
	loggear_debug("TAMANIO_PAGINA: %d", get_tamanio_pagina());
	loggear_debug("MARCOS_MAXIMOS: %d", get_marcos_maximos());
	loggear_debug("RETARDO_SWAP: %d", get_retardo_swap());
	loggear_debug("CANTIDAD_ARCHIVOS: %d", get_cantidad_archivos());
	for (int i = 0; i < get_cantidad_archivos(); ++i) {
		loggear_debug("ARCHIVOS_SWAP[%d]: \t\t%s", i, get_archivos_swap()[i]);
	}
}
