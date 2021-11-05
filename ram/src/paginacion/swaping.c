#include "swaping.h"

uint32_t traer_pagina_de_SWAP(uint32_t PID, int nroPag){

	uint32_t frame;
	void* info_a_guardar;
	if(!hay_que_hacer_swap(PID)) {
		frame = obtener_frame_libre(PID);
		info_a_guardar = pedir_a_swamp_info_pagina(PID, nroPag);
	}else {
		t_list* tabla_paginas = obtener_tabla_paginas_mediante_PID(PID);
		//t_list* lista_paginas_presentes = obtener_lista_paginas_en_memoria(tabla_paginas);
		t_pagina* pagina_victima = obtener_pagina_victima(lista_paginas_presentes);
		frame = pagina_victima->frame;
		info_a_guardar = traer_y_controlar_consistencia_paginas(pagina_victima, nroPag, PID);

	}
	escribir_directamente_en_memoria(info_a_guardar, get_tamanio_pagina(), frame * get_tamanio_pagina());

	return frame;
}

void* traer_y_controlar_consistencia_paginas(t_pagina* pagina, int nro_pag, uint32_t pid) {
	// fijarse si esta modificado, setear en 0 el bit de presencia de la pagina victima
	void* info_en_pagina;
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

 	t_pagina* pagina_victima;

	return pagina_victima;
}

int32_t hay_que_hacer_swap(uint32_t PID){
	//Revisar si hay frame libre segun el tipo de asignacion. Seguramente sea la misma logica que obtener frame pero en vez de conseguir uno preguntamos si hay uno
	return 1;
}

uint32_t obtener_frame_libre(uint32_t PID){
	//Conseguir un frame libre (segun tipo de asignacion FIJA o GLOBAL) y setear el frame conseguido en 1 de ocupado
	return 1;
}

void* pedir_a_swamp_info_pagina(uint32_t PID, int nroPag){
	//Pedir info de la pag a SWAP, serializando y deserealizando msj
	void* data;
	return data;
}
