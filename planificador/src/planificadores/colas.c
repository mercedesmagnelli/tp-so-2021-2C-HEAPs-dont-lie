#include "colas.h"

/**
 * TODOs
 * 1. Crear todas las colas y listas 
 * 2. Crear la destruccion de las colas y listas
 * 3. Pensar en que estructuras se van a pasar entre estados
 * 4. Modificar los void * y los return donde corresponda para pasarlo por parametro o retornarlo
 * 5. Programar la forma de comparar hilos (supongo que comparacion por .pid)
 * 6. Recibir algoritmo de corto plazo para mover entre READY->EXEC
 * 7. Recibir algoritmo de mediano plazo para mover de BLOCK->SUSP/BLOCK
 * */

int colas_iniciar() {
    return 0;
}

int colas_destruir() {
    return 0;
}

int colas_insertar_new(void * hilo) {
    return 0;
}

int colas_mover_new_ready() {
    return 0;
}

int colas_mover_ready_exec() {
    return 0;
}

int colas_mover_exec_finish() {
    return 0;
}

int colas_mover_exec_block() {
    return 0;
}

int colas_mover_block_ready() {
    return 0;
}

int colas_mover_block_block_susp() {
    return 0;
}

int colas_mover_block_susp_block_ready() {
    return 0;
}

int colas_mover_block_ready_ready() {
    return 0;
}
