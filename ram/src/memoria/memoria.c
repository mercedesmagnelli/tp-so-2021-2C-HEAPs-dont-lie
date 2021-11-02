#include "memoria.h"

void* memoria_principal;


int32_t memalloc(uint32_t pid, uint32_t size) {

	if (!cantidad_valida(size)) {
		//corto la ejecucion si ya no tengo que analizar
		loggear_error("No se puede solicitad un tamanio negativo");
		return VALOR_MEMORIA_SOLICITADO_INVALIDO;
	} else {

		if (existe_proceso(pid)){
			loggear_trace("Se pide mas espacio para el proceso %d", pid);
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
					loggear_error("No se puede almacenar el alloc para el proceso %d porque no hay espacio suficiente en memoria", pid);
					return ESPACIO_EN_MEMORIA_INSUF;
				}

			}
		} else {
			//si no existe, entonces tengo que crear el nuevo proceso
			loggear_trace("Se crea un proceso nuevo de pid %d", pid);
			if (se_puede_almacenar_el_alloc_para_proceso(R_S_ESPACIO_PROCESO_NUEVO, pid, size)) {
				int32_t ptro_nuevo_proc = agregar_proceso(pid, size);
				return ptro_nuevo_proc;
			} else {
				return no_se_asigna_proceso(pid, size);
			}
		}

	}
}



int32_t memfree(int32_t direccionLogicaALiberar, uint32_t pid) {

	if(ptro_valido(pid, direccionLogicaALiberar) || ptro_liberado(direccionLogicaALiberar,pid)){
		return -5; // MATE_FREE_FAULT
	}else{
		//Ya aca llegue a un punto donde el puntero puede liberarse
		liberar_memoria(pid, direccionLogicaALiberar);
		consolidar_memoria(pid);
		return 0; //ta to' bien
	}

}

int32_t memread(int32_t direccionLogicaALeer, uint32_t pid, uint32_t tamanioALeer, void* lectura) {

    if(!ptro_valido(pid, direccionLogicaALeer) || ptro_liberado(direccionLogicaALeer,pid) || tamanio_de_direccion(direccionLogicaALeer, pid) < tamanioALeer){
        return -6; //MEM_READ_FAULT
    }else{
        lectura = malloc(tamanioALeer);
        memcpy(lectura, memoria_principal + traducir_a_dir_fisica(pid, direccionLogicaALeer, 0), tamanioALeer);
        return 0;
    }
}

int32_t memwrite(void* valorAEscribir, int32_t direccionLogicaAEscribir,uint32_t pid, uint32_t tamanioAEscribir){

    if(!ptro_valido(pid, direccionLogicaAEscribir) || ptro_liberado(direccionLogicaAEscribir,pid) || tamanio_de_direccion(direccionLogicaAEscribir, pid) < tamanioAEscribir){
            return -7; // MEM_WRITE_FAULT
    }else {
        escribir_directamente_en_memoria(valorAEscribir, tamanioAEscribir, traducir_a_dir_fisica(pid, direccionLogicaAEscribir, 1));
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

