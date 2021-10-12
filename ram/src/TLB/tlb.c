#include "tlb.h"

void inicializar_tlb() {
	//COSA IMPORTANTE: POR DEFAULT LAS COMMONS CREAN UN DICCIONARIO CON 20 ENTRADAS.
	// a. modifico las commons -> no, me caga si usan otra verision
	// b. copio el codigo y modifico -> hago esto
	TLB = dictionary_create();
	//METER LOG DE TLB CREADA EXITOSAMETNE
}

void agregar_entrada_tlb(uint32_t proceso, uint32_t pagina, uint32_t frame) {

	char* key = calcular_hash_key(proceso, pagina);
	//TODO
	entrada_tlb* entrada = malloc(sizeof(entrada_tlb));
	entrada->timestamp = obtener_timestamp_actual();
	entrada->frame = frame; //aca ver como obtener la direccion -> podría venir por parametro???

	dictionary_put(TLB, key, &entrada);

}
char* calcular_hash_key(uint32_t proceso, uint32_t pagina) {
	char** key = string_from_format("%d-%d",proceso, pagina);
	return *key;

}

bool esta_en_tlb(uint32_t pid, uint32_t pag) {
	char* key = calcular_hash_key(pid, pag);
	return dictionary_has_key(TLB, key);
}

void reemplazar_entrada_tlb(entrada_tlb* entrada, uint32_t indice) {
	//TODO
}

uint32_t obtener_direccion_de_tlb(char* key){
	return *(uint32_t*)dictionary_get(TLB, key);
}

double obtener_timestamp_actual(){

	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long long result = (((unsigned long long)tv.tv_sec) * 1000 + ((unsigned long long)tv.tv_usec) / 1000);
	double a = result;
	return a;
}

