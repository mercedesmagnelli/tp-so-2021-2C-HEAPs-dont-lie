#include "paginacion.h"

t_list* lista_procesos;
t_list* lista_frames;
t_dictionary* cant_frames_por_proceso;

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
