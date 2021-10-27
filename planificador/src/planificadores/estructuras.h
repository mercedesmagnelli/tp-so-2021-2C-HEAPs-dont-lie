
#ifndef PLANIFICADORES_ESTRUCTURAS_H_
#define PLANIFICADORES_ESTRUCTURAS_H_

#include <stddef.h>
#include <stdint.h>

#include <sys/time.h>
#include <time.h>

typedef struct timespec t_timestamp;

typedef enum {
	NEW,
	READY,
	ESTADO_EXEC,
	BLOCK,
	FINISH,
	SUSPENDED_BLOCK,
	SUSPENDED_READY
} t_estado_hilo;


// Contiene la informacion necesaria para mover un proceso como un hilo entre las colas
typedef struct {
	uint32_t pid; // Viene de la matelib

    t_estado_hilo estado; // Inicia como NEW

    float estimacion_anterior; // Viene de la LIB
    float estimacion_actual_sjf;
    float estimacion_actual_hrrn;

    t_timestamp timestamp_entrar_exec;
    t_timestamp timestamp_salir_exec;
    float timestamp_tiempo_exec; // 0 la primera vez

    t_timestamp timestamp_entrar_ready;
} t_hilo;

/**
 * @NAME: pid
 * @DESC: Retorna el pid del hilo actual (es para ahorarme hilo->pid, constantemente)
 */
uint32_t pid(t_hilo * hilo);

/**
 * @NAME: estructuras_current_timestamp
 * @DESC: Retorna el timestamp actual
 */
t_timestamp estructuras_current_timestamp();

/**
 * @NAME: float estructuras_timestamp_diff(t_timestamp time1, t_timestamp time2
 * @DESC: Retorna la diferencia de tiempo entre los 2
 */
float estructuras_timestamp_diff(t_timestamp time1, t_timestamp time2);

#endif /* PLANIFICADORES_ESTRUCTURAS_H_ */
