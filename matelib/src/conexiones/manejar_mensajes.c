
#include "../conexiones/manejar_mensajes.h"

int recibir_mensaje(int socket_ram) {
	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket_ram);

	return manejar_mensajes(mensaje);
}

int manejar_mensajes(t_prot_mensaje * mensaje) {
	switch (mensaje->head) {
	case HANDSHAKE_R_P:
		loggear_info("Llegó un handshake de la ram! La aceptamos <3");

		destruir_mensaje(mensaje);
		return 0;

	case EXITO_EN_LA_TAREA:

		destruir_mensaje(mensaje);
		return 0;

	case FALLO_EN_LA_TAREA:

		destruir_mensaje(mensaje);
		return -1;

	case FALLO_EN_LA_RAM:

		destruir_mensaje(mensaje);
		return -1;

	case DESCONEXION_TOTAL:
		loggear_error("Se cerró la conexión con backend");

		return ERROR_DESCONEXION_RAM;
	case DESCONEXION:
		loggear_warning("Se cerró la conexión con backend");

		destruir_mensaje(mensaje);

		return 0;
	break;
	case FALLO_AL_RECIBIR:
		loggear_error("Ocurrió un error al recibir el mensaje de backend");

		destruir_mensaje(mensaje);

		return ERROR_AL_RECIBIR_MENSAJE;
	break;
	default:
		loggear_error("Mensaje desconocido desde backend, Código: %d", mensaje->head);

		destruir_mensaje(mensaje);

		return ERROR_MENSAJE_DESCONOCIDO;
	break;
	}
}
