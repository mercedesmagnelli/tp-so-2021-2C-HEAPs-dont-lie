#include "memoria.h"

void* memoria_principal;


int32_t memalloc(uint32_t pid, uint32_t size) {

	if (!cantidad_valida(size)) {
		//corto la ejecucion si ya no tengo que analizar
		return VALOR_MEMORIA_SOLICITADO_INVALIDO;
	} else {

		if (existe_proceso(pid)) {
			int32_t ptro = ptro_donde_entra_data(pid, size);
			if (ptro >= 0) {
				actualizar_proceso(pid,ptro,size);
				return ptro;
			} else {
				if (se_puede_almacenar_el_alloc_para_proceso(R_S_ESPACIO_PROCESO_EXISTENTE, pid, size)) {
					//como hay espacio disponble, expando lo que ya tenia
					actualizar_proceso(pid,  (-1) * ptro,  size);
					return (-1) * ptro;
				} else {
					return ESPACIO_EN_MEMORIA_INSUF;
				}

			}
		} else {
			//si no existe, entonces tengo que crear el nuevo proceso

			if (se_puede_almacenar_el_alloc_para_proceso(R_S_ESPACIO_PROCESO_NUEVO, pid, size)) {
				int32_t ptro_nuevo_proc = agregar_proceso(pid, size);
				return ptro_nuevo_proc;
			} else {
				return no_se_asigna_proceso(pid, size);
			}
		}

	}
}



int32_t memfree(uint32_t direccionLogicaALiberar, uint32_t pid) {

	if(ptro_valido(pid, direccionLogicaALiberar) || ptro_liberado(direccionLogicaALiberar,pid)){
		return -5; // MATE_FREE_FAULT
	}else{
		//Ya aca llegue a un punto donde el puntero puede liberarse
		liberar_memoria(pid, direccionLogicaALiberar);
		consolidar_memoria(pid);
		return 0; //ta to' bien
	}

}

void* memread(uint32_t direccionLogicaALeer, uint32_t pid, uint32_t tamanioALeer) {

    if(ptro_valido(pid, direccionLogicaALeer) || ptro_liberado(direccionLogicaALeer,pid) || tamanio_de_direccion(direccionLogicaALeer, pid) >= tamanioALeer){
        return (void*) -6; //MEM_READ_FAULT
    }else{
        void* lectura = malloc(tamanioALeer);
        memcpy(lectura, memoria_principal + traducir_a_dir_fisica(direccionLogicaALeer), tamanioALeer);
        return lectura;
    }
}

int32_t memwrite(void* valorAEscribir, uint32_t direccionLogicaAEscribir,uint32_t pid, uint32_t tamanioAEscribir){

    if(ptro_valido(pid, direccionLogicaAEscribir) || ptro_liberado(direccionLogicaAEscribir,pid) || tamanio_de_direccion(direccionLogicaAEscribir, pid) >= tamanioAEscribir){
            return -7; // MEM_WRITE_FAULT
    }else {
        escribir_directamente_en_memoria(valorAEscribir, tamanioAEscribir, traducir_a_dir_fisica(direccionLogicaAEscribir));
    	return 0;

    }
}

void escribir_directamente_en_memoria(void* valorAEscribir, int32_t tamanio, uint32_t direccionLogica){
	memcpy(memoria_principal + direccionLogica, valorAEscribir, tamanio);

}

void inicializar_memoria_principal() {
	int tamanio_memoria = get_tamanio_memoria();
	memoria_principal =  malloc(tamanio_memoria);

}

bool cantidad_valida(uint32_t size) {
	return size >=1 ? 1 : -1;
}

