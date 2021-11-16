#include "memoria.h"

int inicializar_proceso(uint32_t PID){

}

int32_t memalloc(uint32_t pid, int32_t size) {
	loggear_trace("[MATELIB_MEM_ALLOC] estoy entrando al meamalloc");
	if (!cantidad_valida(size)) {
		//corto la ejecucion si ya no tengo que analizar
			return VALOR_MEMORIA_SOLICITADO_INVALIDO;
	} else {
		if (existe_proceso(pid)){
			loggear_trace("[MATELIB_MEM_ALLOC] Existe el proceso %d", pid);
			int32_t ptro = ptro_donde_entra_data(pid, size);
			if (ptro >= 0) {
				actualizar_proceso(pid,ptro,size);
				return ptro;
			} else {
				if (se_puede_almacenar_el_alloc_para_proceso(R_S_PROCESO_EXISTENTE, pid, size)) {
					loggear_trace("[MATELIB_MEM_ALLOC] Se pide mas espacio para el proceso %d", pid);
					//como hay espacio disponble, expando lo que ya tenia
					actualizar_proceso(pid,  (-1) * ptro,  size);
					return (-1) * ptro;
				} else {
					loggear_error("[MATELIB_MEM_ALLOC] No se puede almacenar el alloc para el proceso %d porque no hay espacio suficiente en memoria", pid);
					return ESPACIO_EN_MEMORIA_INSUF;
				}
			}
		} else {
			//si no existe, entonces tengo que crear el nuevo proceso
			if (se_puede_almacenar_el_alloc_para_proceso(R_S_PROCESO_NUEVO, pid, size)) {
				loggear_trace("[MATELIB_MEM_ALLOC] Se crea un proceso nuevo de pid %d", pid);
				int32_t ptro_nuevo_proc = agregar_proceso(pid, size);
				return ptro_nuevo_proc;
			} else {
				return no_se_asigna_proceso(pid, size);
			}
		}

	}
}



int32_t memfree(int32_t direccionLogicaALiberar, uint32_t pid) {

	if(!existe_proceso(pid) || !ptro_valido(pid, direccionLogicaALiberar) || ptro_liberado(pid, direccionLogicaALiberar)){
		return -5; // MATE_FREE_FAULT
	}else{
		loggear_trace("[MATELIB_MEM_FREE] Liberaremos el ptro %d del proceso %d", direccionLogicaALiberar, pid);
		liberar_memoria(pid, direccionLogicaALiberar);
		consolidar_memoria(pid);
		return 0; //ta to' bien
	}

}


int32_t memread(int32_t direccionLogicaALeer, uint32_t pid, uint32_t tamanioALeer, void* lectura) {

    if(!ptro_valido(pid, direccionLogicaALeer) || ptro_liberado(direccionLogicaALeer,pid) || tamanio_de_direccion(direccionLogicaALeer, pid) < tamanioALeer){
        return -6; //MEM_READ_FAULT
    }else{
        lectura = leer_de_memoria(direccionLogicaALeer, pid, tamanioALeer);
        return 0;
    }
}

int32_t memwrite(void* valorAEscribir, int32_t direccionLogicaAEscribir,uint32_t pid, uint32_t tamanioAEscribir){

    if(!ptro_valido(pid, direccionLogicaAEscribir) || ptro_liberado(direccionLogicaAEscribir,pid) || tamanio_de_direccion(direccionLogicaAEscribir, pid) < tamanioAEscribir){
            return -7; // MEM_WRITE_FAULT
    }else {
        escribir_en_memoria(pid, valorAEscribir, tamanioAEscribir, direccionLogicaAEscribir);
    	return 0;

    }
}

void leer_directamente_de_memoria(void* ptroLectura, int32_t tamanio, uint32_t direccionLogica){
	memcpy(ptroLectura, memoria_principal + direccionLogica, tamanio);

}

void escribir_directamente_en_memoria(void* valorAEscribir, int32_t tamanio, uint32_t direccionLogica){
	memcpy(memoria_principal + direccionLogica, valorAEscribir, tamanio);

}

void inicializar_memoria_principal() {
	int tamanio_memoria = get_tamanio_memoria();
	memoria_principal =  malloc(tamanio_memoria);

}

bool cantidad_valida(int32_t size) {
	return size >=1 ? 1 : 0;
}

int32_t close_PID(uint32_t PID){
	if(existe_proceso(PID)){
		eliminar_proceso(PID);
		return 1;
	}else
		return 0;
}

int32_t suspender_PID(uint32_t PID){
	if(existe_proceso(PID)){
		suspender_proceso(PID);
		return 1;
	}else
		return 0;
}

