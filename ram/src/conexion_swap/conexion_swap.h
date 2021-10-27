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

typedef enum{
// RAM -> SWAP
R_S_TRAER_PAGINA, // Mandamos pid_pedido y pag_pedida a traer a la RAM
R_S_ELIMINAR_PAGINA, // Mandamos pid y pag a eliminar en SWAP
R_S_LIBERAR_PROCESO, // Mandamos pid de proceso a eliminar
R_S_SWAP_PAG, //Mandamos pid y pag a guardar en SWAP y enviamos pid_pedido y pag_pedida a traer a la RAM
R_S_ESPACIO_PROCESO_NUEVO, //mandamos pid y size para consultar espacio para un proceso nuevo
R_S_ESPACIO_PROCESO_EXISTENTE, //idem arriba pero proceso ya existente
// SWAP -> RAM
S_R_MEMORIA_CONCEDIDA, // cant pag = 0(no concede) | 1 (concede) segun corresponda
S_R_ENTREGAR_PAG, // pid_pedido con la pag_pedida y la data asociada a la misma
S_R_ELIMINAR_PAG, // cant pag = 0(no concede) | 1 (concede) segun corresponda
S_R_LIBERAR_PROCESO, // cant pag = 0(no concede) | 1 (concede) segun corresponda
S_R_SWAP_PAG // pid_pedido con la pag_pedida y la data asociada a la misma
}header_r_s;
#endif /* CONEXION_SWAP_CONEXION_SWAP_H_ */
