#include "manejo_archivos.h"

uint32_t marco_libre(t_archivo_swamp* swamp);
bool puedo_darle_marco(t_carpincho_swamp* carpincho, uint32_t cantidad_marcos);
int obtener_marco_desde_pagina(uint32_t pagina, t_carpincho_swamp* carpincho);

int escribir_particion(t_carpincho_swamp* carpincho, uint32_t pagina, char* texto_escribir, t_archivo_swamp* swamp){

	FILE* archivo;
	char* ruta_particion = swamp->ruta_archivo;

	uint32_t marco = atoi(list_get(carpincho->marcos_reservados, 0));

	loggear_debug("SE PROCEDE A ESCRIBIR LA PAGINA %d EN EL MARCO %d DE LA PARTICION %s", pagina, marco, ruta_particion);

	archivo = fopen(ruta_particion, "r+");

		if(archivo == NULL){
			loggear_error("Ocurrio un error al abrir el archivo %s, puede deberse a que no esta creado o no es la ruta correcta", ruta_particion);
			return -1;
		}

		int posicion_escribir = marco * get_tamanio_pagina();

		fseek(archivo, posicion_escribir, SEEK_SET);

		fputs(texto_escribir, archivo);

	fclose(archivo);

	t_dupla_pagina_marco* dupla = malloc(sizeof(t_dupla_pagina_marco));
	dupla->marco = marco;
	dupla->pagina = pagina;

	list_add(carpincho->dupla, dupla);

	list_add(carpincho->marcos_usados, list_get(carpincho->marcos_reservados, 0));
	list_remove(carpincho->marcos_reservados, 0);

	loggear_debug("Se escribio con exito la pagina %d en el marco %d", pagina, marco);

	return 0;
}



char* leer_particion(uint32_t pagina, t_archivo_swamp* swamp, t_carpincho_swamp* carpincho){

	char* ruta_particion = swamp->ruta_archivo;

	int marco = obtener_marco_desde_pagina(pagina, carpincho);
	if(marco < 0){
		loggear_trace("NO EXISTE LA PAGINA %d del carpincho %d dentro de la swap", pagina, carpincho->pid_carpincho);
		return "basura";
	}
	loggear_debug("Se comienza a leer la pagina %d de la particion %s", marco, ruta_particion);

	FILE* archivo_lectura;
	char* contenido_pagina = malloc(get_tamanio_pagina() + 1);

	archivo_lectura = fopen(ruta_particion, "r+");

		if(archivo_lectura == NULL){
			loggear_error("Ocurrio un error al abrir el archivo %s, puede deberse a que no esta creado o no es la ruta correcta", ruta_particion);
			return "ERROR EN LECTURA IGNORAR MENSAJE"; // TODO pensar mejor esta parte para mandarle a la RAM o que control se puede hacer.
		}

	int posicion_a_leer = marco * get_tamanio_pagina();

	fseek(archivo_lectura, posicion_a_leer, SEEK_SET);

	fgets(contenido_pagina, get_tamanio_pagina() + 1, archivo_lectura);

	fclose(archivo_lectura);

	loggear_debug("Se termino la lectura de la pagina %d de la particion %s", marco, ruta_particion);

	return contenido_pagina;
}

t_archivo_swamp* archivo_a_escribir(uint32_t pid_carpincho){
	int libre = 0;
	t_archivo_swamp* archivo_a_escribir = list_get(lista_swamp, 0);
	for(int i = 0; i < list_size(lista_swamp); i++){
		t_archivo_swamp* archivo = list_get(lista_swamp, i);
		if(archivo->espacio_libre > libre){
			libre = archivo->espacio_libre;
			archivo_a_escribir = archivo;
		}
	}

	loggear_trace("EL archivo a escribir es  %s", archivo_a_escribir->ruta_archivo);

	list_add(archivo_a_escribir->carpinchos, string_itoa(pid_carpincho));
	return archivo_a_escribir;

}

uint32_t marco_libre(t_archivo_swamp* swamp){

	int i = 0;
	while(bitarray_test_bit(swamp->bitmap_bitarray, i)){
				i++;
			}

	if(i == get_cantidad_marcos()){
			loggear_error("NO HAY MARCO DISPONIBLE PARA ASIGNARLE");
				return -1;
			}

	loggear_trace("Se asigna el marco %d", i);
	return i;
}


t_carpincho_swamp* crear_carpincho(uint32_t pid_carpincho, uint32_t cantidad_paginas_reservadas){

	t_carpincho_swamp* carpincho = malloc(sizeof(t_carpincho_swamp));
	carpincho->pid_carpincho = pid_carpincho;
	carpincho->marcos_reservados = list_create();
	carpincho->marcos_usados = list_create();
	carpincho->dupla = list_create();
	carpincho->estado_carpincho = 0;

	t_archivo_swamp* archivo = archivo_a_escribir(pid_carpincho);
	if(get_asignacion() == FIJA){
		loggear_debug("LLEGO LA SOLICITUD DE UN CARPINCHO Y COMO LA ASIGNACION ES FIJA SE PROCEDE A RESERVAR SUS MARCOS MAXIMOS PID: %d", carpincho->pid_carpincho);
		for(int i = 0; i < get_marcos_maximos(); i++){
			int marco = marco_libre(archivo);
			if(marco < 0){
				loggear_error("NO HAY MARCOS DISPONIBLES PARA ASIGNAR, NO SE PUEDE GUARDAR EL CARPINCHO EN LA SWAP YA QUE NO ESTAN LOS MARCOS SUFICIENTES pid: %d", carpincho->pid_carpincho);
				carpincho->estado_carpincho = 1;
				return carpincho;
			}
			list_add(carpincho->marcos_reservados, string_itoa(marco));
			bitarray_set_bit(archivo->bitmap_bitarray, marco);
		}

		archivo->espacio_libre = archivo->espacio_libre - get_marcos_maximos();

	}else{ //ACA SERIA SI LA ASGINACION ES GLOBAL
		loggear_debug("LLEGO UNA SOLICITUD DE UN CARPINCHO Y COMO LA ASIGNACION ES GLOBAL SE RESERVAN LOS MARCOS %d SOLICITADOS PID:%d", cantidad_paginas_reservadas, carpincho->pid_carpincho);
		if(cantidad_paginas_reservadas <= archivo->espacio_libre){
			for(int j = 0; j < cantidad_paginas_reservadas; j++){
				int marco_global = marco_libre(archivo);
				list_add(carpincho->marcos_reservados, string_itoa(marco_global));
				bitarray_set_bit(archivo->bitmap_bitarray, marco_global);
			}
			archivo->espacio_libre = archivo->espacio_libre - cantidad_paginas_reservadas;
		}


	}

	list_add(lista_carpinchos, carpincho);

	return carpincho;
}


int reservar_marcos(t_carpincho_swamp* carpincho, uint32_t cantidad_marcos, t_archivo_swamp* swamp){

	if(get_asignacion() == FIJA){
		loggear_warning("como la asignacion es fija no entro aqui");
		return 0;
		}
	 // ACA SI ES FIJA YA LOS MARCOS SE LE RESERVARON CON ANTERIORIDAD AL CREAR EL CARPINCHO

		loggear_warning("pase para asginacion global pid: %d", carpincho->pid_carpincho);
	t_list* marcos_lista = list_create();
	for(int i = 0; i < cantidad_marcos; i++){
		int marco = marco_libre(swamp);
		if(marco < 0){
			loggear_error("NO HAY MARCOS DISPONIBLES PARA ASIGNAR");
			for(int j = 0; j < list_size(marcos_lista); j++){
			char* marco_str = list_get(marcos_lista, j);
			bitarray_clean_bit(swamp->bitmap_bitarray, atoi(marco_str));
			}
			list_destroy_and_destroy_elements(marcos_lista, free);
			return -1;
		}
		list_add(marcos_lista, string_itoa(marco));
		bitarray_set_bit(swamp->bitmap_bitarray, marco);
	}

	list_add_all(carpincho->marcos_reservados, marcos_lista);
	swamp->espacio_libre = swamp->espacio_libre - cantidad_marcos;

	loggear_warning("aca voy bien");
	list_destroy(marcos_lista);
	return 0;
}


bool puedo_darle_marco(t_carpincho_swamp* carpincho, uint32_t cantidad_marcos){
	if(list_size(carpincho->marcos_reservados) + cantidad_marcos + list_size(carpincho->marcos_usados)  <= get_marcos_maximos()){
		loggear_error("marcos reservados aqui %d", list_size(carpincho->marcos_reservados));
		return true;
	}
	return false;
}


int obtener_marco_desde_pagina(uint32_t pagina, t_carpincho_swamp* carpincho){
	for(int i = 0; i < list_size(carpincho->dupla); i++){
		t_dupla_pagina_marco* dupla = list_get(carpincho->dupla, i);
		if(dupla->pagina == pagina){
			loggear_warning("el marco de la pagina %d del proceso %d es: %d", pagina, carpincho->pid_carpincho, dupla->marco);
			return dupla->marco;
		}
	}
	return -1;
}

/*
 * PAGINA = 10
 * BUSCO ESPACIO Y ENCUENTRO QUE TENGO LIBRE EL MARCO 1
 * TENGO QUE CONSIDERAR QUE CON ASIGNACION FIJA TENGO QUE RESERVAR CONTIGUOS
 *
 *
 *
 *
 */




