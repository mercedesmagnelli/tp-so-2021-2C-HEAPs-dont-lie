#ifndef ESTRUCTURA_COMPARTIDA_H_
#define ESTRUCTURA_COMPARTIDA_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <commons/string.h>

#include "logger.h"

typedef enum {
	NUEVO,
	READY,
	EXEC,
	BLOCK,
	FINISH
} t_estado_tripulante;

typedef struct {
	int tid;
	int pid;
	int inicial_x;
	int inicial_y;
	int fin_x;
	int fin_y;
	char * nombre_tarea;
	int cantidad;
} t_tripulante_bitacora_d_m;

typedef struct {
	char * tarea;
	uint32_t parametro;
	uint32_t pos_x;
	uint32_t pos_y;
	uint32_t tiempo;
} t_tarea;

typedef struct {
	uint32_t tid;
	uint32_t pid;
	uint32_t pos_x;
	uint32_t pos_y;
	t_estado_tripulante estado;
} t_tripulante;

typedef struct{
	uint32_t pos_x;
	uint32_t pos_y;
}t_origen;

typedef struct {
	uint32_t pid;

	char * tareas;

	uint32_t size_tripulantes;
	t_tripulante ** tripulantes;
} t_patota;

char* convertir_estado_a_texto(t_estado_tripulante estado);

t_tripulante * mock_tripulante(uint32_t pid, uint32_t tid, uint32_t x, uint32_t y);
t_tripulante * crear_tripulante(uint32_t pid, uint32_t tid, uint32_t x, uint32_t y);
t_patota * mock_patota(uint32_t pid, uint32_t cantidad_tripulantes, uint32_t cantidad_tareas);
t_patota * crear_patota(uint32_t pid, uint32_t cantidad_tripulantes, char* tareas);

t_tarea * mock_tarea(uint32_t max_x, uint32_t max_y, uint32_t max_espera);

t_tarea * mock_tarea_sabotaje(uint32_t max_x, uint32_t max_y, uint32_t max_espera);

void * serializar_tripulante(t_tripulante * tripulante);

t_tripulante * deserializar_tripulante(void * puntero);

void * serializar_tarea(t_tarea * tarea, size_t * size_final);

t_tarea * deserializar_tarea(void * tarea);

void * serializar_patota(t_patota * patota, size_t * size_final);

t_patota * deserializar_patota(void * puntero);

char obtener_estado(t_estado_tripulante estado);

void destruir_patota_sin_tripulantes(t_patota * patota);

void destruir_patota(t_patota * patota);

void destruir_tripulantes(t_patota * patota);

#endif /* ESTRUCTURA_COMPARTIDA_H_ */
