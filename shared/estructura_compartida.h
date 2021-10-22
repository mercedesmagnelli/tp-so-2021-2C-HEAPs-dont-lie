 #ifndef ESTRUCTURA_COMPARTIDA_H_
#define ESTRUCTURA_COMPARTIDA_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <commons/string.h>

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


t_matelib_nuevo_proceso * shared_crear_nuevo_proceso(uint32_t pid);
void * serializiar_crear_proceso(t_matelib_nuevo_proceso * mensaje);
t_matelib_nuevo_proceso * deserializar_crear_proceso(void * puntero);

t_matelib_semaforo * shared_crear_nuevo_semaforo(uint32_t pid, char * nombre, int32_t valor_inicial);
t_matelib_semaforo * shared_crear_usar_semaforo(uint32_t pid, char * nombre);
void * serializar_semaforo(t_matelib_semaforo * mensaje);
t_matelib_semaforo * deserializar_semaforo(void * puntero);

t_matelib_io * shared_crear_io(uint32_t pid, char * io_nombre);
void * serializar_io(t_matelib_io * mensaje);
t_matelib_io * deserializar_io(void * puntero);

t_matelib_memoria_alloc * shared_crear_nuevo_alloc(uint32_t pid, uint32_t memoria_size);
t_matelib_memoria_read * shared_crear_nuevo_read(uint32_t pid, uint32_t memoria_size, int32_t mate_pointer);
t_matelib_memoria_write * shared_crear_nuevo_write(uint32_t pid, uint32_t memoria_size, int32_t mate_pointer, void * write);
t_matelib_memoria_free * shared_crear_nuevo_free(uint32_t pid, int32_t mate_pointer);
void * serializar_memoria_alloc(t_matelib_memoria_alloc * mensaje);
void * serializar_memoria_read(t_matelib_memoria_read * mensaje);
void * serializar_memoria_write(t_matelib_memoria_write * mensaje);
void * serializar_memoria_free(t_matelib_memoria_free * mensaje);
t_matelib_memoria_alloc * deserializar_memoria_alloc(void * puntero);
t_matelib_memoria_read * deserializar_memoria_read(void * puntero);
t_matelib_memoria_write * deserializar_memoria_write(void * puntero);
t_matelib_memoria_free * deserializar_memoria_free(void * puntero);


#endif /* ESTRUCTURA_COMPARTIDA_H_ */
