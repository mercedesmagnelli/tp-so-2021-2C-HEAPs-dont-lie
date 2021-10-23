#include "manejo_archivos.h"

int escribir_particion(uint32_t pagina, char* texto_escribir, t_archivo_swamp* swamp){

	FILE* archivo;
	char* ruta_particion = swamp->ruta_archivo;

	loggear_debug("SE PROCEDE A ESCRIBIR LA PAGINA %d DE LA PARTICION %s", pagina, ruta_particion);

	archivo = fopen(ruta_particion, "r+");

		if(archivo == NULL){
			loggear_error("Ocurrio un error al abrir el archivo %s, puede deberse a que no esta creado o no es la ruta correcta", ruta_particion);
			return -1;
		}

		int posicion_escribir = pagina * get_tamanio_pagina();

		fseek(archivo, posicion_escribir, SEEK_SET);

		fputs(texto_escribir, archivo);

	fclose(archivo);

	swamp->espacio_libre = swamp->espacio_libre - 1;

	loggear_debug("Se escribio con exito la pagina %d", pagina);

	return 0;
}



char* leer_particion(uint32_t pagina, t_archivo_swamp* swamp){

	char* ruta_particion = swamp->ruta_archivo;
	loggear_debug("Se comienza a leer la pagina %d de la particion %s", pagina, ruta_particion);

	FILE* archivo_lectura;
	char* contenido_pagina = malloc(get_tamanio_pagina() + 1);

	archivo_lectura = fopen(ruta_particion, "r+");

		if(archivo_lectura == NULL){
			loggear_error("Ocurrio un error al abrir el archivo %s, puede deberse a que no esta creado o no es la ruta correcta", ruta_particion);
			return "ERROR EN LECTURA IGNORAR MENSAJE"; // TODO pensar mejor esta parte para mandarle a la RAM o que control se puede hacer.
		}

	int posicion_a_leer = pagina * get_tamanio_pagina();

	fseek(archivo_lectura, posicion_a_leer, SEEK_SET);

	fgets(contenido_pagina, get_tamanio_pagina() + 1, archivo_lectura);

	fclose(archivo_lectura);

	loggear_debug("Se termino la lectura de la pagina %d de la particion %s", pagina, ruta_particion);

	return contenido_pagina;
}

t_archivo_swamp* archivo_a_escribir(){
	int libre = 0;
	t_archivo_swamp* archivo_a_escribir;
	for(int i = 0; i < list_size(lista_swamp); i++){
		t_archivo_swamp* archivo = list_get(lista_swamp, i);
		if(archivo->espacio_libre > libre){
			libre = archivo->espacio_libre;
			archivo_a_escribir = archivo;
		}
	}

	loggear_trace("EL archivo a ecribir es  %s", archivo_a_escribir->ruta_archivo);

	return archivo_a_escribir;

}



