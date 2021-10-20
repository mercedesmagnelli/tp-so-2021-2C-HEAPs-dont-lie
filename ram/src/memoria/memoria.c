#include "memoria.h"

void* memoria_principal;


uint32_t memalloc(uint32_t pid, uint32_t size) {


	return 1;
}

void memfree(uint32_t* direccionLogicaALiberar) {

}

void* memread(uint32_t* direccionLogicaALeer, uint32_t tamanioALeer) {

	void* lectura = malloc(tamanioALeer);
	memcpy(lectura, direccionLogicaALeer, tamanioALeer);
	return lectura;
}

void memwrite(void* valorAEscribir, uint32_t* direccionLogicaAEscribir, uint32_t tamanioAEscribir){

	memcpy(direccionLogicaAEscribir, valorAEscribir, tamanioAEscribir);

}

void inicializar_memoria_principal() {
	int tamanio_memoria = get_tamanio_memoria();
	memoria_principal =  malloc(tamanio_memoria);


}


void consolidar(uint32_t indice_a_consolidar){

}

