#include "swaping.h"

uint32_t crear_proceso_SWAP(uint32_t PID){

	uint32_t respuesta_final = 0;

	t_matelib_nuevo_proceso* mensaje = shared_crear_nuevo_proceso(PID);

	size_t tamanio;

	void* mensaje_serializado = serializiar_crear_proceso(mensaje, &tamanio);


	pthread_mutex_lock(&mutex_enviar_mensaje_swap);
	enviar_mensaje_protocolo(socket_swap,R_S_PROCESO_NUEVO, tamanio, mensaje_serializado);
	t_prot_mensaje* respuesta = recibir_mensaje_protocolo(socket_swap);
	pthread_mutex_unlock(&mutex_enviar_mensaje_swap);

	if(respuesta->head == EXITO_EN_LA_TAREA){
		respuesta_final = 1;
	}

	free(mensaje);
	return respuesta_final;

}

uint32_t traer_pagina_de_SWAP(uint32_t PID, int nroPag){


	uint32_t frame;
	void* info_a_guardar;
	if(hay_que_hacer_swap(PID)) {
		loggear_warning("pase el if1");
		frame = obtener_frame_libre(PID);
		loggear_warning("pase el if2");
		info_a_guardar =  recibir_info_en_pagina(nroPag, PID);
		loggear_warning("pase el if3");
	}else {
		loggear_warning("pase el else1");
		t_list* lista_frames = obtener_lista_frames_en_memoria(PID);
		loggear_warning("pase el else12");
		t_list* lista_paginas = obtener_lista_paginas_de_frames(lista_frames);
		loggear_warning("pase el else13");
		t_pagina* pagina_victima = obtener_pagina_victima(lista_paginas, PID);
		loggear_warning("pase el else14");
		frame = pagina_victima->frame;
		loggear_warning("pase el else15");
		info_a_guardar = traer_y_controlar_consistencia_paginas(pagina_victima, nroPag, PID);
		loggear_warning("pase el else16");

	}
	loggear_warning("FRAME!!: %d", frame);
	loggear_warning("antes de escribir en memoria");
	loggear_warning("tamanio pagina: %d", get_tamanio_pagina());
	escribir_directamente_en_memoria(info_a_guardar, get_tamanio_pagina(), frame * get_tamanio_pagina());
	loggear_warning("dsp de escribir en memoria");
	return frame;
}

void* traer_y_controlar_consistencia_paginas(t_pagina* pagina_victima, int nro_pag_a_pedir, uint32_t pid_a_pedir) {
	// fijarse si esta modificado, setear en 0 el bit de presencia de la pagina victima

	void* info_en_frame = obtener_info_en_frame(pagina_victima->frame);

	uint32_t pid_pag_victima = obtener_pid_en_frame(pagina_victima->frame);
	uint32_t nro_pag_victima = obtener_pag_en_frame(pagina_victima->frame);

	if(pagina_victima->bit_modificacion == 1) {
		size_t tamanio;
		t_write_s* mensaje = shared_crear_write_s(nro_pag_victima, pid_pag_victima, info_en_frame);
		void* mensaje_serializado = serializar_escribir_en_memoria(mensaje, &tamanio, get_tamanio_pagina());

		pthread_mutex_lock(&mutex_enviar_mensaje_swap);
		enviar_mensaje_protocolo(socket_swap,R_S_ESCRIBIR_EN_PAGINA,tamanio,mensaje_serializado);
		t_prot_mensaje* rec = recibir_mensaje_protocolo(socket_swap);
		pthread_mutex_unlock(&mutex_enviar_mensaje_swap);

		free(mensaje_serializado);
		if(rec->head== FALLO_EN_LA_TAREA){
			loggear_error("[RAM] - Hubo un problema en la escritura de la pagina %d del proceos %d en swamp", nro_pag_victima, pid_pag_victima);
		}

	}

	pagina_victima->bit_presencia = 0;

	void* info_en_pagina = recibir_info_en_pagina(nro_pag_a_pedir, pid_a_pedir);


	return info_en_pagina;
}

uint32_t obtener_pid_en_frame(uint32_t frame) {
	pthread_mutex_lock(&mutex_acceso_lista_frames);
	t_frame* frame_i = (t_frame*) list_get(listaFrames, frame);
	return frame_i->proceso;
	pthread_mutex_unlock(&mutex_acceso_lista_frames);
}
uint32_t obtener_pag_en_frame(uint32_t frame) {
	pthread_mutex_lock(&mutex_acceso_lista_frames);
	t_frame* frame_i = (t_frame*) list_get(listaFrames, frame);
	return frame_i->pagina;
	pthread_mutex_unlock(&mutex_acceso_lista_frames);
}
void* obtener_info_en_frame(uint32_t frame) {
	uint32_t tam_memoria = get_tamanio_pagina();
	void* info = malloc(tam_memoria);
	leer_directamente_de_memoria(info, tam_memoria * frame, tam_memoria);
	return info;
}

void* recibir_info_en_pagina(uint32_t pag_a_pedir, uint32_t pid_a_pedir) {

	loggear_trace("[SWAP] - Se va a pedir la pagina: %d del proceso %d", pag_a_pedir, pid_a_pedir);
	size_t tamanio;
	t_pedir_o_liberar_pagina_s* pedido = shared_crear_pedir_o_liberar(pid_a_pedir, pag_a_pedir);
	loggear_trace("pedido pid %zu", pedido->pid);
	loggear_trace("pedido pag %zu", pedido->nro_pag);

	void* mensaje_serializado = serializar_pedir_pagina(pedido, &tamanio);
	t_pedir_o_liberar_pagina_s* pedido2 = deserializar_mensaje_peticion_liberacion_pagina(mensaje_serializado);

	loggear_trace("pedido2 pid %zu", pedido2->pid);
	loggear_trace("pedido2 pag %zu", pedido2->nro_pag);

	pthread_mutex_lock(&mutex_enviar_mensaje_swap);
	enviar_mensaje_protocolo(socket_swap,R_S_PEDIR_PAGINA,tamanio,mensaje_serializado);
	loggear_trace("termine de enviar mensaje de tamanio %d:", tamanio);
	t_prot_mensaje* rec = recibir_mensaje_protocolo(socket_swap);
	loggear_trace("termine de recibir mensaje");
	pthread_mutex_unlock(&mutex_enviar_mensaje_swap);

	free(mensaje_serializado);
	if(rec->head == FALLO_EN_LA_TAREA){
				loggear_error("[RAM] - Hubo un problema en la recepcion de la info de la pagina %d del proceso %d en swamp", pag_a_pedir, pid_a_pedir);
	}

	void* info = deserializar_pedir_pagina(rec->payload);
	return info;

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

	return list_map(lista_frames, transformar_frame_a_pag);

}

t_pagina* obtener_pagina_victima(t_list* lista_paginas, uint32_t pid) {

	t_pagina* pagina_victima;
	if(get_algoritmo_reemplazo_mmu() == CLOCKM){
		pagina_victima = obtener_victima_Clock_Modificado(lista_paginas, pid);
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

t_pagina* obtener_victima_Clock_Modificado(t_list* lista_paginas, uint32_t pid){

 	t_pagina* pagina_victima;
 	t_proceso* proc = get_proceso_PID(pid);
 	uint32_t puntero = obtener_valor_puntero(proc);



 	uint32_t indice_encontrado;
 	//si no funciona el puntero por referencia entonces ver de hacer una funcion que se encargeu de actualizar el puntero y retornarlo
 	if(buscar_combinacion(lista_paginas,0,0, puntero, &indice_encontrado)){
 		pagina_victima = (t_pagina*) list_get(lista_paginas, indice_encontrado);
 	}else {
 		if(buscar_combinacion(lista_paginas, 0,1, puntero, &indice_encontrado)) {
 			pagina_victima =  (t_pagina*) list_get(lista_paginas, indice_encontrado);
 		}else {
 			modificar_bit_uso(lista_paginas);
 			if(buscar_combinacion(lista_paginas, 0,0, puntero, &indice_encontrado)){
 				pagina_victima = (t_pagina*) list_get(lista_paginas, indice_encontrado);
 			}else {
 				buscar_combinacion(lista_paginas, 0, 1, puntero, &indice_encontrado);
 				pagina_victima =  (t_pagina*) list_get(lista_paginas, indice_encontrado);
 			}
 		}
 	}

 	actualizar_puntero(proc, indice_encontrado + 1);

	return pagina_victima;
}

uint32_t obtener_valor_puntero(t_proceso* proc) {
	uint32_t puntero;
	if (get_tipo_asignacion() == FIJA) {
	 	 		puntero = proc->puntero_frames;
	 	 	}else {
	 	 		puntero = puntero_global;
	 	 	}
	return puntero;
}

void actualizar_puntero(t_proceso* proc, uint32_t indice_encontrado) {
	if (get_tipo_asignacion() == FIJA) {
	 		proc->puntero_frames = indice_encontrado;

	 	 	}else {
	 	 	 puntero_global = indice_encontrado;
	 	 }

}


void modificar_bit_uso(t_list* lista_paginas) {

	for(int i = 0; i < list_size(lista_paginas); i++) {
		t_pagina* pag = (t_pagina*) list_get(lista_paginas, i);
		pag->bit_uso = 0;

	}

}



bool buscar_combinacion(t_list* paginas, uint32_t puntero, uint32_t uso, uint32_t mod, uint32_t* indice_encontrado){


	bool encontrado = false;
	int cantidad_iteraciones = 0;
	uint32_t indice;
	while(!encontrado && cantidad_iteraciones <= list_size(paginas)){
		indice = calcular_indice(puntero, cantidad_iteraciones, list_size(paginas));
		t_pagina* pagina_apuntada = (t_pagina*) list_get(paginas, indice);

		if(pagina_apuntada->bit_modificacion == mod && pagina_apuntada->bit_uso == uso) {
			encontrado = true;
			(*indice_encontrado) = indice;
		}

		cantidad_iteraciones++;

	}
	return encontrado;

}

uint32_t calcular_indice(uint32_t puntero, uint32_t cantidad_it, uint32_t tamanio_lista) {

	uint32_t indice = puntero + cantidad_it;

	if(indice >= tamanio_lista){
		indice = (puntero + cantidad_it) % tamanio_lista;
	}
	return indice;
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
	void* data=NULL;
	return data;
}

bool frame_disponible(void* element){
	t_frame* frame = (t_frame*) element;
	return frame->estado==0;
}

void comunicar_eliminacion_proceso_SWAP(uint32_t PID){

	size_t tamanio;
	t_matelib_nuevo_proceso* pid = shared_crear_nuevo_proceso(PID);
	void* mensaje_serializado = serializiar_crear_proceso(pid, &tamanio);

	pthread_mutex_lock(&mutex_enviar_mensaje_swap);
	enviar_mensaje_protocolo(socket_swap,R_S_ELIMINAR_PROCESO,tamanio,mensaje_serializado);
 	t_prot_mensaje* rec = recibir_mensaje_protocolo(socket_swap);
 	pthread_mutex_unlock(&mutex_enviar_mensaje_swap);

 	free(mensaje_serializado);
	if(rec->head == FALLO_EN_LA_TAREA){
		loggear_error("[RAM] - Hubo un problema en la eliminacion del proceso %d en swamp",PID);
	}
}

void enviar_pagina_a_SWAP(uint32_t PID, uint32_t nro_pag, void* data_pag){

	size_t tamanio;
	t_write_s* mensaje = shared_crear_write_s(nro_pag, PID, data_pag);
	void* mensaje_serializado = serializar_escribir_en_memoria(mensaje, &tamanio, get_tamanio_pagina());

	pthread_mutex_lock(&mutex_enviar_mensaje_swap);
	enviar_mensaje_protocolo(socket_swap,R_S_ESCRIBIR_EN_PAGINA,tamanio,mensaje_serializado);
	t_prot_mensaje* rec = recibir_mensaje_protocolo(socket_swap);
	pthread_mutex_unlock(&mutex_enviar_mensaje_swap);

	free(mensaje_serializado);
	if(rec->head == FALLO_EN_LA_TAREA){
		loggear_error("[RAM] - Hubo un problema en la escritura de la pagina %d del proceos %d en swamp", nro_pag, PID);
	}
}
