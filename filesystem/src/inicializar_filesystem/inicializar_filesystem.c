#include "inicializar_filesystem.h"

int crear_particion();
int escribir_particion();

void iniciar_swamp(){

	crear_particion();

	loggear_warning("procedo a escribir particion");
	escribir_particion();
}

int crear_particion(){

	FILE* archivo;

		for(int i = 0; i < get_cantidad_archivos(); i++){

		archivo = fopen(get_archivos_swap()[i], "w+");

		if(archivo == NULL){
			loggear_error("No se pudo crear la particion en %s en la inicializacion del swamp", get_archivos_swap()[i]);
			return -1;
		}

		fclose(archivo);
		truncate(get_archivos_swap()[i], get_tamanio_swap()); // Por el momento se inicializan todas las particiones con el tamaño del swap.
		}

		loggear_trace("SE REALIZARON LAS PARTICIONES CORRESPONDIENTE CON TAMAÑO %d", get_tamanio_swap());

	return 0;
}

int escribir_particion(){


	FILE* archivo;

		char * ruta_particion = "/home/utnso/swap1.bin";

		archivo = fopen(ruta_particion, "r+");

		if(archivo == NULL){
					return -1;
				}

		fseek(archivo, 10, SEEK_SET);

		//fwrite("alan12345", 9, 1, archivo);

		fputs("texto_a_escribir", archivo);


		fclose(archivo);

		//free(ruta_particion);

		//truncate(ruta_particion2, get_tamanio_swap());

		return 0;
}
