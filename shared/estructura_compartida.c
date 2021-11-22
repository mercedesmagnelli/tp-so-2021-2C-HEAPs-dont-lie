
#include "estructura_compartida.h"

const size_t SIZE_PID = sizeof(int32_t);
const size_t SIZE_CHAR = sizeof(char);
const size_t SIZE_CANT_PAG = sizeof(uint32_t);
const size_t SIZE_NRO_PAG = sizeof(uint32_t);
const size_t SIZE_RESPUESTA = sizeof(uint32_t);
const size_t SIZE_SEMAFORO_VALOR  = sizeof(uint32_t);

const size_t SIZE_MEMORIA_SIZE = sizeof(uint32_t);
const size_t SIZE_MEMORIA_MATE_POINTER = sizeof(int32_t);

const size_t SIZE_SIZE_MEMORIA = sizeof(size_t);

t_matelib_nuevo_proceso * shared_crear_nuevo_proceso(uint32_t pid) {
	t_matelib_nuevo_proceso * mensaje = malloc(sizeof(t_matelib_nuevo_proceso));
	mensaje->pid = pid;
	return mensaje;
}

t_matelib_semaforo * shared_crear_nuevo_semaforo(uint32_t pid, char * nombre, int32_t valor_inicial) {
	t_matelib_semaforo * mensaje = malloc(sizeof(t_matelib_semaforo));
	mensaje->pid = pid;
	mensaje->semaforo_nombre = nombre;
	mensaje->semaforo_valor = valor_inicial;
	return mensaje;
}
t_matelib_semaforo * shared_crear_usar_semaforo(uint32_t pid, char * nombre) {
	t_matelib_semaforo * mensaje = malloc(sizeof(t_matelib_semaforo));
	mensaje->pid = pid;
	mensaje->semaforo_nombre = nombre;
	mensaje->semaforo_valor = 0;
	return mensaje;
}

t_matelib_io * shared_crear_io(uint32_t pid, char * io_nombre) {
	t_matelib_io * mensaje = malloc(sizeof(t_matelib_io));
	mensaje->pid = pid;
	mensaje->io_nombre = io_nombre;
	return mensaje;
}

t_matelib_memoria_alloc * shared_crear_nuevo_alloc(uint32_t pid, uint32_t memoria_size) {
	t_matelib_memoria_alloc * mensaje = malloc(sizeof(t_matelib_memoria_alloc));
	mensaje->pid = pid;
	mensaje->memoria_size = memoria_size;
	return mensaje;
}
t_matelib_memoria_read * shared_crear_nuevo_read(uint32_t pid, uint32_t memoria_size, int32_t mate_pointer) {
	t_matelib_memoria_read * mensaje = malloc(sizeof(t_matelib_memoria_read));
	mensaje->pid = pid;
	mensaje->memoria_size = memoria_size;
	mensaje->memoria_mate_pointer = mate_pointer;
	return mensaje;
}
t_matelib_memoria_write * shared_crear_nuevo_write(uint32_t pid, uint32_t memoria_size, int32_t mate_pointer, void * write) {
	t_matelib_memoria_write * mensaje = malloc(sizeof(t_matelib_memoria_write));
	mensaje->pid = pid;
	mensaje->memoria_size = memoria_size;
	mensaje->memoria_mate_pointer = mate_pointer;
	mensaje->memoria_write = write;
	return mensaje;
}
t_matelib_memoria_free * shared_crear_nuevo_free(uint32_t pid, int32_t mate_pointer) {
	t_matelib_memoria_free * mensaje = malloc(sizeof(t_matelib_memoria_free));
	mensaje->pid = pid;
	mensaje->memoria_mate_pointer = mate_pointer;
	return mensaje;
}


void * serializiar_crear_proceso(t_matelib_nuevo_proceso * mensaje, size_t * size_final) {
	size_t offset = 0;
	size_t tamanio_buffer = SIZE_PID;
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}

void * serializar_semaforo(t_matelib_semaforo * mensaje, size_t * size_final) {
	size_t offset = 0;
	size_t size_semaforo_nombre = strlen(mensaje->semaforo_nombre) + 1;
	size_t tamanio_buffer = SIZE_PID + SIZE_SEMAFORO_VALOR + size_semaforo_nombre;
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	memcpy(buffer + offset, &mensaje->semaforo_valor, SIZE_SEMAFORO_VALOR);
	offset += SIZE_SEMAFORO_VALOR;

	memcpy(buffer + offset, mensaje->semaforo_nombre, size_semaforo_nombre);
	offset += size_semaforo_nombre;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}

void * serializar_io(t_matelib_io * mensaje, size_t * size_final) {
	size_t offset = 0;
	size_t size_io_nombre = strlen(mensaje->io_nombre) + 1;
	size_t tamanio_buffer = SIZE_PID + SIZE_CHAR * size_io_nombre;
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	memcpy(buffer + offset, mensaje->io_nombre, size_io_nombre);
	offset += size_io_nombre;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}

void * serializar_memoria_alloc(t_matelib_memoria_alloc * mensaje, size_t * size_final) {
	size_t offset = 0;
	size_t tamanio_buffer = SIZE_PID + SIZE_MEMORIA_SIZE;
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	memcpy(buffer + offset, &mensaje->memoria_size, SIZE_MEMORIA_SIZE);
	offset += SIZE_MEMORIA_SIZE;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}

void * serializar_memoria_read(t_matelib_memoria_read * mensaje, size_t * size_final) {
	size_t offset = 0;
	size_t tamanio_buffer = SIZE_PID + SIZE_MEMORIA_SIZE + SIZE_MEMORIA_MATE_POINTER;
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	memcpy(buffer + offset, &mensaje->memoria_size, SIZE_MEMORIA_SIZE);
	offset += SIZE_MEMORIA_SIZE;

	memcpy(buffer + offset, &mensaje->memoria_mate_pointer, SIZE_MEMORIA_MATE_POINTER);
	offset += SIZE_MEMORIA_MATE_POINTER;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}

void * serializar_memoria_write(t_matelib_memoria_write * mensaje, size_t * size_final) {

	size_t offset = 0;
	size_t size_memoria_write = SIZE_CHAR * mensaje->memoria_size;
	size_t tamanio_buffer = SIZE_PID + SIZE_MEMORIA_SIZE + SIZE_MEMORIA_MATE_POINTER + SIZE_CHAR * size_memoria_write;
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	memcpy(buffer + offset, &mensaje->memoria_size, SIZE_MEMORIA_SIZE);
	offset += SIZE_MEMORIA_SIZE;

	memcpy(buffer + offset, &mensaje->memoria_mate_pointer, SIZE_MEMORIA_MATE_POINTER);
	offset += SIZE_MEMORIA_MATE_POINTER;

	memcpy(buffer + offset, mensaje->memoria_write, size_memoria_write);
	offset += size_memoria_write;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}

void * serializar_memoria_free(t_matelib_memoria_free * mensaje, size_t * size_final) {
	size_t offset = 0;
	size_t tamanio_buffer = SIZE_PID + SIZE_MEMORIA_MATE_POINTER;
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	memcpy(buffer + offset, &mensaje->memoria_mate_pointer, SIZE_MEMORIA_MATE_POINTER);
	offset += SIZE_MEMORIA_MATE_POINTER;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}

t_matelib_nuevo_proceso * deserializar_crear_proceso(void * puntero) {
	size_t offset = 0;
	t_matelib_nuevo_proceso * mensaje = malloc(sizeof(t_matelib_nuevo_proceso));

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	return mensaje;
}

t_matelib_semaforo * deserializar_semaforo(void * puntero) {
	size_t offset = 0;
	t_matelib_semaforo * mensaje = malloc(sizeof(t_matelib_semaforo));

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	memcpy(&mensaje->semaforo_valor, puntero + offset, SIZE_SEMAFORO_VALOR);
	offset += SIZE_SEMAFORO_VALOR;

	mensaje->semaforo_nombre = string_new();
	string_append(&mensaje->semaforo_nombre, puntero + offset);
	offset += string_length(mensaje->semaforo_nombre) + 1;

	return mensaje;
}

t_matelib_io * deserializar_io(void * puntero) {
	size_t offset = 0;
	t_matelib_io * mensaje = malloc(sizeof(t_matelib_io));

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	mensaje->io_nombre = string_new();
	string_append(&mensaje->io_nombre, puntero + offset);
	offset += string_length(mensaje->io_nombre) + 1;

	return mensaje;
}

t_matelib_memoria_alloc * deserializar_memoria_alloc(void * puntero) {
	size_t offset = 0;
	t_matelib_memoria_alloc * mensaje = malloc(sizeof(t_matelib_memoria_alloc));

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	memcpy(&mensaje->memoria_size, puntero + offset, SIZE_MEMORIA_SIZE);
	offset += SIZE_MEMORIA_SIZE;

	return mensaje;
}

t_matelib_memoria_read * deserializar_memoria_read(void * puntero) {
	size_t offset = 0;
	t_matelib_memoria_read * mensaje = malloc(sizeof(t_matelib_memoria_read));

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	memcpy(&mensaje->memoria_size, puntero + offset, SIZE_MEMORIA_SIZE);
	offset += SIZE_MEMORIA_SIZE;

	memcpy(&mensaje->memoria_mate_pointer, puntero + offset, SIZE_MEMORIA_MATE_POINTER);
	offset += SIZE_MEMORIA_MATE_POINTER;

	return mensaje;
}

t_matelib_memoria_write * deserializar_memoria_write(void * puntero) {
	size_t offset = 0;
	t_matelib_memoria_write * mensaje = malloc(sizeof(t_matelib_memoria_write));

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	memcpy(&mensaje->memoria_size, puntero + offset, SIZE_MEMORIA_SIZE);
	offset += SIZE_MEMORIA_SIZE;

	memcpy(&mensaje->memoria_mate_pointer, puntero + offset, SIZE_MEMORIA_MATE_POINTER);
	offset += SIZE_MEMORIA_MATE_POINTER;

	size_t size_memoria_write = SIZE_CHAR * mensaje->memoria_size;
	mensaje->memoria_write = malloc(size_memoria_write);
	memcpy(mensaje->memoria_write, puntero + offset, size_memoria_write);
	offset += size_memoria_write;

	return mensaje;
}

t_matelib_memoria_free * deserializar_memoria_free(void * puntero) {
	size_t offset = 0;
	t_matelib_memoria_free * mensaje = malloc(sizeof(t_matelib_memoria_free));

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	memcpy(&mensaje->memoria_mate_pointer, puntero + offset, SIZE_MEMORIA_MATE_POINTER);
	offset += SIZE_MEMORIA_MATE_POINTER;

	return mensaje;
}

t_ram_read * shared_crear_ram_read(size_t size, void * memoria_ready) {
	t_ram_read * mensaje = malloc(sizeof(t_ram_read));
	mensaje->size = size;
	mensaje->mem_read = memoria_ready;
	return mensaje;
}
void * serializar_ram_read(t_ram_read * mensaje, size_t * size_final) {
	size_t offset = 0;
	size_t size_memoria_mensaje = SIZE_CHAR * mensaje->size;
	size_t tamanio_buffer = SIZE_SIZE_MEMORIA + size_memoria_mensaje;
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &mensaje->size, SIZE_SIZE_MEMORIA);
	offset += SIZE_SIZE_MEMORIA;

	memcpy(buffer + offset, mensaje->mem_read, size_memoria_mensaje);
	offset += size_memoria_mensaje;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}
t_ram_read * deserializar_ram_read(void * puntero) {
	size_t offset = 0;
	t_ram_read * mensaje = malloc(sizeof(t_ram_read));

	memcpy(&mensaje->size, puntero + offset, SIZE_SIZE_MEMORIA);
	offset += SIZE_SIZE_MEMORIA;

	size_t size_mensaje = SIZE_CHAR * mensaje->size;
	mensaje->mem_read = malloc(size_mensaje);
	memcpy(mensaje->mem_read, puntero + offset, size_mensaje);
	offset += size_mensaje;

	return mensaje;
}


void* serializar_solicitud_espacio(t_mensaje_r_s* mensaje, size_t* size_final){
	size_t offset = 0;
		size_t tamanio_buffer = SIZE_PID + SIZE_CANT_PAG;
		void * buffer = malloc(tamanio_buffer);


			memcpy(buffer + offset, &mensaje->cant_pag, SIZE_CANT_PAG);
			offset += SIZE_CANT_PAG;

			memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
			offset += SIZE_PID;

			if (size_final != NULL) {
				*size_final = tamanio_buffer;
			}

			return buffer;


}
void* serializar_escribir_en_memoria(t_write_s* mensaje, size_t* size_final, size_t tamanio_data){
	size_t offset = 0;
		size_t tamanio_buffer = SIZE_PID+ SIZE_NRO_PAG + tamanio_data;
		void * buffer = malloc(tamanio_buffer);


			memcpy(buffer + offset, &mensaje->nro_pag, SIZE_NRO_PAG);
			offset += SIZE_NRO_PAG;

			memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
			offset += SIZE_PID;

			memcpy(buffer+offset, mensaje->data, tamanio_data);
			offset+=tamanio_data;
			//aca creo que no sería necesario hacer otro desplazamiento porque ya es el final
			if (size_final != NULL) {
				*size_final = tamanio_buffer;
			}

			return buffer;



}
void* serializar_pedir_pagina(t_pedir_o_liberar_pagina_s* mensaje, size_t* size_final){
	size_t offset = 0;
	size_t tamanio_buffer = SIZE_PID + SIZE_NRO_PAG;
	void * buffer = malloc(tamanio_buffer);


	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	loggear_trace("pagina %d", mensaje->nro_pag);
	memcpy(buffer + offset, &mensaje->nro_pag, SIZE_NRO_PAG);
	offset += SIZE_NRO_PAG;


	//aca creo que no sería necesario hacer otro desplazamiento porque ya es el final
	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}

void* serializar_liberar_pagina(t_pedir_o_liberar_pagina_s* mensaje, size_t* size_final){
	size_t offset = 0;
	size_t tamanio_buffer = SIZE_PID + SIZE_NRO_PAG;
	void * buffer = malloc(tamanio_buffer);


	memcpy(buffer + offset, &mensaje->pid, SIZE_PID);
	offset += SIZE_PID;

	memcpy(buffer + offset, &mensaje->nro_pag, SIZE_NRO_PAG);
	offset += SIZE_NRO_PAG;


	//aca creo que no sería necesario hacer otro desplazamiento porque ya es el final
	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}

	return buffer;
}






void* deserializar_pedir_pagina(void* respuesta){
	void* rta = malloc(32);
	memcpy(rta, respuesta, 32);
	return respuesta;

}



t_pedir_o_liberar_pagina_s* shared_crear_pedir_o_liberar(uint32_t pid, uint32_t nro_pag){

	t_pedir_o_liberar_pagina_s* mensaje = malloc(sizeof(t_pedir_o_liberar_pagina_s));
	mensaje->pid = pid;
	mensaje->nro_pag = nro_pag;
	return mensaje;
}

t_write_s* shared_crear_write_s(uint32_t nro_pag, uint32_t pid, void* data){

	t_write_s* mensaje = malloc(sizeof(t_write_s));
	mensaje->nro_pag = nro_pag;
	mensaje->pid = pid;
	mensaje->data = data;

	return mensaje;

}

t_mensaje_r_s* shared_crear_t_mensaje_r_s(uint32_t cant_pag, uint32_t pid){

	t_mensaje_r_s* mensaje = malloc(sizeof(t_mensaje_r_s));
	mensaje->cant_pag = cant_pag;
	mensaje->pid = pid;
	return mensaje;

}


t_mensaje_r_s * deserializar_mensaje_solicitud_r_s(void * puntero) {
	size_t offset = 0;
	t_mensaje_r_s * mensaje = malloc(sizeof(t_mensaje_r_s));

	memcpy(&mensaje->cant_pag, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	return mensaje;
}



t_write_s * deserializar_mensaje_write_s(void* puntero){
	size_t offset = 0;
	t_write_s * mensaje = malloc(sizeof(t_write_s));

	memcpy(&mensaje->nro_pag, puntero + offset, SIZE_PID);
		offset += SIZE_PID;

	memcpy(&mensaje->pid, puntero + offset, SIZE_PID);
		offset += SIZE_PID;

		mensaje->data = string_new();
		string_append(&mensaje->data, puntero + offset);
		offset += string_length(mensaje->data) + 1;

	return mensaje;
}


t_pedir_o_liberar_pagina_s* deserializar_mensaje_peticion_liberacion_pagina(void* puntero){
	size_t offset = 0;
	t_pedir_o_liberar_pagina_s * mensaje = malloc(sizeof(t_pedir_o_liberar_pagina_s));

	memcpy(&mensaje->pid, puntero + offset, SIZE_NRO_PAG);
		offset += SIZE_PID;

	memcpy(&mensaje->nro_pag, puntero + offset, SIZE_NRO_PAG);
		offset += SIZE_NRO_PAG;



	return mensaje;
}


