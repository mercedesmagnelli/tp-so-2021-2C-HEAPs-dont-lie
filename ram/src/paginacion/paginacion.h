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

typedef struct{
    uint32_t en_mp; //me sirve de flag para comprobar cuando realmente hacer el swap
    uint32_t en_mv; // me sirve para tener el control de la cant maxima por proceso
}asignacion_marcos_fijos;

typedef struct{

	uint32_t prevAlloc;
	uint32_t nextAlloc;
	uint8_t isFree;
}heap_metadata;

/*

diccionario_de_frames_fijos

key			| value
pid			| [mp, mv]
1			| [1, 1]
2			| [0, 1]
3			| [3, 0]

maximo_de_frames = get_config_maximo_proceso


get_marcos_del_proceso_en_mv (uint32_t proceso)

get_dictionary(diccionario_de_frames_fijos, proceso);

*/

/*
quiero leer algo que no esta en memoria
if (get_marcos_del_proceso_en_MV (pid) < maximo) {
 no hago swapping, lo traigo directamente
}else {
//hago el swapping
}

*/
typedef struct {
	t_list* tabla_paginas; //t_list pagina
	t_list* lista_hmd;
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




// FUNCIONES PUBLICAS

/**
 * @NAME: existe_proceso
 * @DESC: Se fija si existe el proceso en la lista administrativa
 * @RET:
 *   0 existe el proceso
 *  -1 no existe el proceso
 **/
uint32_t existe_proceso(uint32_t PID);

/**
 * @NAME: ptro_donde_entra_data
 * @DESC: Dado un proceso y el tamaño de la data te devuelve el alloc donde puede entrar, sin pedir memoria a la RAM
 * @RET:
 *   >=0 Devuelve el puntero al alloc donde se puede guardar la data
 *   <0 Devuelve el puntero al ultimo alloc con valor negativo
 **/
uint32_t ptro_donde_entra_data(uint32_t PID, uint32_t tam);

/**
 * @NAME: actualizo_proceso
 * @DESC: Actualiza el HEAP seleccionado del proceso y, en caso necesario, agrega uno nuevo
 **/
void actualizar_proceso(uint32_t PID, uint32_t ptro, uint32_t tamanio);

/**
 * @NAME: agrego_proceso
 * @DESC: Agrega el proceso a la lista de procesos y crea los 2 HEAPs necesarios
 **/
void agregar_proceso(uint32_t PID, uint32_t tam);
/**
 * @NAME: ptro_valido
 * @DESC: Avisa si el puntero del proceso es uno valido
 * @RET:
 *   0 es un puntero valido
 *  -1 espacio no accedible/ existente
 **/
uint32_t ptro_valido(uint32_t PID, uint32_t ptro);

/**
 * @NAME: ptro_liberado
 * @DESC: Avisa si el puntero del proceso esta liberado
 * @RET:
 *   0 es un puntero liberado
 *  -2 es un puntero ocupado
 **/
uint32_t ptro_liberado(uint32_t PID, uint32_t ptro);

/**
 * @NAME: liberar_memoria
 * @DESC: libera la memoria del ptro seleccionado
 **/
void liberar_memoria(uint32_t PID, uint32_t ptro);

/**
 * @NAME: consolidar_memoria
 * @DESC: Se fija los cambios administrativos que se tengan que hacer por liberar memoria
 **/
void consolidar_memoria(PID);

/**
 * @NAME: leer_de_memoria
 * @DESC: Lectura de la data de un proceso de memoria
 * @PARAM:
 *   data: ptro donde se guardara la data leida de memoria
 * @RET:
 *   >0 tamanio del dato leido
 **/
uint32_t leer_de_memoria(uint32_t PID, uint32_t ptroMem, void* data);

/**
 * @NAME: entra_data
 * @DESC: Se fija si en el alloc apuntado puede entrar el tamanio del dato seleccionado
 * @RET:
 *    0 ptro permite el tamanio de la data
 *   -3 ptro no permite el tamanio de la data
 **/
uint32_t entra_data(uint32_t PID, uint32_t ptroMem, uint32_t tamanioData);

/**
 * @NAME: escribir_en_memoria
 * @DESC: Escribe en el ptro de un proceso los datos
 **/
void escribir_en_memoria(int32_t PID, int32_t ptroLogicoMem, int32_t tamanioData, void* data);

/**
 * @NAME: inicializar_estructuras_administrativas()
 * @DESC: inicializa las estructuras administrativas
 **/
void inicializar_estructuras_administrativas();

 /**
 * @NAME: destruir_estructuras_administativas
 * @DESC: libera la memoria reservada por las estructuras administrativas
 **/
void destruir_estructuras_administativas();




// FUNCIONES PRIVADAS DE USO INTERNO
/**
* @NAME: get_proceso
* @DESC: devuelve el proceso con el PID asociado
**/
t_proceso* get_proceso(uint32_t PID);


 /**
 * @NAME: destruir_proceso
 * @DESC: funcion de orden superior que invoca la destruccion de la
 * lista de procesos. Libera la memoria de las listas del struct y el resto de la memoria asociada.
 **/
void destruir_proceso(void* proceso);


#endif /* PAGINACION_PAGINACION_H_ */
