#include "dispositivo_io.h"

t_dictionary * dispositivos;

pthread_mutex_t lock;

int dispositivo_io_estructuras_crear() {
	dispositivos = dictionary_create();

	if (get_cantidad_dispositivos_io() != get_cantidad_duraciones_io()) {
		loggear_error("[IO] LA cantidad de dispositivos IO y sus duraciones son distintas en el archivo de configuracion");
		return -1;
	}

	for (int i = 0; i < get_cantidad_dispositivos_io(); ++i) {
		t_io * io = malloc(sizeof(t_io));

		io->nombre = get_dispositivos_io()[i];
		io->duracion = atoi(get_duraciones_io()[i]);

		pthread_mutex_init(&io->mutex, NULL);

		dictionary_put(dispositivos, io->nombre, io);
	}

    return 0;
}

int dispositivo_io_estructuras_destruir() {
	dictionary_destroy_and_destroy_elements(dispositivos, free);

    return 0;
}

int dispositivo_io_usar(uint32_t pid, char * nombre) {
	if (!dictionary_has_key(dispositivos, nombre)) {
		loggear_error("[IO] No existe el dispositivo de IO con nombre %s", nombre);
		return -1;
	}

	t_io * io = dictionary_get(dispositivos, nombre);

	t_hilo * hilo = colas_mover_exec_block(IO, io->nombre, pid);
	if (hilo == NULL) {
		loggear_error("[IO] [PID: %zu] No existe el hilo", pid);
		return 0;
	}

	loggear_trace("[IO] [PID: %zu] se va a bloquear para usar [%s] durante %d milisegundos", pid, io->nombre, io->duracion);

	pthread_mutex_lock(&io->mutex);

	loggear_trace("[IO] [PID: %zu] está usando el dispositivo [%s]", pid, io->nombre);
	usleep(io->duracion * 1000);
	pthread_mutex_unlock(&io->mutex);

	loggear_trace("[IO] [PID: %zu] se desbloqueo de la llamada a [%s]", pid, io->nombre);

	colas_desbloquear_hilo_concreto(hilo);

    return 0;
}

