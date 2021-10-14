#ifndef DISPOSITIVO_IO_H_
#define DISPOSITIVO_IO_H_

/**
 * Funciona muy similar a como funciona `semaforo.h` con la diferencia que:
 * 1. Al llamarse a un dispositivo, el hilo que llama siempre se bloquea
 * 2. Cada dispositivo tiene un tiempo para desbloquear el hilo
 * 3. Apenas se bloquea, se inicia un pthread aparte, con un sleep para que al terminar desbloquee el hilo
 * */

/**
 * @NAME: dispositivo_io_estructuras_crear
 * @DESC: Crea las estructuras de memoria necesarias para administrar los hilos y los dispositivos de IO
 * @RETURN: 0 si salio todo bien.
 * */
int dispositivo_io_estructuras_crear();

/**
 * @NAME: dispositivo_io_estructuras_destruir
 * @DESC: Vacia y destruye todas las estructuras de memoria que se usen para manejar los dispositivos de IO
 * @RETURN: 0 si salio todo bien.
 * */
int dispositivo_io_estructuras_destruir();

/**
 * @NAME: dispositivo_io_iniciar
 * @DESC: Agrega un dispositivo de IO al sistema para posteriormente utilizarlo 
 * */
int dispositivo_io_iniciar(void * hilo, void * dispositivo);

/**
 * @NAME: dispositivo_io_usar
 * @NOTE: Similar a semaforo_wait
 * @DESC: Avisa para bloquear el hilo que realizo la llamada al dispositivo
 * Al mismo tiempo, crea un pthread con un contador que al pasar los segundos de espera, avisa que hay que desbloquear a alguien
 * 1. No se esta usando y se llama. Bloquea el hilo, crea el pthread para desbloquearlo.
 * 2. Se esta usando por [N] hilos (en espera) y se llama. Bloquea el hilo, agrega [1] al contador del dispositivo
 * para que al terminar con el proceso inicie otra vez ya que el hilo ya estaba bloqueado
 * */
int dispositivo_io_usar(void * hilo, void * dispositivo);

/**
 * @NAME: dispositivo_io_no_usar
 * @NOTE: Similar a semaforo_post
 * @DESC: No deberia llamarse desde afuera, posiblemente borre esta funcion. Pero es la que se llama para avisar que se dejo 
 * de usar un dispositivo, porque paso el tiempo de espera, asi que notifica que hay que quitar alguien de bloqueado.
 * Se reduce en 1 los hilos que usan el dispositivo, si queda mas de 1, se llama de nuevo a la funcion para notificar en X tiempo
 * */
int dispositivo_io_no_usar(void * proceso, void * dispositivo);


/**
 * @NAME: dispositivo_io_destruir
 * @DESC: No deberia llamarse nunca, pero destruye el dispositivo de IO y todos los asociados desbloqueandolos de ser necesario
 * */
int dispositivo_io_destruir(void * dispositivo);

#endif
