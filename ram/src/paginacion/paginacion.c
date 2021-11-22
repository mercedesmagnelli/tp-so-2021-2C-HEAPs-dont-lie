#include "paginacion.h"



// FUNCIONES DE INICIO Y DESTRUCCION ADMINISTRATIVAS

void inicializar_estructuras_administrativas() {

	inicializar_memoria_principal();
    listaProcesos = list_create();
    listaFrames = list_create();
    listaFramesReservados = list_create();
    uint32_t cant_frames = get_tamanio_memoria() / get_tamanio_pagina();
    for(int i=0;i<cant_frames;i++){
    	t_frame* frame = malloc(sizeof(t_frame));
    	frame->nroFrame=i;
    	frame->estado=0;
    	list_add(listaFrames, frame);
    }
    puntero_global = 0;
    cant_frames_por_proceso = dictionary_create();
    inicializar_tlb();
    inicializar_semaforos();

}

void inicializar_semaforos() {
	pthread_mutex_init(&mutex_acceso_lista_frames, NULL);
	pthread_mutex_init(&mutex_enviar_mensaje_swap, NULL);
	pthread_mutex_init(&mutex_acceso_memoria, NULL);
	pthread_mutex_init(&mutex_acceso_lista_frames_r, NULL);
	pthread_mutex_init(&mutex_acceso_diccionario, NULL);
}

void destruir_estructuras_administrativas() {
    list_destroy_and_destroy_elements(listaProcesos, destruir_proceso);
    list_destroy_and_destroy_elements(listaFrames, free);
    free(memoria_principal);
    //TODO: ver que puede ser que tengamos que llamar a dictionary_destroy_and_destroy_elements
    dictionary_destroy(cant_frames_por_proceso);
    destruir_tlb();
    destruir_semaforos();
}
void destruir_semaforos() {
		pthread_mutex_destroy(&mutex_acceso_lista_frames);
		pthread_mutex_destroy(&mutex_enviar_mensaje_swap);
		pthread_mutex_destroy(&mutex_acceso_memoria);
		pthread_mutex_destroy(&mutex_acceso_lista_frames_r);
		pthread_mutex_destroy(&mutex_acceso_diccionario);
}
void destruir_proceso(void* proceso) {

    list_destroy_and_destroy_elements(((t_proceso*)proceso)->tabla_paginas, free);
    list_destroy_and_destroy_elements(((t_proceso*)proceso)->lista_hmd, free);
    list_destroy(((t_proceso*)proceso)->lista_frames_reservados);//no se eliminan los elementos porque son los mismos referenciados en listaFrames que contiene todos los frames de la RAM
    free(proceso);

}




// FUNCIONES PUBLICAS
int32_t existe_proceso(uint32_t PID){
	t_proceso* proceso = get_proceso_PID(PID);
	return proceso == NULL ? 0 : 1;

}

uint32_t iniciar_proceso_SWAP(uint32_t PID){
	return crear_proceso_SWAP(PID);
}

void iniciar_proceso_RAM(uint32_t PID){
	t_proceso* nuevoProceso = malloc(sizeof(t_proceso));
	nuevoProceso->PID = PID;
	nuevoProceso->tabla_paginas = list_create();
	nuevoProceso->lista_hmd = list_create();
	nuevoProceso->lista_frames_reservados = list_create();
	nuevoProceso->hits_proceso = 0;
	nuevoProceso->miss_proceso = 0;
	list_add(listaProcesos, nuevoProceso);
}

void alistar_proceso(uint32_t PID){

	t_proceso* proceso = get_proceso_PID(PID);

	if(get_tipo_asignacion() == FIJA){
		reservar_frames(proceso->lista_frames_reservados);
		proceso->puntero_frames = 0;
	}
}


int32_t ptro_donde_entra_data(uint32_t PID, uint32_t tam){

	t_list* listaHMD = conseguir_listaHMD_mediante_PID(PID);

	int32_t ptro = -9;

	if(!list_is_empty(listaHMD)){

		bool heap_tam_min(void* element){
			bool rta;
			heap_metadata* heap = (heap_metadata*) element;
			leer_heap(heap, PID);
			if(heap->nextAlloc==-1){
				rta = true;
			}else{
				if(espacio_de_HEAP(heap)>= tam+9 && heap->isFree){
					rta = true;
				}else{
					rta = false;
				}
			}
			return rta;
		}

		heap_metadata* heap = list_find(listaHMD,heap_tam_min);

		ptro = heap->currAlloc + 9;

		if(heap->nextAlloc==-1 && calcular_tamanio_ultimo_HEAP(PID)<tam+9){
			ptro = (-1)* ptro;
		}

	}

	return ptro;

}

void actualizar_proceso(uint32_t PID, int32_t ptro, uint32_t tamanio){

	//FIXME: puede que acá haya que traer a memoria???
	t_list* listaHMD = conseguir_listaHMD_mediante_PID(PID);
	if(list_is_empty(listaHMD)) {
		agregar_proceso(PID, tamanio);
	}else {
		heap_metadata* heap = get_HEAP(PID,ptro);
		int nextNextAlloc = heap->nextAlloc;
			heap->nextAlloc = ptro+tamanio;
			heap->isFree = 0;
			guardar_HEAP_en_memoria(PID, heap);


			heap_metadata* nuevoHeap = malloc(sizeof(heap_metadata));
			nuevoHeap->currAlloc = heap->nextAlloc;
			nuevoHeap->prevAlloc = heap->currAlloc;
			nuevoHeap->nextAlloc = nextNextAlloc;
			nuevoHeap->isFree    = 1;
			agregar_HEAP_a_PID(PID,nuevoHeap);
			guardar_HEAP_en_memoria(PID, nuevoHeap);

			//si no es el ultimo alloc, traemos el sig HEAP para modificarlo y actualizamos en mem
			if(nuevoHeap->nextAlloc != -1){

				heap_metadata* heapSig = get_HEAP(PID,nuevoHeap->nextAlloc);
				heapSig->prevAlloc = nuevoHeap->currAlloc;
				guardar_HEAP_en_memoria(PID, heapSig);
			}

	}



}


int32_t no_se_asigna_proceso(uint32_t pid, uint32_t size) {
	if(get_tipo_asignacion() == FIJA) {
		loggear_error("No se puede asignar el espacio para el proceso %d porque se alcanzo la cantidad de frames posibles para un proceso", pid);
		return MAXIMO_MARCOS_ALCANZADO;
	}else {
		loggear_error("No se puede asignar el espacio para el proceso %d porque no hay mas lugar en memoria actualmente", pid);
		return ESPACIO_EN_MEMORIA_INSUF;
	}
}

void agregar_proceso(uint32_t PID, uint32_t tam){
	//creo el proceso y lo inicializo

	t_proceso* nuevoProceso = get_proceso_PID(PID);

	int cantPags = calcular_paginas_para_tamanio(tam);

	t_pagina* nuevaPagina;
	for(int i = 0;i<cantPags;i++){
		nuevaPagina = malloc(sizeof(t_pagina));
		nuevaPagina->bit_presencia=0;//es el unico dato que llenamos
		list_add(nuevoProceso->tabla_paginas,nuevaPagina);
	}



	heap_metadata* nuevoHeapPrimero = malloc(sizeof(heap_metadata));
	nuevoHeapPrimero->currAlloc = 0;
	nuevoHeapPrimero->prevAlloc = -1;
	nuevoHeapPrimero->nextAlloc = tam + 9;
	nuevoHeapPrimero->isFree    = 0;

	agregar_HEAP_a_PID(PID,nuevoHeapPrimero);

	guardar_HEAP_en_memoria(PID, nuevoHeapPrimero);



	heap_metadata* nuevoHeapUltimo = malloc(sizeof(heap_metadata));
	nuevoHeapUltimo->currAlloc = tam+9;
	nuevoHeapUltimo->prevAlloc = 0;
	nuevoHeapUltimo->nextAlloc = -1;
	nuevoHeapUltimo->isFree    = 1;
	agregar_HEAP_a_PID(PID,nuevoHeapUltimo);
	guardar_HEAP_en_memoria(PID, nuevoHeapUltimo);



	//return nuevoHeapPrimero->currAlloc+9;//siempre el primer alloc va a ser 9 porque el primer dato se guarda al comienzo, y el metadata ocupa 9 bytes
}

int32_t memoria_suficiente_en_swap(uint32_t pid, uint32_t size) {

	loggear_warning("entre aqui");
	uint32_t cantidad_paginas_extras = paginas_extras_para_proceso(pid, size);
	loggear_warning("entre aqui2");

	t_mensaje_r_s* mensaje = shared_crear_t_mensaje_r_s(cantidad_paginas_extras, pid);
	loggear_warning("entre aqui3");
	size_t tamanio;
	loggear_warning("entre aqui4");
	void* mensaje_serializado = serializar_solicitud_espacio(mensaje, &tamanio);
	loggear_warning("entre aqui5");


	pthread_mutex_lock(&mutex_enviar_mensaje_swap);
	loggear_error("tamanio %d", tamanio);
	loggear_debug("socket: %d", socket_swap);
	enviar_mensaje_protocolo(socket_swap, R_S_SOLICITUD_ESPACIO, tamanio, mensaje_serializado);
	loggear_warning("entre aqui6");
	t_prot_mensaje* respuesta = recibir_mensaje_protocolo(socket_swap);
	loggear_warning("entre aqui7");
	pthread_mutex_unlock(&mutex_enviar_mensaje_swap);
	loggear_warning("entre aqui8");

	free(mensaje_serializado);

	uint32_t respuesta_final = 1;

	if(respuesta->head == FALLO_EN_LA_TAREA) {
		respuesta_final = 0;
	}

	t_pagina* nueva_pagina;

	t_proceso* proceso = get_proceso_PID(pid);
	if(respuesta_final == 1){
		for(int i = 0;i<cantidad_paginas_extras;i++){
			nueva_pagina= malloc(sizeof(t_pagina));
			nueva_pagina->bit_presencia=0;//es el unico dato que llenamos
			list_add(proceso->tabla_paginas,nueva_pagina);
		}
	}

	free(mensaje);
	return respuesta_final;

}

uint32_t paginas_extras_para_proceso(uint32_t pid, uint32_t size) {
	loggear_debug("entre aqui 2");
	uint32_t cantidad =  (size+9) / get_tamanio_pagina();
	loggear_debug("entre aqui 2  x2");
	uint32_t resto_ult_pag = calcular_tamanio_ultimo_HEAP(pid);
	loggear_debug("entre aqui 2 x3");
	uint32_t excedente = (size+9) % get_tamanio_pagina();

	if(resto_ult_pag < excedente) {
		cantidad++;
	}

	loggear_info("[MATELIB_MEM_ALLOC] Se van a pedir %d paginas extras a swamp", cantidad);

	return cantidad;
}


bool ptro_valido(uint32_t PID, uint32_t ptro) {

	t_list* lista_heaps = conseguir_listaHMD_mediante_PID(PID);

	bool condition(void* heap) {
		heap_metadata* heap_md = (heap_metadata*) heap;
		leer_heap(heap, PID);
		return (heap_md->currAlloc + 9) == ptro;
	}
	return list_any_satisfy(lista_heaps,condition);
}

uint32_t tamanio_de_direccion(uint32_t direccionLogicaALeer, uint32_t PID){
	heap_metadata* heap = get_HEAP(PID, direccionLogicaALeer);
	leer_heap(heap, PID);
    return espacio_de_HEAP(heap);
}

uint32_t traducir_a_dir_fisica(uint32_t PID, uint32_t ptroHEAP, uint32_t bitModificado){
	heap_metadata* heap = get_HEAP(PID, ptroHEAP);
	int nroPag = heap->currAlloc / get_tamanio_pagina();
	int offset = heap->currAlloc % get_tamanio_pagina();
	uint32_t framePag = obtener_marco_de_pagina_en_memoria(PID, nroPag, bitModificado);
	uint32_t ptroEscritura = framePag * get_tamanio_pagina() + offset;
	return ptroEscritura;
}


bool ptro_liberado(uint32_t PID, uint32_t ptro){

	t_list* lista_heaps = conseguir_listaHMD_mediante_PID(PID);

	bool condicion(void* heap_i) {
		heap_metadata* heap = (heap_metadata*) heap_i;
		leer_heap(heap, PID);
		return (heap->currAlloc + 9) == ptro;
	}

	heap_metadata* heap_encontrado = (heap_metadata*) list_find(lista_heaps, condicion);

	return heap_encontrado -> isFree;
}


void liberar_memoria(uint32_t PID, uint32_t ptro){
	heap_metadata* heap_encontrado = get_HEAP(PID, ptro);
	heap_encontrado ->isFree = 1;
}

void consolidar_memoria(uint32_t PID){


	t_list* heaps = conseguir_listaHMD_mediante_PID(PID);
	uint32_t indice=0;

	uint32_t frees_contiguos = cantidad_de_frees_contiguos(heaps, &indice);


	switch(frees_contiguos) {
	case 1:
		//no hay nada para consolidar, logguear
		loggear_info("[MATELIB_MEM_FREE] La memoria no tiene nada para consolidar");
		break;
	case 2:
		//caso en el que modifico el heap del que parto y el siguiente del siguiente
		loggear_info("[MATELIB_MEM_FREE] Se va a consolidar el caso LL");
		modificar_heaps(heaps, indice, PID, 2);
		break;
	case 3:
		//caso en el que puede que modifique mas de uno
		loggear_info("[MATELIB_MEM_FREE] Se va a consolidar el caso LLL");
		modificar_heaps(heaps, indice, PID,3);
		break;

	}
	heap_metadata* ultimo_heap = (heap_metadata*)list_get(heaps, list_size(heaps) - 1);
	t_list* tabla_paginas = obtener_tabla_paginas_mediante_PID(PID);

	if(el_ultimo_heap_libera_paginas(ultimo_heap)){
		liberar_paginas(ultimo_heap, tabla_paginas, PID);
	}

}

void liberar_paginas(heap_metadata* ultimo_heap, t_list* tp, uint32_t pid) {

	uint32_t cantPagNOBORRAR = ultimo_heap->currAlloc/get_tamanio_pagina();

	if(((ultimo_heap->currAlloc)%get_tamanio_pagina()) > 0){
	  cantPagNOBORRAR++;
	}

	uint32_t paginasUsadas = list_size(tp);
	uint32_t cantPagABorrar = paginasUsadas - cantPagNOBORRAR;

	t_pagina* paginaEliminar;

	for(int i = 0; i < cantPagABorrar; i++){
	        paginaEliminar = list_get(tp, cantPagNOBORRAR);
	        if(paginaEliminar->bit_presencia==1){
	            t_frame* frameLiberado = list_get(listaFrames, paginaEliminar);
	            frameLiberado->estado=0;
	        }
	        list_remove_and_destroy_element(tp, cantPagNOBORRAR, free);
	    }

	size_t tamanio;
	t_pedir_o_liberar_pagina_s* mensaje = shared_crear_pedir_o_liberar(pid, cantPagABorrar);
	void* mensaje_serializado = serializar_liberar_pagina(mensaje, &tamanio);

	pthread_mutex_lock(&mutex_enviar_mensaje_swap);
	enviar_mensaje_protocolo(socket_swap, R_S_LIBERAR_PAGINA, tamanio, mensaje_serializado);
	t_prot_mensaje* respuesta = recibir_mensaje_protocolo(socket_swap);
	pthread_mutex_unlock(&mutex_enviar_mensaje_swap);

	free(mensaje_serializado);
    if(respuesta->head == FALLO_EN_LA_TAREA){
        loggear_error("[RAM] - Hubo un problema en la liberacion de la paginas del proceos %d en swamp", pid);
    }
}

t_list* obtener_tabla_paginas_mediante_PID(uint32_t PID){
    t_proceso* proceso = get_proceso_PID(PID);
    return proceso->tabla_paginas;
}

bool el_ultimo_heap_libera_paginas(heap_metadata* ultimo_heap){

	uint32_t tamanio = espacio_de_HEAP(ultimo_heap);
	return tamanio > get_tamanio_pagina();

}

void modificar_heaps(t_list* heaps, uint32_t indice, uint32_t pid, uint32_t cantidad){

	heap_metadata* heap_inicial = (heap_metadata*) list_get(heaps, indice);

	heap_metadata* heap_final = (heap_metadata*) list_get(heaps, MIN(indice+cantidad,list_size(heaps)-1));

	if(heap_final->nextAlloc == -1){

		heap_inicial->nextAlloc = -1;

	}else{
		heap_inicial->nextAlloc = heap_final->currAlloc;
		heap_final->prevAlloc = heap_inicial->currAlloc;
	}


	for(int i = 0; i < cantidad -1; i++){

		list_remove_and_destroy_element(heaps, indice+1, free);

	}


	guardar_HEAP_en_memoria(pid, heap_inicial);

	//TODO: esto esta bien??
	if(heap_final->nextAlloc == -1) {
		guardar_HEAP_en_memoria(pid, heap_final);//verificar que  heap_inicial != -1 para hacer esto
	}




}


uint32_t cantidad_de_frees_contiguos(t_list* lista_heaps, uint32_t* indice) {

	uint32_t cantidad = 1;
	bool no_encontrado = true;
	while(*indice < (list_size(lista_heaps) - 1)  && no_encontrado) {

		heap_metadata* heap = (heap_metadata*)list_get(lista_heaps, *indice);
		if(heap->isFree) {
			cantidad = calcular_seguidos(lista_heaps, *indice);
				if(cantidad >=2) {
					no_encontrado = false;
				(*indice)--;
			}
		}
		(*indice)++;
	}

	return cantidad;


}

uint32_t calcular_seguidos(t_list* lista, uint32_t indice) {

	uint32_t s = 0;
	int i = indice;
	heap_metadata* obtenido = (heap_metadata*) list_get(lista, indice);
	while(i < list_size(lista) && obtenido->isFree){
		i++;
		obtenido = (heap_metadata*) list_get(lista, MIN(i,list_size(lista)-1));
		s++;

	}


	return s;

}
void* leer_heap(heap_metadata* heap, uint32_t PID){

	int nroPag = calcular_pagina_de_puntero_logico(heap->currAlloc);
	int offset = calcular_offset_puntero_en_pagina(heap->currAlloc);
	void* dataLeida = leer_de_memoria_paginada(PID, nroPag, offset, 9);
	return dataLeida;
}


void* leer_de_memoria(int32_t ptroHEAP, uint32_t PID, uint32_t tamanioALeer){

	heap_metadata* heap = get_HEAP(PID, ptroHEAP);
	int nroPag = calcular_pagina_de_puntero_logico(heap->currAlloc);
	int offset = calcular_offset_puntero_en_pagina(heap->currAlloc);
	void* dataLeida = leer_de_memoria_paginada(PID, nroPag, offset, tamanioALeer);
	return dataLeida;
}

void eliminar_proceso(uint32_t PID){
	t_proceso* proceso = remover_proceso_PID_lista_procesos(PID);

	liberar_frames_eliminar_proceso(proceso);

	comunicar_eliminacion_proceso_SWAP(PID);

	destruir_proceso(proceso);

}

void suspender_proceso(uint32_t PID){
	t_proceso* proceso = get_proceso_PID(PID);

	t_pagina* paginaIterada;
	t_frame* frameLiberar;
	void* data;

	for(int i=0; i<list_size(proceso->tabla_paginas); i++){
		paginaIterada = list_get(proceso->tabla_paginas,i);

		if(paginaIterada->bit_presencia){
			pthread_mutex_lock(&mutex_acceso_lista_frames);
			frameLiberar = list_get(listaFrames, paginaIterada->frame);
			frameLiberar->estado=0;
			 pthread_mutex_unlock(&mutex_acceso_lista_frames);
			if(paginaIterada->bit_modificacion){
				data = malloc(get_tamanio_pagina());
				leer_directamente_de_memoria(data, get_tamanio_pagina(), paginaIterada->frame * get_tamanio_pagina());
				enviar_pagina_a_SWAP(PID, i, data);
				free(data);
			}
		}

	}

	if (get_tipo_asignacion() == FIJA){
		eliminar_frames_reservados(proceso);
	}
}





//----------------------------------FUNCIONES PRIVADAS DE USO INTERNO----------------------------------

uint32_t calcular_pagina_de_puntero_logico(uint32_t puntero){

	return puntero/get_tamanio_pagina();
}

uint32_t calcular_offset_puntero_en_pagina(uint32_t puntero) {
	return puntero%get_tamanio_pagina();
}

void escribir_en_memoria(uint32_t pid, void* valor, uint32_t size, uint32_t puntero) {

	uint32_t nro_pag = calcular_pagina_de_puntero_logico(puntero);
	uint32_t offset = calcular_offset_puntero_en_pagina(puntero);
	guardar_en_memoria_paginada(pid, nro_pag, offset, valor, size);

}

t_proceso* get_proceso_PID(uint32_t PID){

	bool proceso_PID(void* element) {
			t_proceso* proceso = (t_proceso*) element;
			return proceso->PID == PID;
		}

	t_proceso* proceso = list_find(listaProcesos, proceso_PID);
	return proceso;

}


uint32_t espacio_de_HEAP(heap_metadata* heap){
	return heap->nextAlloc - heap->currAlloc - 9;
}


heap_metadata* get_HEAP(uint32_t PID, int32_t ptro){

	bool heap_ptro(void* element){
		heap_metadata* heap = (heap_metadata*) element;
		return heap->currAlloc == (ptro-9);

	}

	t_list* listaHMD = conseguir_listaHMD_mediante_PID(PID);
	heap_metadata* heap = list_find(listaHMD,heap_ptro);
	return heap;

}

t_list* conseguir_listaHMD_mediante_PID(uint32_t PID){
    t_proceso* proceso = get_proceso_PID(PID);

    t_list* listaHMD = proceso->lista_hmd;
    return listaHMD;
}

void agregar_HEAP_a_PID(uint32_t PID, heap_metadata* heap){

	t_list* listaHMD = conseguir_listaHMD_mediante_PID(PID);

	bool condicion(void* elementoListado, void* datoAInsertar){
		heap_metadata* elemList = (heap_metadata*) elementoListado;
		heap_metadata* elemInsertar = (heap_metadata*) datoAInsertar;

		return elemList->currAlloc < elemInsertar->currAlloc;
	}

	list_add_sorted(listaHMD, (void*) heap, condicion);

}

void* leer_de_memoria_paginada(uint32_t PID, int nroPag, int offset, int tamDato){
	int desplazamientoEnDato = 0;
	uint32_t marcoPag;
	int ptro_escritura;
	void* data = malloc(tamDato);
	while(tamDato>0){
		marcoPag = obtener_marco_de_pagina_en_memoria(PID, nroPag, 0);
		ptro_escritura = marcoPag * get_tamanio_pagina() + offset;
		if((offset+tamDato) <= get_tamanio_pagina()){
			leer_directamente_de_memoria(data + desplazamientoEnDato, tamDato, ptro_escritura);
			tamDato=0;
		}else{
			int tamDatoParcial = get_tamanio_pagina()- offset;
			leer_directamente_de_memoria(data + desplazamientoEnDato, tamDatoParcial, ptro_escritura);
			desplazamientoEnDato += tamDatoParcial;
			tamDato -= tamDatoParcial;
			offset = 0;
		}
	}
	return data;
}

void guardar_HEAP_en_memoria(uint32_t PID, heap_metadata* heap){
	int nroPag = heap->currAlloc / get_tamanio_pagina();
	int offset = heap->currAlloc % get_tamanio_pagina();
	void* dataHeap = serializar_HEAP(heap);
	guardar_en_memoria_paginada(PID, nroPag, offset, dataHeap, 9);
	free(dataHeap);
}

void guardar_en_memoria_paginada(uint32_t PID, int nroPag, int offset, void* data, int tamDato){
	int desplazamientoEnDato = 0;
	uint32_t marcoPag;
	int ptro_escritura;
	while(tamDato>0){
		marcoPag = obtener_marco_de_pagina_en_memoria(PID, nroPag, 1);
		ptro_escritura = marcoPag * get_tamanio_pagina() + offset;
		if((offset+tamDato) <= get_tamanio_pagina()){
			escribir_directamente_en_memoria(data + desplazamientoEnDato, tamDato, ptro_escritura);
			tamDato=0;
		}else{
			int tamDatoParcial = get_tamanio_pagina()- offset;
			escribir_directamente_en_memoria(data + desplazamientoEnDato, tamDatoParcial, ptro_escritura);
			desplazamientoEnDato += tamDatoParcial;
			tamDato -= tamDatoParcial;
			offset = 0;
		}
	}
}

uint32_t obtener_marco_de_pagina_en_memoria(uint32_t PID, int nroPag, uint32_t bitModificado){
	uint32_t marco;
	if(esta_en_tlb(PID, nroPag)){
		actualizar_datos_TLB(PID, nroPag);
		marco = obtener_frame_de_tlb(PID, nroPag);
		actualizar_datos_pagina(PID, nroPag, bitModificado, 1);
		loggear_debug("[RAM] - TLB HIT para Proceso %d Pagina %d en el marco %d", PID, nroPag, marco);
	}else{
		loggear_debug("[RAM] - TLB MISS para Proceso %d Pagina %d", PID, nroPag);
		if(esta_en_RAM(PID, nroPag)){
			marco = obtener_frame_de_RAM(PID, nroPag);
			actualizar_datos_pagina(PID, nroPag, bitModificado, 0);
		}else{
			marco = traer_pagina_de_SWAP(PID, nroPag);//carga los frames con los datos necesarios, elige victima y cambia paginas, actualiza pagina victima. Tmbn tiene que actualizar la cant de Pags en asig FIJA
			inicializar_datos_pagina(PID, nroPag, marco, bitModificado);//podriamos poner esta funcion dentro de obtener fram asi tmbn se encarga de modificar lo administrativo dsps del cambio de pags?
			loggear_debug("[RAM] - TLB HIT para Proceso %d Pagina %d en el marco %d", PID, nroPag, marco);
		}
		agregar_entrada_tlb(PID, nroPag, marco);
	}
	return marco;
}

void* serializar_HEAP(heap_metadata* heap){//TODO revisar serializacion
	void * dataHEAP = malloc(sizeof(heap_metadata));
	memcpy(dataHEAP, &heap->prevAlloc,4);
	memcpy(dataHEAP+4, &heap->nextAlloc,4);
	memcpy(dataHEAP+8, &heap->isFree,1);
	return dataHEAP;
}

bool esta_en_RAM(uint32_t PID, uint32_t nroPag){

	t_list* tabla_paginas = obtener_tabla_paginas_mediante_PID(PID);
	t_pagina* pag = (t_pagina*) list_get(tabla_paginas, nroPag);
	return pag->bit_presencia == 1 ? true : false;
}

uint32_t obtener_frame_de_RAM(uint32_t PID, uint32_t nroPag){

	t_list* tabla_paginas = obtener_tabla_paginas_mediante_PID(PID);
	t_pagina* pag = (t_pagina*) list_get(tabla_paginas, nroPag);
	return pag->frame;
}

void actualizar_datos_pagina(uint32_t PID, uint32_t nroPag, uint32_t bitModificado, uint32_t bitTLB){
	t_proceso* proceso = get_proceso_PID(PID);
	if(bitTLB){
		proceso->hits_proceso++;
	}else{
		proceso->miss_proceso++;
	}
	t_pagina* pag = list_get(proceso->tabla_paginas, nroPag);
	pag->timestamp = obtener_timestamp_actual();
	pag->bit_uso = 1;
	if(bitModificado){
		pag->bit_modificacion = bitModificado;
	}
}

void inicializar_datos_pagina(uint32_t PID, uint32_t nroPag, uint32_t marco, uint32_t bitModificado){
	t_proceso* proceso = get_proceso_PID(PID);
	proceso->miss_proceso++;
	proceso->hits_proceso++;
	t_pagina* pag = list_get(proceso->tabla_paginas, nroPag);
	pag->frame = marco;
	pag->timestamp = obtener_timestamp_actual();
	pag->bit_uso = 1;
	pag->bit_modificacion = bitModificado;
}

uint32_t calcular_tamanio_ultimo_HEAP(uint32_t PID){
    t_proceso* proceso = get_proceso_PID(PID);
    uint32_t extra;
    int tamanioProceso = list_size(proceso->tabla_paginas) * get_tamanio_pagina();
    t_list* listaHMD = conseguir_listaHMD_mediante_PID(PID);
    loggear_trace("casi llego");
    if(listaHMD == NULL){
        loggear_info("la lista es nula");
    }
    heap_metadata* ultimoHeap;
    loggear_trace("casi llegox2");
    int tamanio_lista = list_size(listaHMD);
    loggear_trace("casi llegox3 %d", tamanio_lista);
    if(tamanio_lista == 0){
    extra = 0;
    loggear_trace("casi llegox4 %d", tamanio_lista);
    }else{
    ultimoHeap = list_get(listaHMD, list_size(listaHMD)-1);
extra = ultimoHeap->currAlloc;
    loggear_trace("casi llegox5 %d", tamanio_lista);
    }
    loggear_trace("no llgue");

    return tamanioProceso - extra;
}

void actualizar_cantidad_frames_por_proceso_RAM(uint32_t PID, int32_t modCant){
	char* keyProceso = calcular_hash_key_dic(PID);
	pthread_mutex_lock(&mutex_acceso_diccionario);
	asignacion_marcos_fijos* asigFija = (asignacion_marcos_fijos*) dictionary_get(cant_frames_por_proceso , keyProceso);
	if(modCant>0){
		if(MIN(asigFija->en_mp, get_marcos_maximos())<get_marcos_maximos()){
			asigFija->en_mp=+1;
		}
	}else{
		asigFija->en_mp=-1;
	}
	pthread_mutex_unlock(&mutex_acceso_diccionario);
}

char* calcular_hash_key_dic(uint32_t proceso) {
	char** key = string_from_format("%d",proceso);
	return *key;
}

int calcular_paginas_para_tamanio(uint32_t tam) {
	int cantPags;
	cantPags = (tam + 18) / get_tamanio_pagina();
		if((tam+18)%get_tamanio_pagina() > 0){
			cantPags++;
		}
		return cantPags;
}

void reservar_frames(t_list* lista_frames_proceso){

	bool frame_disponible_y_no_repetidos_en_lista(void* element){

		return frame_disponible(element) && frame_no_pertenece_a_lista(listaFramesReservados, element);
	}

	for(int i=0; i<get_marcos_maximos();i++){
		pthread_mutex_lock(&mutex_acceso_lista_frames_r);
		pthread_mutex_lock(&mutex_acceso_lista_frames);
		t_frame* frame = (t_frame*)list_find(listaFrames, frame_disponible_y_no_repetidos_en_lista);
		pthread_mutex_unlock(&mutex_acceso_lista_frames);

		list_add(lista_frames_proceso, frame);
		list_add(listaFramesReservados, frame);

		pthread_mutex_unlock(&mutex_acceso_lista_frames_r);
	}
}

bool frame_no_pertenece_a_lista(t_list* lista_frames, void* elementoBuscado){

	t_frame* frame = (t_frame*) elementoBuscado;

	bool frame_es_elemento(void* elemento){
		t_frame* frameIterado = (t_frame*) elemento;
		return frame->nroFrame == frameIterado->nroFrame;
	}

	return !list_any_satisfy(lista_frames, frame_es_elemento);
}


t_proceso* remover_proceso_PID_lista_procesos(uint32_t PID){
	bool proceso_PID(void* element) {
		t_proceso* proceso = (t_proceso*) element;
		return proceso->PID == PID;
	}

	t_proceso* procesoRemovido = list_remove_by_condition(listaProcesos, proceso_PID);
	return procesoRemovido;
}

void liberar_frames_eliminar_proceso(t_proceso* proceso){

	t_pagina* paginaIterada;
	t_frame* frameLiberar;

	for(int i=0; i<list_size(proceso->tabla_paginas); i++){
		paginaIterada = list_get(proceso->tabla_paginas,i);

		if(paginaIterada->bit_presencia){
			pthread_mutex_lock(&mutex_acceso_lista_frames);
			frameLiberar = list_get(listaFrames, paginaIterada->frame);
			frameLiberar->estado=0;
			pthread_mutex_unlock(&mutex_acceso_lista_frames);
		}

	}

	if (get_tipo_asignacion() == FIJA){
		eliminar_frames_reservados(proceso);
	}

}

void eliminar_frames_reservados(t_proceso* proceso){

	t_frame* frame;

	for(int i=0; i<list_size(proceso->lista_frames_reservados); i++){
		frame = list_get(proceso->lista_frames_reservados,i);

		bool frame_numero(void* element) {
			t_frame* frameListaReservada = (t_frame*) element;
			return frameListaReservada->nroFrame == frame->nroFrame;
		}
		pthread_mutex_lock(&mutex_acceso_lista_frames_r);
		list_remove_by_condition(listaFramesReservados, frame_numero);
		pthread_mutex_unlock(&mutex_acceso_lista_frames_r);
	}
}
