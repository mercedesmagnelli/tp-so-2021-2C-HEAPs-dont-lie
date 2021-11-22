
#include "protocolo.h"

t_prot_mensaje * armar_mensaje(t_header header, size_t tamanio_payload, void * payload);

void destruir_mensaje(t_prot_mensaje* mensaje);

t_prot_mensaje* mensaje_error_al_recibir(int socket);
t_prot_mensaje* mensaje_desconexion_al_recibir(int socket);


// Publico
int aceptar_conexion_protocolo(int socket_servidor) {
	struct sockaddr_in direccionCliente;
	unsigned int tamannoDireccion = sizeof(struct sockaddr_in);

	// Se detiene el codigo hasta que un cliente se conecte al servidor o se desconecte o ocurra un error
	return accept(socket_servidor, (struct sockaddr *)&direccionCliente, &tamannoDireccion);
}

// Publico
int enviar_mensaje_protocolo(int socket_destino, t_header header , size_t tamanio_payload, void* payload){

	int resultado_send = 0;

	// Se arma el paquete de envio
	t_prot_mensaje * mensaje = armar_mensaje(header, tamanio_payload, payload);

	loggear_trace("pase por aqui");
	// Envio del tamaño del mensaje, siendo mensaje : header + payload
	resultado_send = send(socket_destino, &(mensaje->tamanio_total) , sizeof(size_t), 0);
	loggear_trace("pase por aquix2");
	if (resultado_send <= 0) {
		destruir_mensaje(mensaje);
		loggear_trace("pase por aquix3");
	} else {
		//1. Reservar lugar para crear el buffer
		loggear_trace("pase por aquix4");
		void* buffer = malloc(mensaje->tamanio_total);
		loggear_trace("pase por aquix5");
		//2. Los primeros bytes son para el HEADER
		memcpy(buffer, &(mensaje->head), sizeof(t_header));
		loggear_trace("pase por aquix6");

		//3. El resto para el payload
		memcpy(buffer + sizeof(t_header), mensaje->payload, mensaje->tamanio_total - sizeof(t_header));
		loggear_trace("pase por aquix7");
		resultado_send = send(socket_destino, buffer, mensaje->tamanio_total, 0);
		loggear_trace("pase por aquix8");
		free(buffer);
		destruir_mensaje(mensaje);

		if (resultado_send <= 0) {
			loggear_trace("pase por aquix9");
			return FALLO_AL_ENVIAR_A_TRAVES_DEL_SOCKET;
		}

	}

	return 0;
}

// Publico
t_prot_mensaje* recibir_mensaje_protocolo(int socket_origen){

	t_prot_mensaje* retorno = (t_prot_mensaje*) malloc(sizeof(t_prot_mensaje));
	/* Si llega 0 es que hubo un close si llega -1 es error*/

	// Recibimos la primera parte y guardamos el tamanio total (de tamaño sizeof(size_t) el cual es similar al unsigned int,)
	int resultado_recv = recv(socket_origen, &(retorno->tamanio_total), sizeof(size_t), MSG_WAITALL);

	if (resultado_recv == -1) {
		close(socket_origen);
		free(retorno);

		return mensaje_error_al_recibir(socket_origen);
	}

	if (resultado_recv == 0){
		close(socket_origen);
		free(retorno);

		return mensaje_desconexion_al_recibir(socket_origen);
	}

	void* buffer = malloc(retorno->tamanio_total);

	// Terminamos de recibir y guardamos en el buffer la cantidad de bytes que me diga tamanio_total
	resultado_recv = recv(socket_origen, buffer, retorno->tamanio_total, MSG_WAITALL);

	if (resultado_recv == -1) {
		close(socket_origen);
		free(retorno);
		free(buffer);

		return mensaje_error_al_recibir(socket_origen);
	}

	if (resultado_recv == 0) {
		close(socket_origen);
		free(retorno);
		free(buffer);

		return mensaje_desconexion_al_recibir(socket_origen);
	}


	// El payload es igual a toda la estructura menos el tamaño total que ya lo sabemos
	void* payload = malloc(retorno->tamanio_total - sizeof(t_header));
	t_header header_recibido;

	memcpy( &header_recibido, buffer, sizeof(t_header));
	memcpy( payload, buffer + sizeof(t_header), retorno->tamanio_total - sizeof(t_header));

	retorno->head = header_recibido;
	retorno->payload = payload;
	retorno->socket = socket_origen;

	free(buffer);

	return retorno;
}

// Publico
char* obtener_string_mensaje(t_prot_mensaje* mensaje){
	size_t largo_string = mensaje->tamanio_total - sizeof(t_header);

	char* string = (char*) malloc(largo_string + 1);

	memset(string, 0, largo_string);
	memcpy(string, mensaje->payload, largo_string );

	string[largo_string] = '\0';

	return string;
}

t_prot_mensaje * armar_mensaje(t_header header, size_t tamanio_payload, void * payload) {
	t_prot_mensaje * mensaje = (t_prot_mensaje*) malloc(sizeof(t_prot_mensaje));

	// Armamos la estructura general
	mensaje->head = header;
	mensaje->payload = malloc(tamanio_payload);
	mensaje->tamanio_total = sizeof(t_header) + tamanio_payload;
	mensaje->socket = 0; // Es innecesario, pero para no mandar basura.

	// Armamos el payload
	memset(mensaje->payload, 0, tamanio_payload);
	memcpy(mensaje->payload, payload, tamanio_payload);

	return mensaje;
}

void destruir_mensaje(t_prot_mensaje* mensaje) {
	if (mensaje != NULL){
		if (mensaje->payload != NULL){
			free(mensaje->payload);
		}

		free(mensaje);
	}
}

t_prot_mensaje* mensaje_error_al_recibir(int socket_origen){
	t_prot_mensaje* retorno = (t_prot_mensaje*)  malloc(sizeof(t_prot_mensaje));

	retorno->head = FALLO_AL_RECIBIR;
	retorno->payload = NULL;
	retorno->socket = socket_origen;
	retorno->tamanio_total = 0;

	return retorno;
}

t_prot_mensaje* mensaje_desconexion_al_recibir(int socket_origen){
	t_prot_mensaje* retorno = (t_prot_mensaje*)  malloc(sizeof(t_prot_mensaje));

	retorno->head = DESCONEXION;
	retorno->payload = NULL;
	retorno->socket = socket_origen;
	retorno->tamanio_total = 0;

	return retorno;
}
