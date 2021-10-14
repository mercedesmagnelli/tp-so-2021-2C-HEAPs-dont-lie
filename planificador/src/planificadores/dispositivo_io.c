#include "dispositivo_io.h"

/**
 * TODOs:
 * 1. Crear las estructuras de memoria para manejar los hilos
 * 2. Crear la funcion de orden superior para que al terminar de usarse un dispositivo busque que hilo quitar de bloqueado
 * 3. Crear la funcion para esperar [X] tiempo segun diga el dispositivo, para notificar del desbloqueo
 * 4. Crear el manejo de hilos que esperan a que termine de usarse un dispositivo
 * */

int dispositivo_io_estructuras_crear() {
    return 0;
}

int dispositivo_io_estructuras_destruir() {
    return 0;
}

int dispositivo_io_iniciar(void * hilo, void * dispositivo) {
    return 0;
}

int dispositivo_io_usar(void * hilo, void * dispositivo) {
    return 0;
}

int dispositivo_io_no_usar(void * proceso, void * dispositivo) {
    return 0;
}

int dispositivo_io_destruir(void * dispositivo) {
    return 0;
}
