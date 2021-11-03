
#ifndef CONEXION_RAM_H_
#define CONEXION_RAM_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../configuracion/configuracion.h"

#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/logger.h"

#include "manejar_mensajes.h"

typedef struct {
	t_header respuesta;
	size_t size;
	void * mensaje;
} t_ram_respuesta;

/**
 * @NAME: ram_enviar_handshake
 * @DESC: Antes de empezar a trabajar con la ram, se debe llamar esta funcion y verificar que la ram nos acepta.
 * De ser asi podemos empezar a manipular tripulantes
 * @RETURN:
 * 	0 nos acepta
 * 	X Error, no nos acepta
 */
int enviar_handshake();

t_ram_respuesta * ram_enviar_alloc(t_matelib_memoria_alloc * memoria_alloc);

t_ram_respuesta * ram_enviar_write(t_matelib_memoria_write * memoria_write);

t_ram_respuesta * ram_enviar_free(t_matelib_memoria_free * memoria_free);

t_ram_respuesta * ram_enviar_read(t_matelib_memoria_read * memoria_read);

t_ram_respuesta * ram_enviar_close(t_matelib_nuevo_proceso * muerto_proceso);

/**
 * @NAME: ram_cerrar_conexiones
 * @DESC: Cierra los hilos y libera toda la memoria ocupada
 * @PARAM:
 * safe_close = true, hace pthread_join
 * safe_close = false, hace pthread_detach
 */
void conexiones_cerrar_conexiones(bool safe_close);

/**
 * @NAME: iniciar_conexion_ram
 * @DESC: Genera y devuelve un socket de la conexion con la ram
 */
int conexiones_iniciar();

#endif /* CONEXION_RAM_H_ */
