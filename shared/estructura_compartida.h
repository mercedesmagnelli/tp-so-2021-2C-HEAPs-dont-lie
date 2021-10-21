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
	int pid;

	char * semaforo_nombre;
	int semaforo_valor;

	char * io_nombre;
	void * io_msg;
	// TODO: Tal vez se agregue el tamaño del mensaje

	int mem_size;
	int32_t mem_mate_pointer;
	void * mem_write;
	// TODO: SIempre se va a querer leer todo el contenido de una variable?
} t_matelib_mensaje;


t_matelib_mensaje * crear_matelib_mensaje_init(int pid);

t_matelib_mensaje * crear_matelib_mensaje_semaforo(int pid, char * semaforo);

t_matelib_mensaje * crear_matelib_mensaje_semaforo_init(int pid, char * semaforo, int valor);

t_matelib_mensaje * crear_matelib_mensaje_io(int pid, char * io, void * msg);

t_matelib_mensaje * crear_matelib_mensaje_mem_alloc(int pid, int size);

t_matelib_mensaje * crear_matelib_mensaje_mem_free(int pid, int32_t mate_pointer);

t_matelib_mensaje * crear_matelib_mensaje_mem_read(int pid, int32_t mate_pointer, int size);

t_matelib_mensaje * crear_matelib_mensaje_mem_write(int pid, int32_t mate_pointer, int size, void * mem_write);


size_t size_matelib_mensaje_serializar(t_matelib_mensaje * matelib_mensaje);

void * serializar_matelib_mensaje(t_matelib_mensaje * matelib_mensaje);

t_matelib_mensaje * deserializar_matelib_mensaje(void * puntero);


#endif /* ESTRUCTURA_COMPARTIDA_H_ */
