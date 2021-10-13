#ifndef MATELIB_H_INCLUDED
#define MATELIB_H_INCLUDED

#include <stdint.h>

//-------------------Type Definitions----------------------/
typedef struct mate_instance
{
    void *group_info;
} mate_instance;

typedef char *mate_io_resource;

typedef char *mate_sem_name;

typedef int32_t mate_pointer;

// TODO: Docstrings

//------------------General Functions---------------------/
/**
 * @NAME: mate_init
 * @DESC: Recibe la instancia y el path del archivo de configuración e inicializa la estructura administrativa necesaria para gestionar la instancia
 * @RETURN: Retorna 0 si todo salió bien. No me queda claro en que caso puede fallar, tal vez al hacer init a una instancia que ya fue iniciada
 */
int mate_init(mate_instance *lib_ref, char *config);
/**
 * @NAME: mate_close
 * @DESC: Recibe una instancia y libera todos los recursos solicitados por la misma
 * @RETURN: Retorna 0 si todo salió bien. Puede que se trate de cerrar una instancia que no existe
 */
int mate_close(mate_instance *lib_ref);

//-----------------Semaphore Functions---------------------/
/**
 * @NAME: mate_sem_init
 * @DESC: Recibe una instancia, un nombre de semáforo y un valor entero sin signo. Avisa al Kernel que esta instancia quiere crear un semáforo con el valor especificado.
 * @RETURN: Retorna 0 si todo salió bien. Según entiendo solo puede hacer uso de esta función mientras este en estado EXEC
 */
int mate_sem_init(mate_instance *lib_ref, mate_sem_name sem, unsigned int value);
/**
 * @NAME: mate_sem_wait
 * @DESC: Recibe una instancia y un nombre de semáforo y envía esta información al kernel. Consume 1 de ese semáforo como un wait normal
 * @RETURN: Retorna 0 si todo salió bien. Según entiendo solo puede hacer uso de esta función mientras este en estado EXEC
 */
int mate_sem_wait(mate_instance *lib_ref, mate_sem_name sem);
/**
 * @NAME: mate_sem_post
 * @DESC: Recibe una instancia y un nombre de semáforo y envía esta información al kernel. Devuelve 1 a ese semáforo como un post/signal normal
 * @RETURN: Retorna 0 si todo salió bien. Según entiendo solo puede hacer uso de esta función mientras este en estado BLOCKED
 */
int mate_sem_post(mate_instance *lib_ref, mate_sem_name sem);
/**
 * @NAME: mate_sem_destroy
 * @DESC: Recibe una instancia y un nombre de semáforo. Libera lo relacionado con ese semáforo y envía esta información al kernel para que haga lo mismo.
 * @RETURN: Retorna 0 si todo salió bien.
 */
int mate_sem_destroy(mate_instance *lib_ref, mate_sem_name sem);

//--------------------IO Functions------------------------/
/**
 * @NAME: mate_call_io
 * @DESC: Recibe una instancia, un recurso y un mensaje. Se comunica con el kernel para que consuma 1 de ese recurso y haga lo que tenga que hace con ese mensaje (imprimir por ejemplo)
 * @RETURN: Retorna 0 si todo salió bien.
 */

int mate_call_io(mate_instance *lib_ref, mate_io_resource io, void *msg);

//--------------Memory Module Functions-------------------/
/**
 * @NAME: mate_memalloc
 * @DESC: Recibe una instancia, un recurso y un tamaño. Se comunica con la memoria (directamente o mediante el kernel) y esta reserva esa cantidad de memoria.
 * @RETURN: Retorna un puntero a la direccion de memoria guardada.
 */
mate_pointer mate_memalloc(mate_instance *lib_ref, int size);
/**
 * @NAME: mate_memfree
 * @DESC: Recibe una instancia, un recurso y mate_pointer. Se comunica con la memoria (directamente o mediante el kernel) y esta libera lo que haya en esa dirección.
 * @RETURN: Retorna 0 si todo salió bien. Puede que la dirección no exista o ya haya sido liberada
 */
int mate_memfree(mate_instance *lib_ref, mate_pointer addr);
/**
 * @NAME: mate_memread
 * @DESC: Recibe una instancia, un mate_pointer origen, un void* destino (donde se va a copiar la info) y un tamaño. Se comunica con la memoria (directamente o mediante el kernel) y esta accede a esta dirección origen y copia el contenido en el void*.
 * @RETURN: Retorna 0 si todo salió bien. Puede que la dirección no exista
 */
int mate_memread(mate_instance *lib_ref, mate_pointer origin, void *dest, int size);
/**
 * @NAME: mate_memwrite
 * @DESC: Recibe una instancia, un void* origen (donde se va a copiar la info), un mate_pointer destino y un tamaño. Se comunica con la memoria (directamente o mediante el kernel) y esta accede a esta dirección destino y copia lo que hay en el void*.
 * @RETURN: Retorna 0 si todo salió bien. Puede que la dirección no exista
 */
int mate_memwrite(mate_instance *lib_ref, void *origin, mate_pointer dest, int size);

#endif
