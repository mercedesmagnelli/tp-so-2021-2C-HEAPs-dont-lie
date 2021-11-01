#include "filesystem_main.h"

void cerrar_todo();
void manejar_signal(int n);
void debug_variables();


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

	escribir_particion(9, 11, "gola don pepito9", particion_a_escribir(9));
	escribir_particion(9, 13, "gola don pepito9", particion_a_escribir(9));
	escribir_particion(10, 13, "gola don pepito9", particion_a_escribir(10));


/*
 * SIRVE PARA MOSTRAR EL BITMAP DE LA PARTICION TENERLO POR AHORA
	t_archivo_swamp* asd = list_get(lista_swamp, 0);

	for(int w = 0; w < get_cantidad_paginas(); w++){
		if(bitarray_test_bit(asd->bitmap_bitarray, w)){
			loggear_trace("1");
		}else{
			loggear_trace("0");
		}
	}
*/
/*
		escribir_particion(9, 11, "gola don pepito9", particion_a_escribir(9)); // 1
	escribir_particion(10, 15, "gola don pepito10", particion_a_escribir(10));//2
*/
	/*NO LO BORREN LO USO PARA PRUEBAS
	 * loggear_trace("vamos dale");
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
		t_archivo_swamp* carp = list_get(lista_swamp, 0);
	t_archivo_swamp* carp2 = list_get(lista_swamp, 1);

	loggear_error("EL carpincho qeu se escribio es %s", list_get(carp->carpinchos, 0));
	loggear_error("EL carpincho qeu se escribio es %s", list_get(carp->carpinchos, 1));
	loggear_error("EL carpincho qeu se escribio es %s", list_get(carp2->carpinchos, 0));
	loggear_error("EL carpincho qeu se escribio es %s", list_get(carp2->carpinchos, 1));

*/
	debug_variables();


/*	pthread_t ram_handshake = thread_ejecutar_funcion(enviar_handshake);


	error = thread_join_and_free(ram_handshake);
	if (error != 0) {
		loggear_error("RAM no nos acepto, Error: %d", error);
		cerrar_todo();
		thread_detach_and_free(ram_handshake);
		return EXIT_FAILURE;
	}

*/

	cerrar_todo();

	return EXIT_SUCCESS;
}

void cerrar_todo() {
	destroy_configuracion();
	destroy_log();
	destroy_lista_swamp();
	destroy_lista_carpinchos();
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
	loggear_debug("CANTIDAD_MARCOS: %d", get_cantidad_paginas());
	loggear_debug("CANTIDAD_ARCHIVOS: %d", get_cantidad_archivos());
	for (int i = 0; i < get_cantidad_archivos(); ++i) {
		loggear_debug("ARCHIVOS_SWAP[%d]: \t\t%s", i, get_archivos_swap()[i]);
	}
}
