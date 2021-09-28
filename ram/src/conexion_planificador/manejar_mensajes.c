
#include "manejar_mensajes.h"

void desconexion(t_prot_mensaje * mensaje);

int recibir_mensaje(int socket) {
	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket);

	return manejar_mensaje(mensaje);
}

int manejar_mensaje(t_prot_mensaje * mensaje) {
	bool seguir_esperando_mensajes = true;
	void dejar_de_esperar_mensajes() { seguir_esperando_mensajes = false; }

	while (seguir_esperando_mensajes) {

		switch (mensaje->head) {
		case HANDSHAKE_P_R:
			enviar_mensaje_protocolo(mensaje->socket, HANDSHAKE_R_P, 0, NULL);

			loggear_info("Llego un handshake de la ram, devolvemos el saludo");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case HANDSHAKE_F_R:
			enviar_mensaje_protocolo(mensaje->socket, HANDSHAKE_R_F, 0, NULL);

			loggear_info("Llego un handshake del filesystem, devolvemos el saludo");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case DESCONEXION:
			loggear_info("Se desconectaron");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return 0;
		case FALLO_AL_RECIBIR:
		default:
			loggear_error("Error al recibir mensaje");

			loggear_error("Ocurrio un error al recibir el mensaje");
			loggear_error("Mensaje recibido Head: %d", mensaje->head);
			loggear_error("Tamaño de mensaje: %d", mensaje->tamanio_total);

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return ERROR_MENSAJE_DESCONOCIDO;
		}

		int socket_escuchar = mensaje->socket;

		if (seguir_esperando_mensajes) {
			mensaje = recibir_mensaje_protocolo(socket_escuchar);
		} else {
			loggear_warning("No escuchamos mas mensajes, Socket: %d", socket_escuchar);
		}
	}

	return 0;
}


void desconexion(t_prot_mensaje * mensaje) {
	loggear_debug("Se cierra el socket");

	close(mensaje->socket);
}

