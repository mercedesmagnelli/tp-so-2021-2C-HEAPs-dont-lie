#ifndef CONEXION_SWAP_CONEXION_SWAP_H_
#define CONEXION_SWAP_CONEXION_SWAP_H_


#include "../../../shared/protocolo.h"

typedef struct{
	uint32_t pid; //tanto en pid como pag mandamos info que queremos enviar de la RAM a la SWAmP
	uint32_t pag;
	uint32_t cant_pag; // este es el que sirve de cntrol para saber si se envian o no las cosas
	uint32_t pid_pedido; //tanto en pid_pedido como pag_pedida mandamos info que queremos traer de SWAmP a la RAM
	uint32_t pag_pedida;
	uint32_t size;
	void* data; //por aca se envia la data a guardar SWAmP (R-S) y se recibe la data a traer de la SWAmp (S-R)
}t_datos_envio;

#endif /* CONEXION_SWAP_CONEXION_SWAP_H_ */



/**
 * @NAME: serializar_pedido_memoria
 * @DESC: Deja en un mismo void* el pid y el size
 * @RET: un void* con la siguiente forma: [pid | size]
 **/


void* serializar_pedido_memoria(uint32_t pid, uint32_t size);



