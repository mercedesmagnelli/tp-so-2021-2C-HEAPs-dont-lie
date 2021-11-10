#include "filesystem_main.h"

void cerrar_todo();
void manejar_signal(int n);
void debug_variables();

/*PARA ACORDARME CONTINUAR CON LO DE DUPLA AL ESCRIBIR PARA LUEGO HACER LO MISMO PARA LA LECTURA
 *HACER EL CONTROL PARA EL CASO QUE NO HAY MAS ESPACIO
 *
 *
 */

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

// ULTIMAS PRUEBAS REALIZADAS
/*   t_carpincho_swamp* carpincho = crear_carpincho(14,5); // 1 	5
    t_carpincho_swamp* carpincho1 = crear_carpincho(13,4); // 2 4
    t_carpincho_swamp* carpincho2 = crear_carpincho(11,12); // 2 16
    t_carpincho_swamp* carpincho3 = crear_carpincho(10,9); // 1 13
    t_carpincho_swamp* carpincho4 = crear_carpincho(19,5); // 1 18

    loggear_error("%d", carpincho->pid_carpincho);
    loggear_error("%d", carpincho1->pid_carpincho);
    loggear_error("%d", carpincho2->pid_carpincho);
    loggear_error("%d", carpincho3->pid_carpincho);
    loggear_error("%d", carpincho4->pid_carpincho);

    reservar_marcos(carpincho,7, particion_a_escribir(carpincho->pid_carpincho));

    t_archivo_swamp* archivo1 = list_get(lista_swamp, 0);
    t_archivo_swamp* archivo2 = list_get(lista_swamp, 1);
    char* carpincho1_str = list_get(archivo1->carpinchos, 0);
    char* carpincho2_str = list_get(archivo1->carpinchos, 1);
    char* carpincho3_str = list_get(archivo1->carpinchos, 2);

    char* carpincho4_str = list_get(archivo2->carpinchos, 0);
    char* carpincho5_str = list_get(archivo2->carpinchos, 1);

    loggear_warning("espacio libre archivo 1 %d", archivo1->espacio_libre);
    loggear_warning("espacio libre archivo 2 %d", archivo2->espacio_libre);

    loggear_warning("carpincho 1 %s", carpincho1_str);
    loggear_warning("carpincho 2 %s", carpincho2_str);
    loggear_warning("carpincho 3 %s", carpincho3_str);
    loggear_warning("carpincho 4 %s", carpincho4_str);
    loggear_warning("carpincho 5 %s", carpincho5_str);

    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,0));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,1));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,2));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,3));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,4));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,5));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,6));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,7));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,8));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,9));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,10));
    loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,11));
*/



	/*reservar_marcos(carpincho,3, archivo_a_escribir(carpincho->pid_carpincho));
	reservar_marcos(carpincho,7, archivo_a_escribir(carpincho->pid_carpincho));

	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 0) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 1) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 2) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 3) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 4) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 5) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 6) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 7) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 8) );
	loggear_warning("marco reservado posicion %s",list_get(carpincho->marcos_reservados, 9) );

	reservar_marcos(carpincho,1, archivo_a_escribir(carpincho->pid_carpincho));
*/

// MI SUPER MAIN IMPORTANTE NO BORRAR, LAMENTO SUBIR ESTO IGNORALO POR EL MOMENTO ALEX, PERO ME AYUDA MUCHO A PROBAR. PROMETO BORRARLO APENAS TERMINE//
/*
	t_carpincho_swamp* carpincho = crear_carpincho(14);
	t_carpincho_swamp* carpincho2 = crear_carpincho(12);
	t_carpincho_swamp* carpincho3 = crear_carpincho(14);

	escribir_particion(carpincho, 11, "gola don pepito9", particion_a_escribir(carpincho->pid_carpincho));
	escribir_particion(carpincho, 13, "gola don pepito9", particion_a_escribir(carpincho->pid_carpincho));
	escribir_particion(carpincho2, 14, "gola don pepito9", particion_a_escribir(carpincho2->pid_carpincho));
	escribir_particion(carpincho3, 15, "12313 don pepito9", particion_a_escribir(carpincho3->pid_carpincho));

	agregar_carpincho_a_lista_carpinchos(carpincho);


	agregar_carpincho_a_lista_carpinchos(carpincho3);

	agregar_carpincho_a_lista_carpinchos(carpincho2);

	//escribir_particion(10, 13, "gola don pepito9", particion_a_escribir(10));
	t_dupla_pagina_marco* dupla = list_get(carpincho->dupla, 0);
	t_dupla_pagina_marco* dupla2 = list_get(carpincho->dupla, 1);

	loggear_trace("DUPLA 1 marco = %d pagina = %d", dupla->marco, dupla->pagina);
	loggear_trace("DUPLA 2 marco = %d pagina = %d", dupla2->marco, dupla2->pagina);

	t_archivo_swamp* archivo = list_get(lista_swamp, 0);
	t_archivo_swamp* archivo2 = list_get(lista_swamp, 1);

	loggear_debug("PRIMER CARPINCHO %d", atoi(list_get(archivo->carpinchos, 0)));
	loggear_debug("PRIMER CARPINCHO %d", atoi(list_get(archivo2->carpinchos, 0)));

	t_carpincho_swamp* asdadasd = list_get(lista_carpinchos,0);

	loggear_trace("LISTA TIENE %d", asdadasd->pid_carpincho);


*/

	//loggear_debug("SEGUNDO CARPINCHO %d", list_get(archivo->carpinchos, 1));

//SIRVE PARA MOSTRAR EL BITMAP DEJAR POR AHORA
 /*
	t_archivo_swamp* asd = list_get(lista_swamp, 0);

	for(int w = 0; w < get_cantidad_marcos(); w++){
		if(bitarray_test_bit(asd->bitmap_bitarray, w)){
			loggear_trace("1");
		}else{
			loggear_trace("0");
		}
	}

	t_archivo_swamp* asd1 = list_get(lista_swamp, 1);

	for(int w = 0; w < get_cantidad_marcos(); w++){
		if(bitarray_test_bit(asd1->bitmap_bitarray, w)){
			loggear_debug("1");
		}else{
			loggear_debug("0");
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



	pthread_t ram_handshake = thread_ejecutar_funcion(enviar_handshake);


	error = thread_join_and_free(ram_handshake);
	if (error != 0) {
		loggear_error("RAM no nos acepto, Error: %d", error);
		cerrar_todo();
		thread_detach_and_free(ram_handshake);
		return EXIT_FAILURE;
	}



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
	loggear_debug("CANTIDAD_MARCOS: %d", get_cantidad_marcos());
	loggear_debug("CANTIDAD_ARCHIVOS: %d", get_cantidad_archivos());
	for (int i = 0; i < get_cantidad_archivos(); ++i) {
		loggear_debug("ARCHIVOS_SWAP[%d]: \t\t%s", i, get_archivos_swap()[i]);
	}
}

/*
 * NOTAS IMPORTANTES PARA ESCRIBIR CUANDO LLEGA UN PROCESO.
 * PRIMERO EJECUTAR LA FUNCION
 *
 * t_carpincho_swamp* carpincho = crear_carpincho(14);
 *
 * LUEGO PARA ESCRIBIRLO. YA LA FUNCION PARTICION_A_ESCRIBIR DEVUELVE EN QUE PARTICION SE DEBE ESCRIBIR
 *
 * escribir_particion(carpincho, 11, "gola don pepito9", particion_a_escribir(carpincho->pid_carpincho));
 *
 * LUEGO PARA AGREAGARLO A LA LISTA ADMISTRATIVA REALIZAR ESTO.
 *
 *  agregar_carpincho_a_lista_carpinchos(carpincho);
 *
 *
 *
 *
 *NOTAS IMPORTANTES 2.
 * LLEGA UN PROCESO, LO PRIMERO QUE HACES ES FIJARNOS SI LA ASIGNACION ES FIJA O GLOBAL
 * ESTA MAL EL ARCHIVO A ESCRIBIR SEGUIR VIENDO MAS TARDE
 */
