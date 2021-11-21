#ifndef PAGINACION_ESTRUCTURAS_PAGINACION_H_
#define PAGINACION_ESTRUCTURAS_PAGINACION_H_

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <stdint.h>
#include <pthread.h>
t_list* listaProcesos;//casos: agregar_proceso, liberar_paginas
t_list* listaFrames;//casos: traer_pagina_de_SWAP, liberar_paginas
t_list* listaFramesReservados;//lista de frames reservados por todos los procesos
t_dictionary* cant_frames_por_proceso;//casos: traer_pagina_de_SWAP, liberar_paginas
uint32_t puntero_global;

pthread_mutex_t mutex_acceso_memoria;
pthread_mutex_t mutex_acceso_lista_frames;
pthread_mutex_t mutex_acceso_lista_frames_r;
pthread_mutex_t mutex_acceso_diccionario;
pthread_mutex_t mutex_enviar_mensaje_swap;

typedef struct{
    uint32_t en_mp; //me sirve de flag para comprobar cuando realmente hacer el swap
    uint32_t en_mv; // me sirve para tener el control de la cant maxima por proceso
}asignacion_marcos_fijos;

typedef struct{
	uint32_t currAlloc;
	int32_t prevAlloc;
	int32_t nextAlloc;
	uint8_t isFree;
}heap_metadata;

typedef struct{
	//índice: frame
	uint32_t nroFrame;
	uint32_t estado;
	uint32_t proceso;
	uint32_t pagina;
}t_frame;


typedef struct {
	uint32_t PID;
	t_list* tabla_paginas; //t_list pagina
	t_list* lista_hmd;
	t_list* lista_frames_reservados;
	uint32_t puntero_frames;
	uint32_t miss_proceso;
	uint32_t hits_proceso;
}t_proceso;


typedef struct{
	uint32_t bit_presencia;
	uint32_t frame;
	double timestamp;
	uint32_t bit_uso;
	uint32_t bit_modificacion;
}t_pagina;



#endif /* PAGINACION_ESTRUCTURAS_PAGINACION_H_ */
