
#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../../../shared/estructura_compartida.h"
#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/logger.h"

#include "../utils/utils.h"

int conexiones_iniciar(t_instance_metadata* instancia);
/**
 * @NAME: enviar_mate_init
 * @DESC: Envia el nuevo proceso a la memoria o al Kernel
 */
int enviar_mate_init(t_instance_metadata* instancia, t_matelib_nuevo_proceso * nuevo_proceso);

int enviar_mate_close(t_instance_metadata* instancia, t_matelib_nuevo_proceso * nuevo_proceso);

int enviar_mate_sem_init(t_instance_metadata* instancia, t_matelib_semaforo* nuevo_semaforo);

int enviar_mate_sem_wait(t_instance_metadata* instancia, t_matelib_semaforo* semaforo);

int enviar_mate_sem_post(t_instance_metadata* instancia, t_matelib_semaforo* semaforo);

int enviar_mate_sem_destroy(t_instance_metadata* instancia, t_matelib_semaforo* semaforo);

int enviar_mate_call_io(t_instance_metadata* instancia, t_matelib_io* entrada_salida);

int32_t enviar_mate_memalloc(t_instance_metadata* instancia, t_matelib_memoria_alloc* alloc);

int enviar_mate_memfree(t_instance_metadata* instancia, t_matelib_memoria_free* free);

int enviar_mate_memread(t_instance_metadata* instancia, t_matelib_memoria_read* read);

int enviar_mate_memwrite(t_instance_metadata* instancia, t_matelib_memoria_write* write);



/**
 * @NAME: conexiones_cerrar_conexiones
 * @DESC: Cierra los hilos y libera toda la memoria ocupada
 */
void conexiones_cerrar_conexiones();

/**
 * @NAME: conexiones_iniciar
 * @DESC: Genera y devuelve un socket de la conexion con la ram
 */


#endif /* CONEXIONES_H_ */
