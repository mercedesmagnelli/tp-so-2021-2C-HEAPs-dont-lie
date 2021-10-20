#include "memoria.h"

void* memoria_principal;


int32_t memalloc(uint32_t pid, uint32_t size) {

	if (!cantidad_valida(size)) {
		//corto la ejecucion si ya no tengo que analizar
		return VALOR_MEMORIA_SOLICITADO_INVALIDO;
	} else {

		if (existe_el_proceso(pid)) {
			int32_t ptro = ptro_donde_entra_data(pid, size);
			if (ptro >= 0) {
				actualizar_proceso(pid, size);
				return ptro;
			} else {
				if (puedo_pedir_mas_memoria(pid, size)) {
					//como hay espacio disponble, expando lo que ya tenia
					actualizar_proceso(pid,  (-1) * ptro,  size);
					return (-1) * ptro;
				} else {
					return ESPACIO_EN_MEMORIA_INSUF;
				}

			}
		} else {
			//si no existe, entonces tengo que crear el nuevo proceso

			if (se_asigna_memoria_necesaria(pid, size)) {
				int32_t ptro_nuevo_proc = agregar_proceso(pid, size);
				return ptro_nuevo_proc;
			} else {
				return no_se_asigna_proceso(pid, size);
			}
		}

	}
}



void memfree(uint32_t* direccionLogicaALiberar) {

}

void* memread(uint32_t* direccionLogicaALeer, uint32_t tamanioALeer) {

	void* lectura = malloc(tamanioALeer);
	memcpy(lectura, direccionLogicaALeer, tamanioALeer);
	return lectura;
}

void memwrite(void* valorAEscribir, uint32_t* direccionLogicaAEscribir, uint32_t tamanioAEscribir){

	memcpy(direccionLogicaAEscribir, valorAEscribir, tamanioAEscribir);

}

void inicializar_memoria_principal() {
	int tamanio_memoria = get_tamanio_memoria();
	memoria_principal =  malloc(tamanio_memoria);


}


void consolidar(uint32_t indice_a_consolidar){

}

bool cantidad_valida(uint32_t size) {
	return size >=1 ? 1 : -1;
}

