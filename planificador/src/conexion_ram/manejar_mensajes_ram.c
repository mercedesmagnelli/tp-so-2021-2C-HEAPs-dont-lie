
#include "manejar_mensajes_ram.h"

int recibir_mensaje_ram(int socket_ram) {
	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket_ram);

	return manejar_mensajes_ram(mensaje);
}

int manejar_mensajes_ram(t_prot_mensaje * mensaje) {
	switch (mensaje->head) {
	case HANDSHAKE_R_D:
		loggear_info("Llegó un handshake de la ram! La aceptamos <3");

		destruir_mensaje(mensaje);
		return 0;
	case DESCONEXION_RAM:
	case DESCONEXION:
		loggear_warning("Se cerró la conexión con ram");

		destruir_mensaje(mensaje);

		return ERROR_DESCONEXION_RAM;
	break;
	case FALLO_AL_RECIBIR:
		loggear_error("Ocurrió un error al recibir el mensaje de ram");

		destruir_mensaje(mensaje);

		return ERROR_AL_RECIBIR_MENSAJE;
	break;
	default:
		loggear_error("Mensaje desconocido desde ram, Código: %d", mensaje->head);

		destruir_mensaje(mensaje);

		return ERROR_MENSAJE_DESCONOCIDO;
	break;
	}
}
