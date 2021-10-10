#include "tlb.h"

void inicializar_tlb() {
	//COSA IMPORTANTE: POR DEFAULT LAS COMMONS CREAN UN DICCIONARIO CON 20 ENTRADAS.
	// a. modifico las commons -> no, me caga si usan otra verision
	// b. copio el codigo y modifico -> hago esto
	TLB = malloc(sizeof(t_dictionary));
	TLB->table_max_size = get_cantidad_entradas_tlb();
	TLB->elements = calloc(TLB->table_max_size, sizeof(t_hash_element*));
	TLB->table_current_size = 0;
	TLB->elements_amount = 0;
	//METER LOG DE TLB CREADA EXITOSAMETNE
}

void agregar_entrada_tlb(int proceso, int pagina, int frame) {

	char* key = calcular_hash_key(proceso, pagina);
	//TODO
	entrada_tlb entrada = malloc(sizeof(entrada_tlb));
	entrada.timestamp = 1;
	entrada.frame = frame; //aca ver como obtener la direccion -> podría venir por parametro???

	dictionary_put(TLB, key, &entrada);

}
char* calcular_hash_key(int proceso, int pagina) {
	char* key = string_from_format("%d-%d", proceso,pagina);
	return key;

}

bool esta_en_tlb(int pid, int pag) {
	char* key = calcular_hash_key(pid, pag);
	return dictionary_has_key(TLB, key);
}

void reemplazar_entrada_tlb(entrada_tlb* entrada, int indice) {
	//TODO
}

int obtener_direccion_de_tlb(char key){
	return *(int*)dictionary_get(TLB, key);
}

