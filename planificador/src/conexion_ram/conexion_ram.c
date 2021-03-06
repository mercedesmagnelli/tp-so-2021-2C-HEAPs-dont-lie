#include "conexion_ram.h"

int conexiones_iniciar();

void avisar_ram_desconexion();

// Publica
int ram_enviar_handshake() {
	int socket_ram = conexiones_iniciar();
	if (socket_ram < 0) {
		return socket_ram;
	}

	int resultado = enviar_mensaje_protocolo(socket_ram, HANDSHAKE_P_R, 0, NULL);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al enviar el Handshake a ram, Error: %d", resultado);

		return resultado;
	}

	loggear_trace("Enviado handshake a la ram");

	int error = ram_recibir_mensaje(socket_ram);
	if (error != 0) {
		loggear_info("La RAM nos desconoce, no podemos trabajar");
		return error;
	}

	close(socket_ram);

	return 0;
}

t_prot_mensaje * ram_enviar_mensaje(t_header header, void * mensaje, size_t size) {
	int socket_ram = conexiones_iniciar();
	if (socket_ram < 0) {
		return NULL;
	}

	int resultado = enviar_mensaje_protocolo(socket_ram, header, size, mensaje);
	if (resultado < 0) {
		loggear_error("Ocurrió un error al enviar el mensaje a ram, Error: %d", resultado);

		return NULL;
	}

	t_prot_mensaje * mensaje_ram = ram_recibir(socket_ram);

	close(socket_ram);

	return mensaje_ram;
}

t_ram_respuesta * ram_enviar_alloc(t_matelib_memoria_alloc * memoria_alloc) {
	size_t * size = malloc(sizeof(size_t));
	void * mensaje_lib = serializar_memoria_alloc(memoria_alloc, size);

	t_prot_mensaje * respuesta_ram = ram_enviar_mensaje(MATELIB_MEM_ALLOC, mensaje_lib, *size);

	t_ram_respuesta * respuesta = malloc(sizeof(t_ram_respuesta));

	respuesta->respuesta = respuesta_ram->head;
	respuesta->size = sizeof(int32_t);
	respuesta->mensaje = respuesta_ram->payload;

	free(size);
	free(mensaje_lib);
	free(respuesta_ram); // No destruimos .payload porque se va a necesitar

	return respuesta;
}

t_ram_respuesta * ram_enviar_write(t_matelib_memoria_write * memoria_write) {
	size_t * size = malloc(sizeof(size_t));
	void * mensaje_lib = serializar_memoria_write(memoria_write, size);

	t_prot_mensaje * respuesta_ram = ram_enviar_mensaje(MATELIB_MEM_WRITE, mensaje_lib, *size);

	t_ram_respuesta * respuesta = malloc(sizeof(t_ram_respuesta));

	respuesta->respuesta = respuesta_ram->head;
	respuesta->size = 0;
	respuesta->mensaje = NULL;

	free(size);
	free(mensaje_lib);
	destruir_mensaje(respuesta_ram);

	return respuesta;
}

t_ram_respuesta * ram_enviar_free(t_matelib_memoria_free * memoria_free) {
	size_t * size = malloc(sizeof(size_t));
	void * mensaje_lib = serializar_memoria_free(memoria_free, size);

	t_prot_mensaje * respuesta_ram = ram_enviar_mensaje(MATELIB_MEM_FREE, mensaje_lib, *size);

	t_ram_respuesta * respuesta = malloc(sizeof(t_ram_respuesta));

	respuesta->respuesta = respuesta_ram->head;
	respuesta->size = 0;
	respuesta->mensaje = NULL;

	free(size);
	free(mensaje_lib);
	destruir_mensaje(respuesta_ram);

	return respuesta;
}

t_ram_respuesta * ram_enviar_read(t_matelib_memoria_read * memoria_read) {
	size_t * size = malloc(sizeof(size_t));
	void * mensaje_lib = serializar_memoria_read(memoria_read, size);

	t_prot_mensaje * respuesta_ram = ram_enviar_mensaje(MATELIB_MEM_READ, mensaje_lib, *size);

	t_ram_read * ram_read = deserializar_ram_read(respuesta_ram->payload);

	//loggear_warning("[ram_enviar_read] SIZE A LEER: %d", ram_read->size);
	//loggear_warning("[ram_enviar_read] TEXTO A LEER: %d", *((int *) ram_read->mem_read));

	size_t * size_mensaje_serializado = malloc(sizeof(size_t));
	void * mensaje_serializado = serializar_ram_read(ram_read, size_mensaje_serializado);

	t_ram_respuesta * respuesta = malloc(sizeof(t_ram_respuesta));

	respuesta->respuesta = respuesta_ram->head;
	respuesta->size = *size_mensaje_serializado;
	respuesta->mensaje = mensaje_serializado;

	free(size);
	free(size_mensaje_serializado);
	free(mensaje_lib);
	destruir_mensaje(respuesta_ram);
	free(ram_read->mem_read);
	free(ram_read);

	return respuesta;
}

t_ram_respuesta * ram_enviar_close(t_matelib_nuevo_proceso * muerto_proceso) {
	size_t * size = malloc(sizeof(size_t));
	void * mensaje_lib = serializiar_crear_proceso(muerto_proceso, size);

	t_prot_mensaje * respuesta_ram = ram_enviar_mensaje(MATELIB_CLOSE, mensaje_lib, *size);

	t_ram_respuesta * respuesta = malloc(sizeof(t_ram_respuesta));

	respuesta->respuesta = respuesta_ram->head;
	respuesta->size = 0;
	respuesta->mensaje = NULL;

	free(size);
	free(mensaje_lib);
	destruir_mensaje(respuesta_ram);

	return respuesta;
}

t_ram_respuesta * ram_enviar_init(t_matelib_nuevo_proceso * nuevo_proceso) {
	size_t * size = malloc(sizeof(size_t));
	void * mensaje_lib = serializiar_crear_proceso(nuevo_proceso, size);

	t_prot_mensaje * respuesta_ram = ram_enviar_mensaje(MATELIB_INIT, mensaje_lib, *size);

	t_ram_respuesta * respuesta = malloc(sizeof(t_ram_respuesta));

	respuesta->respuesta = respuesta_ram->head;
	respuesta->size = 0;
	respuesta->mensaje = NULL;

	free(size);
	free(mensaje_lib);
	destruir_mensaje(respuesta_ram);

	return respuesta;
}


int ram_enviar_cambio_estado_proceso(uint32_t pid, t_header estado) {
	uint32_t * pid_m = malloc(sizeof(uint32_t));
	*pid_m = pid;

	t_prot_mensaje * respuesta_ram = ram_enviar_mensaje(estado, pid_m, sizeof(uint32_t));

	int error = 0;
	if (respuesta_ram->head != EXITO_EN_LA_TAREA) {
		loggear_error("[PID: %zu] - La ram no devolvio exito en el cambio de estado", pid);
		error = 1;
	}

	free(pid_m);
	destruir_mensaje(respuesta_ram);

	return error;
}

int ram_enviar_proceso_suspendido(uint32_t pid) {
	loggear_info("[PID: %zu] - Se avisa a la RAM la suspension del proceso", pid);

	return ram_enviar_cambio_estado_proceso(pid, SUSPENDER_PROCESO);
}

int ram_enviar_proceso_ready(uint32_t pid) {
	loggear_info("[PID: %zu] - Se avisa a la RAM que el proceso se movio a ready", pid);

	return ram_enviar_cambio_estado_proceso(pid, PROCESO_EN_READY);
}

// Publica
void ram_cerrar_conexiones() {
	avisar_ram_desconexion();

	loggear_trace("Cerrado los threads y sockets");
}

void avisar_ram_desconexion() {
	int socket_avisar = conexiones_iniciar();

	enviar_mensaje_protocolo(socket_avisar, DESCONEXION_TOTAL, 0, NULL);

	close(socket_avisar);
}

int conexiones_iniciar() {
	char * ram_ip = get_ip_ram();
	int ram_puerto = get_puerto_ram();

	int socket_server = conectar_a_servidor(ram_ip, ram_puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor de ram, Ip: %s, Puerto: %d, Error: %d", ram_ip, 8082, socket_server);

		return socket_server;
	}

	return socket_server;
}
