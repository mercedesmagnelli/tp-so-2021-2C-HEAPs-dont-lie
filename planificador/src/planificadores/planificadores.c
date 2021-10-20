
#include "planificadores.h"

/**
 * TODOs:
 * 1. Crear todo el orquestador entre los distintos modulos del Kernel
 * 2. Crear los hilos que controlan el movimiento entre estados
 * 3. Crear los semaforos para coordinar los hilos y el movimiento entre estados
 * 4. ...
 * */

int planificadores_iniciar() {
    loggear_debug("Iniciado planificador de largo plazo");
    loggear_debug("\tSe encarga de insertar los procesos nuevos en la cola de NEW");
    loggear_debug("\tGenerar las estructuras administrativas del proceso dentro del Kernel");
    loggear_debug("\tNotificar a la memoria del nuevo proceso y esperar confirmacion");
    loggear_debug("\tSi el grado de multiprogramacion lo permite, moverlo en FIFO a READY");

    loggear_debug("Iniciado planificador de corto plazo");
    loggear_debug("\tSe encarga de mover los proceso desde la cola de READY en EXEC");
    loggear_debug("\t\tEsto solo se hace si el grado de multiprocesamiento lo permite");
    loggear_debug("\t\tTambien, para realizarlo se sigue un algoritmo SJF (sin desalojo) o HRRN");

    loggear_debug("Iniciado planificador de mediano plazo");
    loggear_debug("\tSe encarga de las transiciones de BLOCK a SUSP-BLOCK, SUSP-BLOCK a SUSP-READY y SUSP-READY a READY");
    loggear_debug("\tSe utiliza cuando el grado de multiprocesamiento esta copado por procesos que no hacen uso del CPU");

    return 0;
}

void planificadores_destruir() {

}

int planificadores_proceso_iniciar(void * proceso) {
    return 0;
}

int planificadores_proceso_cerrar(void * proceso) {
    return 0;
}

int planificadores_semaforos_iniciar(void * proceso, void * semaforo) {
    return 0;
}

int planificadores_semaforos_wait(void * proceso, void * semaforo) {
    return 0;
}

int planificadores_semaforos_post(void * proceso, void * semaforo) {
    return 0;
}

int planificadores_semaforos_destruir(void * proceso, void * semaforo) {
    return 0;
}

int planificadores_io_llamar(void * proceso, void * dispositivo_io) {
    return 0;
}

void * planificadores_memoria_alloc(void * proceso, void * llamada_memoria) {
    return NULL;
}

int planificadores_memoria_free(void * proceso, void * llamada_memoria) {
    return 0;
}

void * planificadores_memoria_read(void * proceso, void * llamada_memoria) {
    return NULL;
}

int planificadores_memoria_write(void * proceso, void * llamada_memoria) {
    return 0;
}

