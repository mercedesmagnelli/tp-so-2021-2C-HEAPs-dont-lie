
#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "codigo_error.h"

//HEADER
typedef enum {
	CONEXION, // Un cliente informa a un servidor que se ha conectado. Payload: Algun t_cliente
	FALLO_AL_RECIBIR,// Indica que un mensaje no se recibio correctamente en prot_recibir_mensaje
	DESCONEXION, // Indica que un cliente se ha desconectado (usado para instancias)
	DESCONEXION_TRIPULANTE,
	DESCONEXION_PLANIFICADOR,

	// Planificador => Filesystem
	HANDSHAKE_D_M,
	ESCUCHAR_SABOTAJES_D_M,
	NACIO_TRIUPULANTE_D_M,
	INICIO_TAREA_D_M,
	FIN_TAREA_D_M,
	MOVIMIENTO_TRIPULANTE_D_M,
	CORRER_A_SABOTAJE_D_M,
	INICIO_RESOLVER_SABOTAJE_D_M,
	RESOLUCION_SABOTAJE_M_D,
	RESOLVER_TAREA_IO_D_M,
	MURIO_TRIPULANTE_D_M,
	BITACORA_D_M,

	// Filesystem => Planificador
	HANDSHAKE_M_D,
	SABOTAJE_M_D,
	DESCONEXION_FILESYSTEM,
	BITACORA_M_D,

	// Ram => Planificador
	HANDSHAKE_R_D,
	SIGUIENTE_TAREA_R_D,
	NO_HAY_MAS_TAREAS_R_D,
	NUEVA_PATOTA_OK_R_D,
	NUEVA_PATOTA_ERROR_R_D,
	DESCONEXION_RAM,
	RESPUESTA_OK_R_D,

	// Planificador => Ram
	HANDSHAKE_D_R,
	CAMBIO_ESTADO_TRIPULANTE_D_R,
	SIGUIENTE_TAREA_D_R,
	NUEVA_PATOTA_D_R,
	NACIO_TRIUPULANTE_D_R,
	MOVIMIENTO_TRIPULANTE_D_R,
	INICIO_TAREA_D_R,
	FIN_TAREA_D_R,
	MURIO_TRIPULANTE_D_R,

	// Pruebas RAM => Planificador
	CACHO_DE_TEXTO

} t_header;

typedef struct{
	t_header head;
	size_t tamanio_total;
	void* payload;
	int socket;
}t_prot_mensaje;

/**
 * @NAME: aceptar_conexion_protocolo
 * @DESC: Recibe un socket enlazado a un puerto (se necesita hacer crear_servidor(puerto) y queda esperando a que llegue un cliente.
 * @USO: while (aceptar_conexion_protocolo > 0) { Código para recivir un mensaje }
 */
int aceptar_conexion_protocolo(int socket_servidor);

/**
* @NAME: enviar_mensaje
* @DESC: Recibe SOCKET destino, HEADER a enviar, sizeof del PAYLOAD y el puntero al PAYLOAD
* @PARAMS
* 		socket_destino - Entero al socket
* 		header - Header del mensaje, es un ENUM. Lo lee primero el que recibe
* 		tamanio_payload - El sizeof, no lo podemos calcular acá dentro porque es un puntero de tipo void*
* 		payload - El payload como tal
*
* @EJEMPLO: Enviar un int, con el header NUMERO
*		 int a = 8
*		 prot_enviar_mensaje( destino, NUMERO, sizeof(int), &a);
*/
int enviar_mensaje_protocolo(int socket_destino, t_header header , size_t tamanio_payload, void* payload);

/**
* @NAME: recibir_mensaje
* @DESC: Retorna un mensaje recibido en el socket que se le pasó por parametro.
* @RETURN:
* 	Tamaño: { HEADER + PAYLOAD }
* 	Dentro del header se encuentra el tamaño del payload.
*
* 	Al recibirlo se tiene que castear.
* 	t_prot_mensaje* mensaje 	= recibir_mensaje_protocolo(numero_de_socket);
* 	t_header header			 	= mensaje->head;
* 	tu_tipo_de_struct payload 	= *(tu_tipo_de_struct*) mensaje->payload;
*/
t_prot_mensaje* recibir_mensaje_protocolo(int socket_origen);

/**
 * @NAME: obtener_string_mensaje
 * @DESC: Cuando el payload es un string y no debe ser casteado en ninguna estructura.
 * @RETURN: Retorna el string, se debe hacer un free(...) luego de usarlo
 */
char* obtener_string_mensaje(t_prot_mensaje* mensaje);

/**
 * @NAME: destruir_mensaje
 * @DESC: Limpia de la memoria al mensaje
 */
void destruir_mensaje(t_prot_mensaje* mensaje);

#endif /* PROTOCOLO_H_ */
