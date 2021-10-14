#include "memoria.h"

uint32_t memalloc(uint32_t pid, uint32_t size) {

	/*Podría dividir el problema en 3
	 * 1. Es el primer elemento a agregar -> lo agrego al principio de la lista
	 * 2. Es un elemento que puede entrar en el medio
	 * 3. Se inserta al final */
 	/* Antes de empezar a insertar, tengo que verificar si puedo asignarle al proceso
 	 * En ambos casos (fija, dinamica) me tengo que fijar que la última pagina me pueda permitir guardar todo lo que quiero guardar.
 	 * Si no alcanza -> Si es fija: me tengo que fijar si ya llegue al máximo de marcos por proceso. Si ya llegué, entonces => rechazo
 	 * 				 -> Si es dinámica: me tengo que fijar que haya lugar en la RAM
 	 */

	return 0;
}

void memfree(uint32_t* direccionLogicaALiberar) {

}

void* memread(uint32_t* direccionLogicaALeer) {
	return 0;
}

void memwrite(void* valorAEscribir, uint32_t* direccionLogicaAEscribir /*t_list* tablaPaginas*/){

}

void inicializar_memoria_principal() {
	int tamanio_memoria = get_tamanio_memoria();
	memoria_principal =  malloc(tamanio_memoria);
}

void inicializar_estructuras_administrativas() {

	adm_memoria = list_create();
	cant_frames_proceso = dictionary_create();

}

void liberar_estructuras_administrativas() {

}

uint32_t* encontrar_espacio_disponible(uint32_t size) {

	return 1;
}


