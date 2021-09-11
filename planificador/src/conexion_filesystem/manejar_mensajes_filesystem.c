
#include "manejar_mensajes_filesystem.h"

int recibir_mensaje_filesystem(int socket_filesystem) {
	t_prot_mensaje * mensaje_filesystem = recibir_mensaje_protocolo(socket_filesystem);

	return manejar_mensajes_filesystem(mensaje_filesystem);
}

int manejar_mensajes_filesystem(t_prot_mensaje * mensaje) {
	switch (mensaje->head) {
	case HANDSHAKE_M_D:
		loggear_info("Llegó un handshake del planificador! Quedamos esperando sabotajes");

		destruir_mensaje(mensaje);
		return 0;
	case DESCONEXION_FILESYSTEM:
	case DESCONEXION:
		loggear_warning("Se cerró la conexión con Filesystem");

		destruir_mensaje(mensaje);

		return ERROR_DESCONEXION_FILESYSTEM;
	break;
	case FALLO_AL_RECIBIR:
		loggear_error("Ocurrió un error al recibir el mensaje de Filesystem");

		destruir_mensaje(mensaje);

		return ERROR_AL_RECIBIR_MENSAJE;
	break;
	default:
		loggear_error("Mensaje desconocido desde Filesystem, Código: %d", mensaje->head);

		destruir_mensaje(mensaje);

		return ERROR_MENSAJE_DESCONOCIDO;
	break;
	}
}
