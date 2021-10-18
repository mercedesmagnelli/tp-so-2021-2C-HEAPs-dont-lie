#ifndef PAGINACION_PAGINACION_H_
#define PAGINACION_PAGINACION_H_

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct{
	//índice: frame
	uint32_t estado;
	uint32_t proceso;
	uint32_t pagina;
}t_frame;


typedef struct {
	t_list* tablaPaginas; //t_list pagina
	uint32_t miss_proceso;
	uint32_t hits_proceso;
}t_proceso;

typedef struct{
	uint32_t num_pag; // creo que va indexado por esto
	uint32_t bit_presencia;
	uint32_t frame;
	uint32_t timestamp;
	uint32_t bit_uso;
	uint32_t bit_modificacion;
	uint32_t espacio_ocupado;
}t_pagina;


/**
 * @NAME: inicializar_estructuras_administrativas_paginacion
 * @DESC: Inicializa los diccionarios y las listas
 **/
void inicializar_estructuras_administrativas_paginacion();

/**
 * @NAME: solicitar_espacio_memoria
 * @DESC: Solicita memoria para un proceso de un tamanio determinado
 * @RET:
 *   >=0 puntero lógico dirigido al espacio de memoria solicitado
 *   -1  se solicito un valor erroneo de memoria (negativo o 0)
 *   -2  el proceso no tiene mas espacio del cual solicitar (Asig Fija)
 *   -3  No hay mas espacio de memoria del cual solicitar (Asig Global)
 **/
int32_t solicitar_espacio_memoria(int32_t PID,int32_t cantMemoria);

/**
 * @NAME: liberar_espacio_memoria
 * @DESC: Libera una data de un proceso de memoria
 * @RET:
 *   0 se pudo liberar el espacio de memoria
 *   -1 espacio no accedible/existente
 *   -2 el espacio estaba liberado
 **/
int liberar_espacio_memoria(int32_t PID, int32_t ptroLogicoMem);

/**
 * @NAME: leer_espacio_memoria
 * @DESC: Lectura de la data de un proceso de memoria
 * @PARAM:
 *   data: ptro donde se guardara la data leida de memoria
 * @RET:
 *   >0 tamanio del dato leido
 *   -1 espacio no accedible/existente
 *   -2 el espacio estaba liberado
 **/
int32_t leer_espacio_memoria(int32_t PID, int32_t ptroMem, void* data);

/**
 * @NAME: escribir_espacio_memoria
 * @DESC: Escribe en la memoria la data de un proceso
 * @RET:
 *   0 se guardo exitosamente la data en memoria
 *   -1 espacio no accedible/ existente
 *   -2 el espacio estaba liberado
 **/
int escribir_espacio_memoria(int32_t PID, int32_t ptroLogicoMem, int32_t tamanioData, void* data);

#endif /* PAGINACION_PAGINACION_H_ */
