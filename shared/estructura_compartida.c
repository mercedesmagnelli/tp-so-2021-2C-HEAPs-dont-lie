
#include "estructura_compartida.h"

t_matelib_mensaje * crear_matelib_mensaje_init(int pid) {
	t_matelib_mensaje * mensaje = malloc(sizeof(t_matelib_mensaje));
	mensaje->pid = pid;
	return mensaje;
}

t_matelib_mensaje * crear_matelib_mensaje_semaforo(int pid, char * semaforo) {
	t_matelib_mensaje * mensaje = crear_matelib_mensaje_init(pid);
	mensaje->semaforo_nombre = semaforo;
	return mensaje;
}

t_matelib_mensaje * crear_matelib_mensaje_semaforo_init(int pid, char * semaforo, int valor) {
	t_matelib_mensaje * mensaje = crear_matelib_mensaje_init(pid);
	mensaje->semaforo_nombre = semaforo;
	mensaje->semaforo_valor = valor;
	return mensaje;
}

t_matelib_mensaje * crear_matelib_mensaje_io(int pid, char * io, void * msg) {
	t_matelib_mensaje * mensaje = crear_matelib_mensaje_init(pid);
	mensaje->io_nombre = io;
	mensaje->io_msg = msg;
	return mensaje;
}

t_matelib_mensaje * crear_matelib_mensaje_mem_alloc(int pid, int size) {
	t_matelib_mensaje * mensaje = crear_matelib_mensaje_init(pid);
	mensaje->mem_size = size;
	return mensaje;
}

t_matelib_mensaje * crear_matelib_mensaje_mem_free(int pid, int32_t mate_pointer) {
	t_matelib_mensaje * mensaje = crear_matelib_mensaje_init(pid);
	mensaje->mem_mate_pointer = mate_pointer;
	return mensaje;
}

t_matelib_mensaje * crear_matelib_mensaje_mem_read(int pid, int32_t mate_pointer, int size) {
	t_matelib_mensaje * mensaje = crear_matelib_mensaje_init(pid);
	mensaje->mem_mate_pointer = mate_pointer;
	mensaje->mem_size = size;
	return mensaje;
}

t_matelib_mensaje * crear_matelib_mensaje_mem_write(int pid, int32_t mate_pointer, int size, void * mem_write) {
	t_matelib_mensaje * mensaje = crear_matelib_mensaje_init(pid);
	mensaje->mem_mate_pointer = mate_pointer;
	mensaje->mem_size = size;
	mensaje->mem_write = mem_write;
	return mensaje;
}


size_t size_matelib_mensaje_serializar(t_matelib_mensaje * matelib_mensaje) {
	size_t size_tamanio_char = sizeof(size_t);

	size_t size_pid = sizeof(int);

	size_t size_tamanio_semaforo_nombre = size_tamanio_char;
	size_t size_semaforo_nombre = strlen(matelib_mensaje->semaforo_nombre) + 1;
	size_t size_semaforo_valor = sizeof(int);

	size_t size_tamanio_io_nombre = size_tamanio_char;
	size_t size_io_nombre = strlen(matelib_mensaje->io_nombre) + 1;
	// TODO: Calcular el io_msg

	size_t size_mem_size = sizeof(int);
	size_t size_mem_mate_pointer = sizeof(int32_t);
	// TODO: Calcular el mem_write

	return size_pid + size_tamanio_semaforo_nombre + size_semaforo_nombre + size_semaforo_valor + size_tamanio_io_nombre + size_io_nombre + size_mem_size + size_mem_mate_pointer;
}

void * serializar_matelib_mensaje(t_matelib_mensaje * matelib_mensaje) {
	size_t offset = 0;

	size_t tamanio_buffer = size_matelib_mensaje_serializar(matelib_mensaje);
	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &matelib_mensaje->pid, sizeof(int));
	offset += sizeof(int);

	size_t size_semaforo_nombre = strlen(matelib_mensaje->semaforo_nombre) + 1;
	memcpy(buffer + offset, &size_semaforo_nombre, sizeof(size_t));
	offset += sizeof(size_t);
	memcpy(buffer + offset, &matelib_mensaje->semaforo_nombre, size_semaforo_nombre);
	offset += size_semaforo_nombre;


	memcpy(buffer + offset, &matelib_mensaje->semaforo_valor, sizeof(int));
	offset += sizeof(int);


	size_t size_io_nombre = strlen(matelib_mensaje->io_nombre) + 1;
	memcpy(buffer + offset, &size_io_nombre, sizeof(size_t));
	offset += sizeof(size_t);
	memcpy(buffer + offset, &matelib_mensaje->io_nombre, size_io_nombre);
	offset += size_io_nombre;


	// TODO: "void * io_msg" Ver el tamaño del mensaje

	memcpy(buffer + offset, &matelib_mensaje->mem_size, sizeof(int));
	offset += sizeof(int);

	memcpy(buffer + offset, &matelib_mensaje->mem_mate_pointer, sizeof(int32_t));
	offset += sizeof(int32_t);

	// TODO: "char * mem_write" Ver el tamaño del mensaje

	return buffer;
}

t_matelib_mensaje * deserializar_matelib_mensaje(void * puntero) {
	size_t offset = 0;

	// FIXME: Cambiar esto, por un calculo del tamaño de las variables
	t_matelib_mensaje * matelib_mensaje = malloc(sizeof(t_matelib_mensaje));

	// PID
	memcpy(&matelib_mensaje->pid, puntero + offset, sizeof(int));
	offset += sizeof(int);

	// SEMAFORO_NOMBRE
	// TODO: DELETE THIS si no lo necesitamos al mandar char*
	size_t * cantidad_char_semaforo_nombre = malloc(sizeof(size_t));
	memcpy(cantidad_char_semaforo_nombre, puntero + offset, sizeof(size_t));
	free(cantidad_char_semaforo_nombre);
	offset += sizeof(size_t);

	matelib_mensaje->semaforo_nombre = string_new();
	string_append(&matelib_mensaje->semaforo_nombre, puntero + offset);
	offset += string_length(matelib_mensaje->semaforo_nombre) + 1;


	// SEMAFORO_VALOR
	memcpy(&matelib_mensaje->semaforo_valor, puntero + offset, sizeof(int));
	offset += sizeof(int);

	// TODO: DELETE THIS si no lo necesitamos al mandar char*
	size_t * cantidad_char_io_nombre = malloc(sizeof(size_t));
	memcpy(cantidad_char_io_nombre, puntero + offset, sizeof(size_t));
	offset += sizeof(size_t);
	free(cantidad_char_io_nombre);

	matelib_mensaje->io_nombre = string_new();
	string_append(&matelib_mensaje->io_nombre, puntero + offset);
	offset += string_length(matelib_mensaje->io_nombre) + 1;


	// TODO: "void * io_msg" Ver el tamaño del mensaje

	memcpy(&matelib_mensaje->mem_size, puntero + offset, sizeof(int));
	offset += sizeof(int);

	memcpy(&matelib_mensaje->mem_mate_pointer, puntero + offset, sizeof(int32_t));
	offset += sizeof(int32_t);

	// TODO: "char * mem_write" Ver el tamaño del mensaje

	return matelib_mensaje;
}


