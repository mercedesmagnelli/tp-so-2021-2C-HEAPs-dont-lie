#include "paginacion.h"



// FUNCIONES DE INICIO Y DESTRUCCION ADMINISTRATIVAS

void inicializar_estructuras_administrativas() {
    lista_procesos = list_create();
    lista_frames = list_create();
    cant_frames_por_proceso = dictionary_create();

}

void destruir_estructuras_administativas() {
    list_destroy_and_destroy_elements(lista_procesos, destruir_proceso);
    list_destroy_and_destroy_elements(lista_frames, free);
    //TODO: ver que puede ser que tengamos que llamar a dictionary_destroy_and_destroy_elements
    dictionary_destroy(cant_frames_por_proceso);

}
void destruir_proceso(void* proceso) {

    list_destroy_and_destroy_elements(((t_proceso*)proceso)->tabla_paginas, free);
    list_destroy_and_destroy_elements(((t_proceso*)proceso)->lista_hmd, free);
    free(proceso);

}




// FUNCIONES PUBLICAS
int32_t existe_proceso(uint32_t PID){
	t_proceso* proceso = get_proceso_PID(PID);
	return proceso != NULL ? 0 : -1;

}


int32_t ptro_donde_entra_data(uint32_t PID, uint32_t tam){
	t_proceso* proceso = get_proceso_PID(PID);

	t_list* listaHMD = proceso->lista_hmd;

	return get_ptro_con_tam_min(listaHMD, tam);

}

void actualizar_proceso(uint32_t PID, int32_t ptro, uint32_t tamanio){
	heap_metadata* heap = get_HEAP(PID,ptro);
	heap->isFree = 0;
	if(heap->nextAlloc == NULL){
		heap_metadata* nuevoHeap = malloc(sizeof(heap_metadata));
		nuevoHeap->currAlloc = heap->nextAlloc - heap->currAlloc -9 -tamanio ;
		nuevoHeap->prevAlloc = heap->currAlloc;
		nuevoHeap->nextAlloc = heap->nextAlloc;
		nuevoHeap->isFree    = 1;
		heap->nextAlloc = nuevoHeap->currAlloc;
		agregar_HEAP_a_PID(PID,nuevoHeap);
	}else{
		if((heap->nextAlloc-heap->currAlloc-9) > (tamanio+9)){
			heap_metadata* nuevoHeap = malloc(sizeof(heap_metadata));
			nuevoHeap->currAlloc = heap->nextAlloc - heap->currAlloc -9 -tamanio ;
			nuevoHeap->prevAlloc = heap->currAlloc;
			nuevoHeap->nextAlloc = heap->nextAlloc;
			nuevoHeap->isFree    = 1;
			heap->nextAlloc = nuevoHeap->currAlloc;
			heap_metadata* heapSig = get_HEAP(PID,nuevoHeap->nextAlloc);
			heapSig->prevAlloc = nuevoHeap->currAlloc;
			agregar_HEAP_a_PID(PID,nuevoHeap);
		}
	}
}



// FUNCIONES PRIVADAS DE USO INTERNO

t_proceso* get_proceso_PID(uint32_t PID){

	bool proceso_PID(void* element) {
			t_proceso* proceso = (t_proceso*) element;
			return proceso->PID == PID;
		}

	t_proceso* proceso = list_find(lista_procesos, proceso_PID);
	return proceso;

}

int32_t get_ptro_con_tam_min(t_list* listaHMD, uint32_t tam){

	int32_t ptro;

	bool heap_tam_min(void* element){
		bool rta;
		heap_metadata* heap = (heap_metadata*) element;
		if(heap->nextAlloc==NULL){
			rta = true;
		}else{
			if((heap->nextAlloc - heap->currAlloc - 9)>= tam){
				rta = true;
			}else{
				rta = false;
			}
		}
		return rta;
	}

	heap_metadata* heap = list_find(listaHMD,heap_tam_min);

	if(heap->nextAlloc==NULL){
		ptro = (-1)* (heap->currAlloc+9);
	}

	return ptro;

}


heap_metadata* get_HEAP(uint32_t PID, int32_t ptro){

	bool heap_ptro(void* element){
		heap_metadata* heap = (heap_metadata*) element;
		return heap->currAlloc == (ptro-9);

	}

	t_proceso* proceso = get_proceso_PID(PID);
	t_list* listaHMD = proceso->lista_hmd;
	heap_metadata* heap = list_find(listaHMD,heap_ptro);
	return heap;

}

void agregar_HEAP_a_PID(uint32_t PID, heap_metadata* heap){

	bool condicion(void* elementoListado, void* datoAInsertar){
		heap_metadata* elemList = (heap_metadata*) elementoListado;
		heap_metadata* elemInsertar = (heap_metadata*) datoAInsertar;

		return elemList->currAlloc < elemInsertar->currAlloc;
	}

	t_proceso* proceso = get_proceso_PID(PID);
	t_list* listaHMD = proceso->lista_hmd;

	list_add_sorted(listaHMD, (void*) heap, condicion);
	//TODO testear que el insert se haga en la posicion correcta

}


///////////////////////////////////////////////////////////////////////////////////////////

int32_t no_se_asigna_proceso(uint32_t pid, uint32_t size) {
	if(1) {
		//fijo
		return MAXIMO_MARCOS_ALCANZADO;
	}else {
		return ESPACIO_EN_MEMORIA_INSUF;
	}
}

int32_t agregar_proceso(uint32_t PID, uint32_t tam){
	printf("agregar proceso");
	return 1;
}

int32_t se_asigna_memoria_necesaria(uint32_t pid, uint32_t size) {
	printf("hola, desarrollame, no seas rata");
	return 1;
}
uint32_t puedo_pedir_mas_memoria(uint32_t pid, uint32_t size){
	printf("no, no podés, deja de joder");
	return 1;
}

uint32_t ptro_valido(uint32_t PID, uint32_t ptro) {
	return 0;
}



uint32_t traducir_a_dir_fisica(uint32_t logica){
	return 1;
}


uint32_t ptro_liberado(uint32_t PID, uint32_t ptro){
	return 1;
}

void liberar_memoria(uint32_t PID, uint32_t ptro){
	printf("libera la memoria rataaa");
}

void consolidar_memoria(uint32_t PID){
	printf("don't mess with me, i have the power of god and anime by my side");
}
