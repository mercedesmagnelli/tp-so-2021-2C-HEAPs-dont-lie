#include "conexion_swap.h"



void* serializar_pedido_memoria(uint32_t pid, uint32_t size){

	void* pedido = malloc(8);

	memcpy(pedido, &pid, sizeof(uint32_t));
	memcpy(pedido + sizeof(uint32_t), &size, sizeof(uint32_t));

	return pedido;


}
