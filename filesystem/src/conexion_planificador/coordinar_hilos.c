
#include "coordinar_hilos.h"

pthread_mutex_t * mutex_threads;

int conectar_planificador(void * socket_servidor) {
	int socket = *((int *) socket_servidor);

	bool continuar_escuchando = true;
	while ( continuar_escuchando ) {
		int socket_planificador = aceptar_conexion_protocolo(socket);
		if (socket_planificador < 0) {
			loggear_error("Ocurrio un error al aceptar la conexion: %d", socket_planificador);
			continue;
		}

		t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket_planificador);
		if (mensaje->head == DESCONEXION_PLANIFICADOR) {
			continuar_escuchando = false;
			loggear_trace("Se desconectara el planificador");

			destruir_mensaje(mensaje);
			break;
		}

		pthread_attr_t tattr;
		pthread_t tid;
		int ret;

		ret = pthread_attr_init(&tattr);
		ret = pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
		ret = pthread_create(&tid, &tattr, (void *) manejar_mensaje_planificador, mensaje);

		if (ret != 0) {
			loggear_error("Ocurrió un error al crear el hilo para escuchar el mensaje, Error: %d", ret);
			if (ret == 11) {
				loggear_error("No se pueden crear mas threads, error %d", ret);
			}
		}

		loggear_warning("Se creo el hilo [%zu] pendiente para destruir", ret);
	}

	sem_post(&semaforo_detener_ejecucion);

	return 0;
}
