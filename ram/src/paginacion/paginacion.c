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
uint32_t existe_proceso(uint32_t PID){
	return 0;

}


uint32_t ptro_donde_entra_data(PID,tam){
	t_proceso* proceso = get_proceso(PID);
	return 1;

}



// FUNCIONES PRIVADAS DE USO INTERNO

t_proceso* get_proceso(uint32_tPID){
	t_proceso* a;
	//TODO desarrollar funcion
	return a;

}
