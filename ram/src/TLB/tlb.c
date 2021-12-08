#include "tlb.h"







//	FUNCIONES PUBLICAS

void inicializar_tlb() {
	TLB = list_create();
	max_entradas = get_cantidad_entradas_tlb();
	loggear_debug("[RAM] - TLB creada exitosamente");
}

void limpiar_tlb(){
	for(int i = 0; i<list_size(TLB);i++){
		eliminar_entrada(0);
	}
}

void imprimir_tlb(){

	loggear_trace("----------------------------------------------------------");
	loggear_trace("-----------------VOY A IMPRIMIR LA TLB--------------------");
	loggear_trace("----------------------------------------------------------");
	for(int i = 0; i < list_size(TLB); i++) {
	entrada_tlb* entrada = (entrada_tlb*) list_get(TLB,i);
	loggear_trace("ENTRADA: %d | KEY: %s | FRAME: %d | TIMESTAMP %f", i, entrada->hash_key, entrada->frame, entrada->timestamp);

}
	loggear_trace("----------------------------------------------------------");
	loggear_trace("-----------------------I'M DONE---------------------------");
	loggear_trace("----------------------------------------------------------");
}

void agregar_entrada_tlb(uint32_t proceso, uint32_t pagina, uint32_t frame) {

	if(max_entradas > 0) {

		char* hash_KEY_del_ingresante = calcular_hash_key(proceso, pagina);

		if(existe_la_KEY(hash_KEY_del_ingresante)){

			bool soy_KEY_buscada(void* element){
				entrada_tlb* elemento_iterado = (entrada_tlb*) element;
				return strcmp(elemento_iterado->hash_key, hash_KEY_del_ingresante) == 0;
			}

			entrada_tlb* elemento_encontrado = list_find(TLB, soy_KEY_buscada);
			elemento_encontrado->timestamp =  obtener_timestamp_actual();
			elemento_encontrado->frame = frame;
		}else{
			entrada_tlb* entrada = malloc(sizeof(entrada_tlb));
			entrada->timestamp = obtener_timestamp_actual();
			entrada->frame = frame;
			entrada->hash_key = calcular_hash_key(proceso, pagina);
			if(max_entradas == list_size(TLB)) {
				uint32_t indice_victima = obtener_entrada_victima();
				loggear_trace("[TLB] - Voy a reemplazar la entrada %d", indice_victima);
				eliminar_entrada(indice_victima);
			}

			list_add(TLB, entrada);
		}
	}
}


void eliminar_entrada(uint32_t indice_victima) {

	list_remove_and_destroy_element(TLB, indice_victima, destructor_de_entradas);

}

uint32_t obtener_entrada_victima() {
	uint32_t victima;
	if(get_algoritmo_reemplazo_tlb() == FIFO){
		victima = 0;
	}else {
		victima =  conseguir_victima_entrada_LRU();
	}
	return victima;
}

uint32_t conseguir_victima_entrada_LRU() {

	uint32_t ultimo_usado = 0;
	entrada_tlb* entrada_anterior = (entrada_tlb*) list_get(TLB, 0);
	for (int i = 0; i < list_size(TLB); i++){

		entrada_tlb* entrada_actual = (entrada_tlb*) list_get(TLB,i);

		if (entrada_actual->timestamp < entrada_anterior->timestamp) {
			ultimo_usado = i;
		}
		entrada_anterior = entrada_actual;

	}

	return ultimo_usado;

}

bool esta_en_tlb(uint32_t pid, uint32_t pag) {

	if(max_entradas == 0){
		return false;
	}else {

	char* key = calcular_hash_key(pid, pag);

	bool condicion(void* entrada_i){

		entrada_tlb* entrada = (entrada_tlb*) entrada_i;

		if(strcmp(entrada->hash_key, key) == 0) {

			return true;
		}else {

			return false;
		}
		//return strcmp(entrada->hash_key, key) == 0 ? 1: 0;
	}

	return list_any_satisfy(TLB, condicion);
	}
}

void actualizar_datos_TLB(uint32_t PID, uint32_t nroPag){


	char* key = calcular_hash_key(PID, nroPag);

	bool buscar_key(void* entrada_i) {
			entrada_tlb* entrada_a = (entrada_tlb*) entrada_i;

			return strcmp(entrada_a->hash_key, key) == 0 ? 1: 0;
	}

		entrada_tlb* entrada_encontrada = (entrada_tlb*)list_find(TLB, buscar_key);

		entrada_encontrada->timestamp = obtener_timestamp_actual();

		free(key);


}

void destruir_tlb(){
	list_destroy_and_destroy_elements(TLB, destructor_de_entradas);
	loggear_debug("[RAM] - TLB destruida exitosamente");
}


// FUNCIONES PRIVADAS
char* calcular_hash_key(uint32_t proceso, uint32_t pagina) {
	char* key = string_from_format("%d-%d",proceso, pagina);
	return key;
}

int obtener_timestamp_actual(){

//	struct timespec timestamp;
//
//	clock_gettime(CLOCK_REALTIME, &timestamp);
//
//	return timestamp.tv_sec + 1e-9 * timestamp.tv_nsec;
	int valor;

	pthread_mutex_lock(&mutex_acceso_tiempo);
	valor = tiempo;
	tiempo++;
	pthread_mutex_unlock(&mutex_acceso_tiempo);
	return valor;
}

uint32_t obtener_frame_de_tlb(uint32_t proceso, uint32_t pagina){

	char* key = calcular_hash_key(proceso, pagina);
	bool buscar_key(void* entrada_i) {
		entrada_tlb* entrada_a = (entrada_tlb*) entrada_i;
		return strcmp(entrada_a->hash_key, key) == 0 ? 1: 0;
	}

	entrada_tlb* entrada_encontrada = (entrada_tlb*)list_find(TLB, buscar_key);
	return entrada_encontrada->frame;
	free(key);
}

bool existe_la_KEY (char* KEY){

	bool soy_KEY_buscada(void* element){
		entrada_tlb* elemento_iterado = (entrada_tlb*) element;
		return strcmp(elemento_iterado->hash_key, KEY) == 0;
	}

	return list_any_satisfy(TLB, soy_KEY_buscada);
}

void destructor_de_entradas(void* entrada) {
	entrada_tlb* e_tlb = (entrada_tlb*) entrada;
	free(e_tlb->hash_key);
	free(e_tlb);
}

