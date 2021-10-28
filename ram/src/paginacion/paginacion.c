#include "paginacion.h"



// FUNCIONES DE INICIO Y DESTRUCCION ADMINISTRATIVAS

void inicializar_estructuras_administrativas() {
    listaProcesos = list_create();
    listaFrames = list_create();
    cant_frames_por_proceso = dictionary_create();

}

void destruir_estructuras_administativas() {
    list_destroy_and_destroy_elements(listaProcesos, destruir_proceso);
    list_destroy_and_destroy_elements(listaFrames, free);
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

	t_list* listaHMD = conseguir_listaHMD_mediante_PID(PID);

	return get_ptro_con_tam_min(listaHMD, tam);

}

void actualizar_proceso(uint32_t PID, int32_t ptro, uint32_t tamanio){

	//modificamos el HEAP que vamos ocupar y actualizamos en mem
	heap_metadata* heap = get_HEAP(PID,ptro);
	int nextNextAlloc = heap->nextAlloc;
	heap->nextAlloc = ptro+tamanio;
	heap->isFree = 0;
	guardar_HEAP_en_memoria(PID, heap);

	//creamos el HEAP nuevo que vamos a ingresar y actualizamos en mem
	heap_metadata* nuevoHeap = malloc(sizeof(heap_metadata));
	nuevoHeap->currAlloc = heap->nextAlloc;
	nuevoHeap->prevAlloc = heap->currAlloc;
	nuevoHeap->nextAlloc = nextNextAlloc;
	nuevoHeap->isFree    = 1;
	agregar_HEAP_a_PID(PID,nuevoHeap);
	guardar_HEAP_en_memoria(PID, nuevoHeap);

	//si no es el ultimo alloc, traemos el sig HEAP para modificarlo y actualizamos en mem
	if(heap->nextAlloc == -1){
		heap_metadata* heapSig = get_HEAP(PID,nuevoHeap->nextAlloc);
		heapSig->prevAlloc = nuevoHeap->currAlloc;
		guardar_HEAP_en_memoria(PID, heapSig);
	}
}


int32_t no_se_asigna_proceso(uint32_t pid, uint32_t size) {
	if(get_tipo_asignacion() == FIJA) {
		//fijo
		return MAXIMO_MARCOS_ALCANZADO;
	}else {
		return ESPACIO_EN_MEMORIA_INSUF;
	}
}

int32_t agregar_proceso(uint32_t PID, uint32_t tam){
	//creo el proceso y lo inicializo
	t_proceso* nuevoProceso = malloc(sizeof(t_proceso));
	nuevoProceso->PID = PID;
	nuevoProceso->tabla_paginas = list_create();
	nuevoProceso->lista_hmd = list_create();
	nuevoProceso->hits_proceso = 0;
	nuevoProceso->miss_proceso = 0;


	//agrego las paginas que me dieron
	int cantPags = (tam + 9) / get_tamanio_pagina();
	if((tam+9)%get_tamanio_pagina() > 0){
		cantPags++;
	}
	t_pagina* nuevaPagina;
	for(int i = 0;i<cantPags;i++){
		nuevaPagina = malloc(sizeof(nuevaPagina));
		nuevaPagina->bit_presencia=0;//es el unico dato que llenamos
		list_add(nuevoProceso->tabla_paginas,nuevaPagina);
	}


	//creamos el primer HEAP nuevo que vamos a ingresar y actualizamos en mem
	heap_metadata* nuevoHeapPrimero = malloc(sizeof(heap_metadata));
	nuevoHeapPrimero->currAlloc = 0;
	nuevoHeapPrimero->prevAlloc = -1;
	nuevoHeapPrimero->nextAlloc = tam + 9;
	nuevoHeapPrimero->isFree    = 0;
	agregar_HEAP_a_PID(PID,nuevoHeapPrimero);
	guardar_HEAP_en_memoria(PID, nuevoHeapPrimero);


	//creamos el segundo HEAP nuevo que vamos a ingresar y actualizamos en mem
	heap_metadata* nuevoHeapUltimo = malloc(sizeof(heap_metadata));
	nuevoHeapUltimo->currAlloc = tam+9;
	nuevoHeapUltimo->prevAlloc = 0;
	nuevoHeapUltimo->nextAlloc = -1;
	nuevoHeapUltimo->isFree    = 1;
	agregar_HEAP_a_PID(PID,nuevoHeapUltimo);
	guardar_HEAP_en_memoria(PID, nuevoHeapUltimo);


	//agrego proceso a la lista
	list_add(listaProcesos, nuevoProceso);

	return nuevoHeapPrimero->currAlloc+9;//siempre el primer alloc va a ser 9 porque el primer dato se guarda al comienzo, y el metadata ocupa 9 bytes
}

int32_t se_puede_almacenar_el_alloc_para_proceso(t_header header, uint32_t pid, uint32_t size) {
	//funcion cuando un proceso pide memoria
	//consguir socket
	void* mensaje = serializar_pedido_memoria(pid, size);

	//semaforo_socket
	int a = enviar_mensaje_protocolo(1,header, 8, mensaje);
	if (a < 0 ) {
		//imprimir que hubo un pronlema
	}
	//como consigo el socket del swap?
	t_prot_mensaje* respuesta = recibir_mensaje_protocolo(1);
	//semaforo_socket
	uint32_t respuesta_final = *(uint32_t*)(respuesta->payload);
	//loggear la respuesta
	free(mensaje);

	return respuesta_final;
}



bool ptro_valido(uint32_t PID, uint32_t ptro) {

	t_list* lista_heaps = conseguir_listaHMD_mediante_PID(PID);

	bool condition(void* heap) {
		heap_metadata* heap_md = (heap_metadata*) heap;
		return (heap_md->currAlloc + 9) == ptro;
	}

	return list_any_satisfy(lista_heaps,condition);
}

uint32_t tamanio_de_direccion(uint32_t direccionLogicaALeer, uint32_t PID){
	heap_metadata* heap = get_HEAP(PID, direccionLogicaALeer);
    return espacio_de_HEAP(heap);
}

uint32_t traducir_a_dir_fisica(uint32_t PID, uint32_t ptroHEAP, uint32_t bitModificado){
	heap_metadata* heap = get_HEAP(PID, ptroHEAP);
	int nroPag = heap->currAlloc / get_tamanio_pagina();
	int offset = heap->currAlloc % get_tamanio_pagina();
	t_pagina* pag = obtener_pagina_de_memoria(PID, nroPag, bitModificado);
	uint32_t ptroEscritura = pag->frame * get_tamanio_pagina() + offset;
	return ptroEscritura;
}


bool ptro_liberado(uint32_t PID, uint32_t ptro){

	t_list* lista_heaps = conseguir_listaHMD_mediante_PID(PID);

	bool condicion(void* heap_i) {
		heap_metadata* heap = (heap_metadata*) heap_i;
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
		loggear_info("[MEMORIA] - La memoria no tiene nada para consolidar");
		break;
	case 2:
		//caso en el que modifico el heap del que parto y el siguiente del siguiente
		modificar_heaps(heaps, indice, PID, 2);
		break;
	case 3:
		//caso en el que puede que modifique mas de uno
		modificar_heaps(heaps, indice, PID,3);
		break;

	}
	heap_metadata* ultimo_heap = (heap_metadata*)list_get(heaps, list_size(heaps) - 1);
	t_list* tabla_paginas = obtener_tabla_paginas_mediante_PID(PID);

	if(el_ultimo_heap_libera_paginas(ultimo_heap)){
		liberar_paginas(ultimo_heap, tabla_paginas);
	}

}

void liberar_paginas(heap_metadata* ultimo_heap, t_list* tp) {


	uint32_t cantPagNOBORRAR = ultimo_heap->currAlloc/get_tamanio_pagina();

	if(((ultimo_heap->currAlloc)%get_tamanio_pagina()) > 0){
	  cantPagNOBORRAR++;
	}

	uint32_t paginasUsadas = list_size(tp);
	uint32_t cantPagABorrar = paginasUsadas - cantPagNOBORRAR;

	for(int i = 0; i < cantPagABorrar; i++){
		list_remove_and_destroy_element(tp, cantPagNOBORRAR, free);
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
	heap_metadata* heap_final = (heap_metadata*) list_get(heaps, indice+cantidad);

	heap_inicial->nextAlloc = heap_final->currAlloc;
	heap_final->prevAlloc = heap_inicial->currAlloc;

	for(int i = 0; i < cantidad - 1; i++){

		list_remove_and_destroy_element(heaps, indice + cantidad - i, free);

	}


	guardar_HEAP_en_memoria(pid, heap_inicial);
	guardar_HEAP_en_memoria(pid, heap_final);



}


uint32_t cantidad_de_frees_contiguos(t_list* lista_heaps, uint32_t* indice) {

	uint32_t cantidad = 1;
	bool no_encontrado = true;
	while(*indice < (list_size(lista_heaps) - 1)  && no_encontrado) {

		heap_metadata* heap = (heap_metadata*)list_get(lista_heaps, *indice);

		if(!heap->isFree) {
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
		obtenido = (heap_metadata*) list_get(lista, i);
		s++;

	}


	return s;

}

// FUNCIONES PRIVADAS DE USO INTERNO

t_proceso* get_proceso_PID(uint32_t PID){

	bool proceso_PID(void* element) {
			t_proceso* proceso = (t_proceso*) element;
			return proceso->PID == PID;
		}

	t_proceso* proceso = list_find(listaProcesos, proceso_PID);
	return proceso;

}

int32_t get_ptro_con_tam_min(t_list* listaHMD, uint32_t tam){

	int32_t ptro;

	bool heap_tam_min(void* element){
		bool rta;
		heap_metadata* heap = (heap_metadata*) element;
		if(heap->nextAlloc==-1){
			rta = true;
		}else{
			if(espacio_de_HEAP(heap)>= tam+9){
				rta = true;
			}else{
				rta = false;
			}
		}
		return rta;
	}

	heap_metadata* heap = list_find(listaHMD,heap_tam_min);

	ptro = heap->currAlloc + 9;

	if(heap->nextAlloc==-1){
		ptro = (-1)* ptro;
	}

	return ptro;

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

	bool condicion(void* elementoListado, void* datoAInsertar){
		heap_metadata* elemList = (heap_metadata*) elementoListado;
		heap_metadata* elemInsertar = (heap_metadata*) datoAInsertar;

		return elemList->currAlloc < elemInsertar->currAlloc;
	}

	t_list* listaHMD = conseguir_listaHMD_mediante_PID(PID);

	list_add_sorted(listaHMD, (void*) heap, condicion);
	//TODO testear que el insert se haga en la posicion correcta

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
	t_pagina* pag;
	int ptro_escritura;
	while(tamDato>0){
		pag = obtener_pagina_de_memoria(PID, nroPag, 1);
		ptro_escritura = pag->frame * get_tamanio_pagina() + offset;
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

t_pagina* obtener_pagina_de_memoria(uint32_t PID, int nroPag, uint32_t bit_modificado){
	//se fija si esta en la TLB
	// 		- si está: actualiza datos de entrada TLB (timestamp) y los datos de la pagina (timestamp/ bit_uso, bit_modificacion )
	//		- no está: va a buscar a la RAM
	//					- SI está: agrega entrada a la TLB y actualizo datos de pagina (timestamp/ bit_uso, bit_modificacion)
	//					- NO está: traigo la PAG de SWAP (segun asignacion), agrego entrada a la TLB y actualizo datos de pagina (timestamp/ bit_uso, bit_modificacion)
	t_pagina* a = NULL;
	return a;
}

void* serializar_HEAP(heap_metadata* heap){//TODO revisar serializacion
	void * dataHEAP = malloc(sizeof(heap_metadata));
	memcpy(dataHEAP, &heap->prevAlloc,4);
	memcpy(dataHEAP+4, &heap->nextAlloc,4);
	memcpy(dataHEAP+8, &heap->isFree,1);
	return dataHEAP;
}



