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

int32_t no_se_asigna_proceso(uint32_t pid, uint32_t size) {
	if(1) {
		//fijo
		return MAXIMO_MARCOS_ALCANZADO;
	}else {
		return ESPACIO_EN_MEMORIA_INSUF;
	}
}

uint32_t ptro_valido(uint32_t PID, uint32_t ptro) {
	return 0;
}
int32_t agregar_proceso(uint32_t PID, uint32_t tam){
	printf("agregar proceso");
	return 1;
}

void actualizar_proceso(uint32_t PID, int32_t ptro, uint32_t tamanio){
	printf("holis");
}

int32_t se_asigna_memoria_necesaria(uint32_t pid, uint32_t size) {
	printf("hola, desarrollame, no seas rata");
	return 1;
}
uint32_t puedo_pedir_mas_memoria(uint32_t pid, uint32_t size){
	printf("no, no podés, deja de joder");
	return 1;
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
		ptro = (-1)* heap->currAlloc;
	}

	return ptro;

}

