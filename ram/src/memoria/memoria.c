#include "memoria.h"

void* memoria_principal;
//t_dictionary* cant_frames_proceso;
t_list* espacios_memoria;


uint32_t memalloc(uint32_t pid, uint32_t size) {

	/*Cuando hago el memalloc, tengo que escribir el heapmetadata correcpondiente y */


	return encontrar_espacio_disponible_ff(size);
}

void memfree(uint32_t* direccionLogicaALiberar) {

}

void* memread(uint32_t* direccionLogicaALeer, uint32_t tamanioALeer) {

	void* lectura = malloc(tamanioALeer);
	memcpy(lectura, direccionLogicaALeer , tamanioALeer);
	return lectura;
}

void memwrite(void* valorAEscribir, uint32_t* direccionLogicaAEscribir, uint32_t tamanioAEscribir){

	memcpy(direccionLogicaAEscribir, valorAEscribir, tamanioAEscribir);

}

void inicializar_memoria_principal() {
	int tamanio_memoria = get_tamanio_memoria();
	memoria_principal =  malloc(tamanio_memoria);
	/*Ya inicializo la memoria con un primer heapmetadata que me divide a la memoria en un gran bloque*/

}

void guardar_heapmetadata(uint32_t donde, uint32_t tamanio) {

}

void inicializar_estructuras_administrativas() {

	espacios_memoria = list_create();
	//cant_frames_proceso = dictionary_create();

}

void liberar_estructuras_administrativas() {

}

uint32_t* encontrar_espacio_disponible_ff(uint32_t size) {

	return 1;
}

void consolidar(uint32_t indice_a_consolidar){

}

