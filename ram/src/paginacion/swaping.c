#include "swaping.h"

uint32_t traer_pagina_de_SWAP(uint32_t PID, int nroPag){

	uint32_t frame;
	void* info_a_guardar;
	if(!hay_que_hacer_swap(PID)) {
		frame = obtener_frame_libre(PID);
		info_a_guardar = pedir_a_swamp_info_pagina(PID, nroPag);
	}else {
		t_list* lista_frames = obtener_lista_frames_en_memoria(PID);
		t_list* lista_paginas = obtener_lista_paginas_de_frames(lista_frames);
		t_pagina* pagina_victima = obtener_pagina_victima(lista_paginas);
		frame = pagina_victima->frame;
		info_a_guardar = traer_y_controlar_consistencia_paginas(pagina_victima, nroPag, PID);

	}
	escribir_directamente_en_memoria(info_a_guardar, get_tamanio_pagina(), frame * get_tamanio_pagina());

	return frame;
}

void* traer_y_controlar_consistencia_paginas(t_pagina* pagina, int nro_pag, uint32_t pid) {
	// fijarse si esta modificado, setear en 0 el bit de presencia de la pagina victima
	void* info_en_pagina = NULL;
	//primero envio lo que tengo acutualizado
	//después recibo lo que tengoque copiar enmemoria
	if(pagina->bit_modificacion == 1) {
		//enviar la info modificada						send
		//recibir la info de la pagina de ese pid
	}

	return info_en_pagina;
}

t_list* obtener_lista_frames_en_memoria(uint32_t pid) {
	t_list* lista_frames_proceso;
	t_proceso* proceso = get_proceso_PID(pid);
		if(get_tipo_asignacion() == FIJA) {
			lista_frames_proceso = proceso->lista_frames_reservados;
		}else {
			lista_frames_proceso = listaFrames;
		}

	return lista_frames_proceso;

}

t_list* obtener_lista_paginas_de_frames(t_list* lista_frames){

	void* transformar_frame_a_pag(void* element){
		t_frame* frame = (t_frame*) element;
		t_proceso* proceso = get_proceso_PID(frame->proceso);
		return list_get(proceso->tabla_paginas,frame->pagina);
	}

	t_list* aux = list_map(lista_frames, transformar_frame_a_pag);
	return aux;
}

t_pagina* obtener_pagina_victima(t_list* lista_paginas) {

	t_pagina* pagina_victima;
	if(get_algoritmo_reemplazo_mmu() == CLOCKM){
		pagina_victima = obtener_victima_Clock_Modificado(lista_paginas);
	}else {
		pagina_victima = obtener_victima_LRU(lista_paginas);
	}
	return pagina_victima;
}

t_pagina* obtener_victima_LRU(t_list* lista_paginas){
	t_pagina* pagina_victima;
	t_pagina* pagina_anterior = (t_pagina*) list_get(lista_paginas, 0);
	for(int i = 0; i < list_size(lista_paginas); i++) {

		t_pagina* pagina_actual = (t_pagina*) list_get(lista_paginas, i);
		if(pagina_actual-> timestamp < pagina_anterior->timestamp) {
			pagina_victima = pagina_actual;
		}

		pagina_anterior = pagina_actual;


	}

	return pagina_victima;
}

t_pagina* obtener_victima_Clock_Modificado(t_list* lista_paginas){

 	t_pagina* pagina_victima = NULL;

	return pagina_victima;
}

int32_t hay_que_hacer_swap(uint32_t PID){
	//Revisar si hay frame libre segun el tipo de asignacion. Seguramente sea la misma logica que obtener frame pero en vez de conseguir uno preguntamos si hay uno
	t_list* frames = obtener_lista_frames_en_memoria(PID);
	return list_any_satisfy(frames, frame_disponible);
}

uint32_t obtener_frame_libre(uint32_t PID){
	//Conseguir un frame libre (segun tipo de asignacion FIJA o GLOBAL) y setear el frame conseguido en 1 de ocupado
	t_list* frames = obtener_lista_frames_en_memoria(PID);
	t_frame* frame_libre = list_find(frames, frame_disponible);
	return frame_libre->nroFrame;
}

void* pedir_a_swamp_info_pagina(uint32_t PID, int nroPag){//tengo que poner el frame ocupado, actualizarle los datos al frame y actualizarle los datos a la pag
	//Pedir info de la pag a SWAP, serializando y deserealizando msj
	void* data = NULL;
	return data;
}

bool frame_disponible(void* element){
	t_frame* frame = (t_frame*) element;
	return frame->estado==0;
}

void comunicar_eliminacion_proceso_SWAP(uint32_t PID){

}
