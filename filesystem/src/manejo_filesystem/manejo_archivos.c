#include "manejo_archivos.h"

uint32_t marco_libre(t_archivo_swamp* swamp);
bool puedo_darle_marco(t_carpincho_swamp* carpincho, uint32_t cantidad_marcos);

int escribir_particion(t_carpincho_swamp* carpincho, uint32_t pagina, char* texto_escribir, t_archivo_swamp* swamp){

	FILE* archivo;
	char* ruta_particion = swamp->ruta_archivo;

	uint32_t marco = marco_libre(swamp);

	if(marco < 0){
		loggear_error("NO HAY MAS MARCOS DISPONIBLES EN LA PARTICION %s PARA GUARDAR LA PAGINA %d DEL PROCESO %d", ruta_particion, pagina, carpincho->pid_carpincho);
		return -1;
	}

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

	swamp->espacio_libre = swamp->espacio_libre - 1;

	bitarray_set_bit(swamp->bitmap_bitarray, marco);

	t_dupla_pagina_marco* dupla = malloc(sizeof(t_dupla_pagina_marco));
	dupla->marco = marco;
	dupla->pagina = pagina;

	list_add(carpincho->dupla, dupla);

	loggear_debug("Se escribio con exito la pagina %d en el marco %d", pagina, marco);

	return 0;
}



char* leer_particion(uint32_t marco, t_archivo_swamp* swamp){

	char* ruta_particion = swamp->ruta_archivo;
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
	t_archivo_swamp* archivo_a_escribir;
	for(int i = 0; i < list_size(lista_swamp); i++){
		t_archivo_swamp* archivo = list_get(lista_swamp, i);
		if(archivo->espacio_libre > libre){
			libre = archivo->espacio_libre;
			archivo_a_escribir = archivo;
		}
	}

	loggear_trace("EL archivo a escribir es  %s", archivo_a_escribir->ruta_archivo);

	list_add(archivo_a_escribir->carpinchos, string_itoa(pid_carpincho));
	archivo_a_escribir->espacio_libre = archivo_a_escribir->espacio_libre - 1;
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


t_carpincho_swamp* crear_carpincho(uint32_t pid_carpincho){

	t_carpincho_swamp* carpincho = malloc(sizeof(t_carpincho_swamp));
	carpincho->pid_carpincho = pid_carpincho;
	carpincho->marcos_reservados = list_create();
	carpincho->marcos_usados = list_create();
	carpincho->dupla = list_create();
	carpincho->estado_carpincho = 0;

	t_archivo_swamp* archivo = archivo_a_escribir(pid_carpincho);

	loggear_error("aqui llego");

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
	}

	return carpincho;
}


int reservar_marcos(t_carpincho_swamp* carpincho, uint32_t cantidad_marcos, t_archivo_swamp* swamp){

	if(get_asignacion() == FIJA){
		if(!puedo_darle_marco(carpincho, cantidad_marcos)){
			loggear_warning("No se le puede reservar los marcos solicitados ya que excede la asignacion para este proceso pid: %d", carpincho->pid_carpincho);
			return -1;
		}
	} // ACA TESTEO SI NO SE EXCEDE DE LOS MARCOS MAXIMOS PERMITIDOS PARA EL PROCESO, SI SE EXCEDE SE RECHAZA.


	for(int i = 0; i < cantidad_marcos; i++){
		int marco = marco_libre(swamp);
		if(marco < 0){
			loggear_error("NO HAY MARCOS DISPONIBLES PARA ASIGNAR");
			list_clean(carpincho->marcos_reservados);
			return -1;
		}
		list_add(carpincho->marcos_reservados, string_itoa(marco));
		bitarray_set_bit(swamp->bitmap_bitarray, marco);
	}

	return 0;
}


bool puedo_darle_marco(t_carpincho_swamp* carpincho, uint32_t cantidad_marcos){
	if(list_size(carpincho->marcos_reservados) + cantidad_marcos + list_size(carpincho->marcos_usados)  <= get_marcos_maximos()){
		loggear_error("marcos reservados aqui %d", list_size(carpincho->marcos_reservados));
		return true;
	}
	return false;
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




