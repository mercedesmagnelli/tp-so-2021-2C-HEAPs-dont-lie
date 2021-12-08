#include "memoria.h"

uint32_t inicializar_proceso(uint32_t PID){
	if(!existe_proceso(PID)){
		loggear_trace("[MATELIB_INIT] estoy inicializando el proceso nuevo %d", PID);

		uint32_t respuesta_swap = iniciar_proceso_SWAP(PID);
		if (respuesta_swap) {
			loggear_trace("[MATELIB_INIT] Se crea el proceso nuevo de pid %d", PID);
			iniciar_proceso_RAM(PID);
			return 0;
		}

		loggear_trace("[MATELIB_INIT] estoy inicializando un proceso nuevo");
		return ESPACIO_EN_MEMORIA_INSUF;
	}else{
		loggear_trace("[MATELIB_INIT] el proceso ya existia en memoria");
		return PROCESO_EXISTENTE;
	}
}

uint32_t PID_listo(uint32_t PID){
	if(existe_proceso(PID)){
		loggear_warning("existe el proceso con el pid %d", PID);
		alistar_proceso(PID);
		loggear_warning("termine de alistar el proceso");
		return 1;
	}else{
		return 0;
	}
}

int32_t memalloc(uint32_t pid, int32_t size) {
	loggear_info("[MATELIB_MEM_ALLOC], el proceso %d pide espacio en la RAM", pid);
	if (!cantidad_valida(size)) {
		//corto la ejecucion si ya no tengo que analizar
			return VALOR_MEMORIA_SOLICITADO_INVALIDO;
	} else {
		int32_t ptro = ptro_donde_entra_data(pid, size);
		loggear_trace("El valor del puntero es %d", ptro);
		if(ptro>=0) {
			loggear_trace("[MATELIB_MEM_ALLOC] - Se puede asignar el espacio solicitado para el proceso %d", pid);
			//puedo asignar en algo que ya estaba
			actualizar_proceso(pid,ptro,size);
			return ptro;
		}else {
			loggear_trace("[MATELIB_MEM_ALLOC] Se verifica memoria en SWAP");
			if(memoria_suficiente_en_swap(pid,size)) {
				//como hay espacio disponble, expando lo que ya tenia
				actualizar_proceso(pid,  (-1) * ptro,  size);
				return (-1) * ptro;
			}else {
				loggear_warning("[MATELIB_MEM_ALLOC] - No se puede pedir mas memoria para el proceso %d", pid);
				return no_se_asigna_proceso(pid, size);
			}
		}
	}
}


int32_t memfree(int32_t direccionLogicaALiberar, uint32_t pid) {

	if(!existe_proceso(pid) || !ptro_valido(pid, direccionLogicaALiberar)){
		return -5; // MATE_FREE_FAULT
	}else{
		loggear_trace("[MATELIB_MEM_FREE] Liberaremos el ptro %d del proceso %d", direccionLogicaALiberar, pid);
		liberar_memoria(pid, direccionLogicaALiberar);
		consolidar_memoria(pid);
		return 0; //ta to' bien
	}

}


int32_t memread(int32_t direccionLogicaALeer, uint32_t pid, uint32_t tamanioALeer, void** lectura) {

    if(!ptro_valido(pid, direccionLogicaALeer)){
        return -6; //MEM_READ_FAULT
    }else{
    	loggear_trace("[MATELIB_MEM_READ] Leeremos el ptro %d del proceso %d con tamanio %d", direccionLogicaALeer, pid, tamanioALeer);
        (*lectura) = leer_de_memoria(direccionLogicaALeer, pid, tamanioALeer);

        return 0;
    }
}

int32_t memwrite(void* valorAEscribir, int32_t direccionLogicaAEscribir,uint32_t pid, uint32_t tamanioAEscribir){

    if(!ptro_valido(pid, direccionLogicaAEscribir)){
            return -7; // MEM_WRITE_FAULT
    }else {
       loggear_trace("[MATELIB_MEM_WRITE - PID: %d] Escribiremos en el ptro %d", pid, direccionLogicaAEscribir);
        escribir_en_memoria(pid, valorAEscribir, tamanioAEscribir, direccionLogicaAEscribir);
    	return 0;

    }
}

void leer_directamente_de_memoria(void* ptroLectura, int32_t tamanio, uint32_t direccionLogica){
	//TODO: CREO QUE NO VA
	//pthread_mutex_lock(&mutex_acceso_memoria);
	memcpy(ptroLectura, memoria_principal + direccionLogica, tamanio);
	//pthread_mutex_unlock(&mutex_acceso_memoria);

}

void escribir_directamente_en_memoria(void* valorAEscribir, int32_t tamanio, uint32_t direccionLogica){
	//TODO: CREO QUE NO VA
	//pthread_mutex_lock(&mutex_acceso_memoria);
	memcpy(memoria_principal + direccionLogica, valorAEscribir, tamanio);

	//pthread_mutex_unlock(&mutex_acceso_memoria);

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
		loggear_trace("[MATELIB_CLOSE] Cerraremos el proceso %d en memoria", PID);
		eliminar_proceso(PID);
		return 1;
	}else
		return 0;
}

int32_t suspender_PID(uint32_t PID){
	if(existe_proceso(PID)){
		loggear_trace("[SUSPENDER] Suspenderemos el proceso %d en memoria", PID);
		suspender_proceso(PID);
		return 1;
	}else
		return 0;
}

