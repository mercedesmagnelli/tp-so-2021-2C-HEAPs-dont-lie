
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
	DESCONEXION, // Indica que un cliente se ha desconectado
	DESCONEXION_TOTAL, // Se apagan los modulos

	// Mensajes que envia la matelib hacia el KERNEL o la RAM
	MATELIB_INIT, // Envia un nuevo proceso al Kernel, que lo crea en NEW y luego lo mueve a READY
	MATELIB_CLOSE, // Avisa al Kernel que hay que mover el proceso a FINISH
	MATELIB_SEM_INIT, // Inicia un semaforo con el valor que pasen
	MATELIB_SEM_WAIT, // Le resta uno al contador del semaforo, si es menor a 1, se mueve a BLOCKED el proceso
	MATELIB_SEM_POST, // Le suma uno al contador del semaforo, si es mayor a 1, se quita un proceso de BLOCKED
	MATELIB_SEM_DESTROY, // Se destruye el semaforo y los procesos asociados dejan de requerir el semaforo
	MATELIB_CALL_IO, // Un proceso llama a un recurso de IO, se bloquea y si el recurso esta en uso se bloquea esperando
	MATELIB_MEM_ALLOC, // El proceso le pide a la ram memoria y devuelve un puntero a la direccion
	MATELIB_MEM_FREE, // El proceso libera memoria en la RAM si son 2 bloques contiguos, se hacen uno
	MATELIB_MEM_READ, // Lee el contenido como void * de lo que esta en la RAM
	MATELIB_MEM_WRITE, // Escribe un void * en la RAM

	// Ram => Filesystem
	HANDSHAKE_R_F,

	// Filesystem => Ram
	HANDSHAKE_F_R,

	// Ram => Planificador
	HANDSHAKE_R_P,

	DESCONEXION_RAM,
	RESPUESTA_OK_R_D,

	// Planificador => Ram
	HANDSHAKE_P_R,

	// Mensajes genericos
	EXITO_EN_LA_TAREA,
	FALLO_EN_LA_TAREA,

	// Pruebas
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
