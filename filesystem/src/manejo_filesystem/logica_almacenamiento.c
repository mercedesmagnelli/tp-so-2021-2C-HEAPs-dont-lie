#include "logica_almacenamiento.h"

bool el_carpincho_esta_en_la_lista(t_archivo_swamp* archivo, uint32_t pid_carpincho);


t_archivo_swamp* particion_a_escribir(uint32_t pid_carpincho){ //TODO ver si esto es necesario hacerlo todo junto o si ya se si el proceso es nuevo de antemano por la RAM.
	for(int i = 0; i < get_cantidad_archivos(); i++){
		t_archivo_swamp* archivo = list_get(lista_swamp, i);
		if(el_carpincho_esta_en_la_lista(archivo, pid_carpincho)){
			loggear_debug("El archivo a utilizar es %s [PID: %d]", archivo->ruta_archivo, pid_carpincho);
			return archivo;
		}
	}
	return archivo_a_escribir(pid_carpincho);
}

bool el_carpincho_esta_en_la_lista(t_archivo_swamp* archivo, uint32_t pid_carpincho){
	t_list* carpinchos = archivo->carpinchos;
	int size = list_size(carpinchos);

	for(int i = 0; i < size; i++){
	char* pid = list_get(carpinchos, i);
		if(atoi(pid) == pid_carpincho){
			return true;
		}
	}
	return false;
}


bool esta_el_carpincho_en_lista_carpinchos(t_carpincho_swamp* carpincho){
	for(int i = 0; i < list_size(lista_carpinchos); i++){
		t_carpincho_swamp* carpinchi = list_get(lista_carpinchos, i);
		if(carpinchi->pid_carpincho == carpincho->pid_carpincho){
			return true;
		}
	}

	return false;
}

void agregar_carpincho_a_lista_carpinchos(t_carpincho_swamp* carpincho){

	if(!esta_el_carpincho_en_lista_carpinchos(carpincho)){
		loggear_debug("lo agrego a la lista xq no existe el carpincho en swamp");
		list_add(lista_carpinchos, carpincho);
	}else{

		destroy_carpinchos_swamp(carpincho);
		loggear_debug("ya existe el carpincho en la lista por lo que elimino el recien creado");

	}
}


