
#include "conexion_planificador.h"

int prender_servidor();

int * socket_servidor; // Socket del servidor donde se conectan

pthread_t thread_escuchar_notificaciones; // Donde está el hilo que crea hilos por conexiones

// Publica, Hilos: 1, Crea: 1
int levantar_servidor() {
	int server_error = prender_servidor();
	if (server_error < 0) {
		loggear_error("Ocurrio un error al iniciar el servidor");
		return server_error;
	}

	int error = pthread_create(&thread_escuchar_notificaciones, NULL, (void *) conectar_clientes, (void *) socket_servidor);
	if (error != 0) {
		loggear_error("Ocurrió un error al crear el hilo del servidor, Error: %d", error);
		return error;
	}

	loggear_trace("Creado el servidor");

	return 0;
}

// Publica
void cerrar_conexiones(bool safe_close) {

	if (safe_close) {
		pthread_join(thread_escuchar_notificaciones, NULL);
	} else {
		pthread_detach(thread_escuchar_notificaciones);
	}

	loggear_trace("Cerramos todos los threads que no hayan terminado");

	if (*socket_servidor > 0) {
		close(*socket_servidor);
	}

	free(socket_servidor);

	loggear_trace("Cerrado los threads y sockets");
}

int prender_servidor() {
	socket_servidor = malloc(sizeof(int));
	*socket_servidor = crear_servidor(get_puerto());

	if (*socket_servidor < 0) {
		loggear_error("Ocurrio un error al crear el servidor: %d", *socket_servidor);
		return ERROR_AL_ABRIR_SOCKET;
	}

	return 0;
}

