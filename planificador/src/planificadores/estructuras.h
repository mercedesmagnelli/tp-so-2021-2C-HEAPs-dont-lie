
#ifndef PLANIFICADORES_ESTRUCTURAS_H_
#define PLANIFICADORES_ESTRUCTURAS_H_

#include <stdint.h>

typedef enum {
	NEW,
	READY,
	EXEC,
	BLOCK,
	FINISH,
	SUSPENDED_BLOCK,
	SUSPENDED_READY
} t_estado_hilo;

typedef struct {
    float estimacion_anterior; // Resultado de estimacion anterior. Se obtiene por archivo de configuracion la primera vez.
    float estimacion_actual; // Resultado de la nueva estimacion. Se usa para comparar quien se mueve a EXEC.
    uint32_t rafaga_real_anterior; // Milisegundos que estuvo dentro de EXEC. Por defecto es 0 la primera vez. =Salir-Entrar.
    uint32_t timestamp_entrar_exec; // Se toma el timestamp cuando entra en EXEC.
    uint32_t timestamp_salir_exec; // Se toma el timestamp cuando sale de EXEC.
} t_rafaga_cpu;

// Contiene la informacion necesaria para mover un proceso como un hilo entre las colas
typedef struct {
    t_rafaga_cpu rafaga_actual;
    uint32_t numero_proceso;

    t_estado_hilo estado;

    void * proceso;
} t_hilo;

#endif /* PLANIFICADORES_ESTRUCTURAS_H_ */
