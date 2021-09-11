
#include "manejar_mensajes_planificador.h"

void handshake_planificador(t_prot_mensaje * mensaje);
void desconexion(t_prot_mensaje * mensaje);

int recibir_mensaje_planificador(int socket_planificador) {
	t_prot_mensaje * mensaje_planificador = recibir_mensaje_protocolo(socket_planificador);

	return manejar_mensaje_planificador(mensaje_planificador);
}

// Hilos: 0-N, Crea: 0
int manejar_mensaje_planificador(t_prot_mensaje * mensaje) {
	bool seguir_esperando_mensajes = true;
	void dejar_de_esperar_mensajes() { seguir_esperando_mensajes = false; }

	while (seguir_esperando_mensajes) {

		switch (mensaje->head) {
		case HANDSHAKE_D_R:
			handshake_planificador(mensaje);

			dejar_de_esperar_mensajes();
			break;
		case DESCONEXION_PLANIFICADOR:
			loggear_warning("Desconexion planificador");

			desconexion(mensaje);
			destruir_mensaje(mensaje);
			return ERROR_DESCONEXION_PLANIFICADOR;
		case DESCONEXION:
			loggear_warning("Desconexion GENERAL");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return ERROR_DESCONEXION_PLANIFICADOR;
		case FALLO_AL_RECIBIR:
			loggear_error("Error al recibir mensaje");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return ERROR_AL_RECIBIR_MENSAJE;
		default:
			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return ERROR_MENSAJE_DESCONOCIDO;
		}

		int socket_escuchar = mensaje->socket;
		destruir_mensaje(mensaje);

		if (seguir_esperando_mensajes) {
			mensaje = recibir_mensaje_protocolo(socket_escuchar);
		} else {
			loggear_warning("No escuchamos mas mensajes, Socket: %d", socket_escuchar);
		}
	}

	return 0;
}

void handshake_planificador(t_prot_mensaje * mensaje) {
	loggear_info("Recibimos un handshake, es el planificador");

	enviar_mensaje_protocolo(mensaje->socket, HANDSHAKE_R_D, 0, NULL);

	close(mensaje->socket);
}

void desconexion(t_prot_mensaje * mensaje) {
	loggear_info("Se desconectaron!");

	close(mensaje->socket);
}

