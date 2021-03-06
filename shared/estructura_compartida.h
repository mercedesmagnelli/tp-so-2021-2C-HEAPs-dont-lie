 #ifndef ESTRUCTURA_COMPARTIDA_H_
#define ESTRUCTURA_COMPARTIDA_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <commons/string.h>
#include <stdint.h>
#include "logger.h"

/// NUEVO


typedef enum {
	MATELIB,
	KERNEL,
	MEMORIA,
	FILESYSTEM
} t_mensajero;

// Todos los modulos usan esta estructura para enviar mensajes
typedef struct {
	t_mensajero emisor;
	t_mensajero receptor;

	int status;

	void * mensaje;
} t_mensaje;

// Matelib la usa para enviarle a Kernel y/o Memoria
typedef struct {
	uint32_t pid;
} t_matelib_nuevo_proceso;

typedef struct {
	uint32_t pid;
	char * semaforo_nombre;
	int32_t semaforo_valor;
} t_matelib_semaforo;

typedef struct {
	uint32_t pid;
	char * io_nombre;
} t_matelib_io;

typedef struct {
	uint32_t pid;
	uint32_t memoria_size;
} t_matelib_memoria_alloc;
typedef struct {
	uint32_t pid;
	uint32_t memoria_size;
	int32_t memoria_mate_pointer;
} t_matelib_memoria_read;
typedef struct {
	uint32_t pid;
	uint32_t memoria_size;
	int32_t memoria_mate_pointer;
	void * memoria_write;
} t_matelib_memoria_write;
typedef struct {
	uint32_t pid;
	int32_t memoria_mate_pointer;
} t_matelib_memoria_free;

typedef struct {
	size_t size;
	void * mem_read;
} t_ram_read;


typedef struct{
uint32_t cant_pag;
uint32_t pid;
}t_mensaje_r_s; //solicitud de espacio

typedef struct{
	uint32_t nro_pag;
	uint32_t pid;
	size_t tam_data;
	void* data;
}t_write_s; //solicitud escritura

typedef struct{
	uint32_t pid;
	uint32_t nro_pag; //tmb puede ser la cantidad de paginas a liberar - dsp ver de poner un mejor nombre
}t_pedir_o_liberar_pagina_s; //leer y liberar paginas

/**
 * ALLOC: Devuelve un int32_t
 * WRITE: Responde solo t_header EXITO o FALLO
 * FREE: Responde solo t_header EXITO o FALLO
 * READ: t_header de EXITO o FALLO, en caso caso de EXITO sturct { size_t, void * }
 */

t_matelib_nuevo_proceso * shared_crear_nuevo_proceso(uint32_t pid);
void * serializiar_crear_proceso(t_matelib_nuevo_proceso * mensaje, size_t * size_final);
t_matelib_nuevo_proceso * deserializar_crear_proceso(void * puntero);

t_matelib_semaforo * shared_crear_nuevo_semaforo(uint32_t pid, char * nombre, int32_t valor_inicial);
t_matelib_semaforo * shared_crear_usar_semaforo(uint32_t pid, char * nombre);
void * serializar_semaforo(t_matelib_semaforo * mensaje, size_t * size_final);
t_matelib_semaforo * deserializar_semaforo(void * puntero);

t_matelib_io * shared_crear_io(uint32_t pid, char * io_nombre);
void * serializar_io(t_matelib_io * mensaje, size_t * size_final);
t_matelib_io * deserializar_io(void * puntero);

t_matelib_memoria_alloc * shared_crear_nuevo_alloc(uint32_t pid, uint32_t memoria_size);
t_matelib_memoria_read * shared_crear_nuevo_read(uint32_t pid, uint32_t memoria_size, int32_t mate_pointer);
t_matelib_memoria_write * shared_crear_nuevo_write(uint32_t pid, uint32_t memoria_size, int32_t mate_pointer, void * write);
t_matelib_memoria_free * shared_crear_nuevo_free(uint32_t pid, int32_t mate_pointer);
void * serializar_memoria_alloc(t_matelib_memoria_alloc * mensaje, size_t * size_final);
void * serializar_memoria_read(t_matelib_memoria_read * mensaje, size_t * size_final);
void * serializar_memoria_write(t_matelib_memoria_write * mensaje, size_t * size_final);
void * serializar_memoria_free(t_matelib_memoria_free * mensaje, size_t * size_final);
t_matelib_memoria_alloc * deserializar_memoria_alloc(void * puntero);
t_matelib_memoria_read * deserializar_memoria_read(void * puntero);
t_matelib_memoria_write * deserializar_memoria_write(void * puntero);
t_matelib_memoria_free * deserializar_memoria_free(void * puntero);

t_ram_read * shared_crear_ram_read(size_t size, void * memoria_ready);
void * serializar_ram_read(t_ram_read * mensaje, size_t * size_final);
t_ram_read * deserializar_ram_read(void * puntero);


void* serializar_solicitud_espacio(t_mensaje_r_s* mensaje, size_t* size_final);
void* serializar_escribir_en_memoria(t_write_s* mensaje, size_t* size_final);
void* serializar_pedir_pagina(t_pedir_o_liberar_pagina_s* mensaje, size_t* size_final);
void* serializar_liberar_pagina(t_pedir_o_liberar_pagina_s* mensaje, size_t* size_final);


void* deserializar_pedir_pagina(void* respuesta);


t_pedir_o_liberar_pagina_s* shared_crear_pedir_o_liberar(uint32_t pid, uint32_t nro_pag);
t_write_s* shared_crear_write_s(uint32_t nro_pag, uint32_t pid, size_t tam_data, void* data);
t_mensaje_r_s* shared_crear_t_mensaje_r_s(uint32_t cant_pag, uint32_t pid);

//PARA EL SWAP
t_mensaje_r_s * deserializar_mensaje_solicitud_r_s(void * puntero);
t_write_s * deserializar_mensaje_write_s(void* puntero);
t_pedir_o_liberar_pagina_s* deserializar_mensaje_peticion_liberacion_pagina(void* puntero);


#endif /* ESTRUCTURA_COMPARTIDA_H_ */
