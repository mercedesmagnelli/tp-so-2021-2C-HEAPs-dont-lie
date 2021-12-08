#include "inicializar_filesystem.h"

int crear_particion();

void iniciar_swamp(){

	loggear_info("------------ SE COMIENZA A REALIZAR EL INICIO DEL SWAMP -----------------");

	crear_particion();

	//INICIALIZO UNA LISTA PARA TENER TODOS LOS CARPINCHOS QUE VAN APARECIENDO

	lista_carpinchos = list_create();


	loggear_info("SE INICIO EL SWAMP");

}

int crear_particion(){

	FILE* archivo;

	for(int i = 0; i < get_cantidad_archivos(); i++){

		archivo = fopen(get_archivos_swap()[i], "r");

		if(archivo == NULL){
			loggear_trace("No existe la particion en %s en la inicializacion del swamp", get_archivos_swap()[i]);
			loggear_trace("Se procede a crear la particion en %s", get_archivos_swap()[i]);

			archivo = fopen(get_archivos_swap()[i], "w+");

			fclose(archivo);

			truncate(get_archivos_swap()[i], get_tamanio_swap()); // Por el momento se inicializan todas las particiones con el tamaño del swap.

			loggear_trace("SE REALIZARON LAS PARTICIONES CORRESPONDIENTE CON TAMAÑO %d", get_tamanio_swap());

		}else{

		fclose(archivo);
		loggear_trace("YA existe la particion en %s", get_archivos_swap()[i]);
		}

	}

	return 0;
}

void destroy_carpinchos_swamp(t_carpincho_swamp* carpincho){
	if(carpincho->dupla != NULL){
	loggear_error("EL SIZE DE CARPINCHO DUPLA ES %d PID: %d", list_size(carpincho->dupla), carpincho->pid_carpincho);
	for(int i = 0; i < list_size(carpincho->dupla); i++){
	t_dupla_pagina_marco* dupl = list_get(carpincho->dupla, i);
	loggear_error("dupla %d = (%d, %d", i, dupl->marco, dupl->pagina);
	}
	for(int j = 0; j < list_size(carpincho->dupla); j++){
		t_dupla_pagina_marco* dupl2 = list_get(carpincho->dupla, j);
		free(dupl2);
		loggear_warning("VA %d", j);
	}
	list_destroy(carpincho->dupla);
	}
	loggear_error("rompemos aca");
	if(carpincho->marcos_reservados != NULL){
		loggear_warning("debo entrar aca1");
	list_destroy_and_destroy_elements(carpincho->marcos_reservados, free);
	}
	if(carpincho->marcos_usados != NULL){
		loggear_warning("debo entrar aca2");
	list_destroy_and_destroy_elements(carpincho->marcos_usados, free);
	}
	free(carpincho);
}

void destroy_lista_carpinchos(){
	for(int j = 0; j < list_size(lista_carpinchos); j++){
		destroy_carpinchos_swamp(list_get(lista_carpinchos, j));
	}

	list_destroy(lista_carpinchos);
}
