#include "manejar_mensajes.h"

t_carpincho_swamp* buscar_carpincho_en_lista(uint32_t pid); //busca el carpincho en la lista, no se si debería estar aca pero para avanzar por el momento queda aqui.


int recibir_mensaje(int socket_ram) {
	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket_ram);

	return manejar_mensajes(mensaje);
}

int manejar_mensajes(t_prot_mensaje * mensaje) {

	int error;
	t_carpincho_swamp* carpincho;
	usleep(1000 *  get_retardo_swap());
	switch (mensaje->head) {
	case HANDSHAKE_R_P:
		loggear_info("[HANDSHAKE_R_P] Llegó un handshake de la ram! La aceptamos <3");

		destruir_mensaje(mensaje);
		return 0;
	case R_S_PROCESO_NUEVO:
		loggear_info("[R_S_PROCESO_NUEVO] Nuevo carpincho");

		t_matelib_nuevo_proceso * mensaje_deserializado_nuevo = deserializar_crear_proceso(mensaje->payload);

		loggear_debug("[R_S_PROCESO_NUEVO] [PID: %d] Se crea la estructura administrativa del carpincho", mensaje_deserializado_nuevo->pid);
		//if(get_asignacion() == FIJA){
		//loggear_trace("La asignación es fija");

		carpincho = crear_carpincho(mensaje_deserializado_nuevo->pid, get_marcos_maximos());
		if (carpincho->estado_carpincho > 0) {
			destroy_carpinchos_swamp(carpincho);
			loggear_error("[R_S_PROCESO_NUEVO] [PID: %d] No hay mas espacio, se retorna mensaje de error a la RAM", mensaje_deserializado_nuevo->pid);
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL); //TODO esto no se si es así revisar bien.
		} else {
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		}

		/*}else{
			loggear_trace("La asignación es global");
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		}*/

		free(mensaje_deserializado_nuevo);
		destruir_mensaje(mensaje);
		return 0;
	case R_S_SOLICITUD_ESPACIO:
		loggear_info("[R_S_SOLICITUD_ESPACIO] Llego una solicitud de espacio");
		t_mensaje_r_s* mensaje_deserializado = deserializar_mensaje_solicitud_r_s(mensaje->payload);

		void reservar_marcos_carpincho() {
			loggear_info("[R_S_SOLICITUD_ESPACIO] [PID: %zu] Se crea estructura admionistrativa y reserva de marcos", mensaje_deserializado->pid);

			int error = reservar_marcos(carpincho, mensaje_deserializado->cant_pag, particion_a_escribir(carpincho->pid_carpincho));
			if(error < 0){
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}else{
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			}
		}

		loggear_trace("[R_S_SOLICITUD_ESPACIO] [PID: %zu] [PAG: %zu] El carpincho solicita espacio", mensaje_deserializado->pid, mensaje_deserializado->cant_pag);

		carpincho = buscar_carpincho_en_lista(mensaje_deserializado->pid);

		if (carpincho == NULL) {
			loggear_trace("[R_S_SOLICITUD_ESPACIO] [PID: %zu] El carpincho no esta en la lista", mensaje_deserializado->pid);
			carpincho = crear_carpincho(mensaje_deserializado->pid,mensaje_deserializado->cant_pag);
			if(get_asignacion() == GLOBAL){
				reservar_marcos_carpincho();
			}
		} else {
			reservar_marcos_carpincho();
		}

		free(mensaje_deserializado);
		destruir_mensaje(mensaje);
		return 0;
	case R_S_ESCRIBIR_EN_PAGINA:
		loggear_info("[R_S_ESCRIBIR_EN_PAGINA] Solicitud de la RAM de escritura");

		 t_write_s* write_deserializado = deserializar_mensaje_write_s(mensaje->payload);
		 uint32_t pid = write_deserializado->pid;

		 carpincho = buscar_carpincho_en_lista(pid); //TODO HACER FUNCION

		 if (carpincho == NULL) {
			 loggear_error("[R_S_ESCRIBIR_EN_PAGINA] [PID: %zu] No se encontro el carpincho en la lista, por lo que no se puede escribir", pid);
			 enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);

			 free(write_deserializado->data);
			 free(write_deserializado);
			 destruir_mensaje(mensaje);
			 return 0;
		 }

		 loggear_trace("[R_S_ESCRIBIR_EN_PAGINA] [PID: %zu] El carpincho va a escribir particion", pid);

		 loggear_error("[ALAN]DE LA RAM NOS LLEGA PARA ESCRIBIR %s", (char*)write_deserializado->data);

		 error = escribir_particion(carpincho, write_deserializado->nro_pag, write_deserializado->data, particion_a_escribir(pid));
		 if (error < 0){
			 loggear_error("[R_S_ESCRIBIR_EN_PAGINA] [PID: %zu] Error al escribir en particion", pid);
			 enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
		 } else {
			 loggear_info("[R_S_ESCRIBIR_EN_PAGINA] [PID: %zu] Se escribio en particion", pid);
			 enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		 }

		 free(write_deserializado->data);
		 free(write_deserializado);
		 destruir_mensaje(mensaje);
		 return 0;
	case R_S_PEDIR_PAGINA:
		loggear_info("[R_S_PEDIR_PAGINA] Llego una peticion de pagina");
		//mensaje_serializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));
		//memcpy(mensaje_serializado, mensaje->payload, sizeof(t_mensaje_r_s));
		//t_pedir_o_liberar_pagina_s* pedir_deserializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));

		t_pedir_o_liberar_pagina_s* pedir_deserializado = deserializar_mensaje_peticion_liberacion_pagina(mensaje->payload);

		loggear_info("[R_S_PEDIR_PAGINA] [PID: %zu] [PAG: %d] Se pide la pagina", pedir_deserializado->pid, pedir_deserializado->nro_pag);
		carpincho  = buscar_carpincho_en_lista(pedir_deserializado->pid);
		if (carpincho == NULL) {
			 loggear_error("[R_S_PEDIR_PAGINA] [PID: %zu] No se encontro el carpincho en la lista", pedir_deserializado->pid);
			 enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);

			 free(pedir_deserializado);
			 destruir_mensaje(mensaje);
			 return 0;
		 }

		int resultado;
		char* pagina_info = leer_particion(pedir_deserializado->nro_pag, particion_a_escribir(carpincho->pid_carpincho), carpincho, &resultado);
		if (resultado != 0) {
			loggear_error("[R_S_PEDIR_PAGINA] [PID: %zu] Ocurrio un error al leer la pagina", carpincho->pid_carpincho);
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, get_tamanio_pagina(), pagina_info);
		} else {
			loggear_info("[R_S_PEDIR_PAGINA] [PID: %zu] Se obtuvo la pagina correctamente", carpincho->pid_carpincho);
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, get_tamanio_pagina(), pagina_info);
			free(pagina_info);
		}


		free(pedir_deserializado);
		destruir_mensaje(mensaje);
		return 0;
	case R_S_ELIMINAR_PROCESO:
		loggear_info("[R_S_ELIMINAR_PROCESO] Peticion para eliminar un carpincho");

		t_matelib_nuevo_proceso* PID_proceso_eliminar = deserializar_crear_proceso(mensaje->payload);

		loggear_info("[R_S_ELIMINAR_PROCESO] [PID: %zu] Se solicita eliminar el carpincho", PID_proceso_eliminar->pid);

		carpincho = buscar_carpincho_en_lista(PID_proceso_eliminar->pid);
		if (carpincho == NULL) {
			loggear_error("[R_S_ELIMINAR_PROCESO] [PID: %zu] No encontro el carpincho en la lista para borrar", PID_proceso_eliminar->pid);
			error = -1;
		} else {
			error = eliminar_proceso(carpincho);
		}

		if (error < 0) {
			loggear_error("[R_S_ELIMINAR_PROCESO] [PID: %zu] Ocurrio un error al eliminar el carpincho", PID_proceso_eliminar->pid);
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
		} else {
			loggear_info("[R_S_ELIMINAR_PROCESO] [PID: %zu] Se elimino con exito el carpincho", PID_proceso_eliminar->pid);
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		}

		loggear_info("[R_S_ELIMINAR_PROCESO] [PID: %zu] Quedan [CARPINCHOS_EN_SWAMP: %d]", PID_proceso_eliminar->pid, list_size(lista_carpinchos));

		free(PID_proceso_eliminar);
		destruir_mensaje(mensaje);
		return 0;
	case R_S_LIBERAR_PAGINA:
		loggear_info("[R_S_LIBERAR_PAGINA] Peticion liberar paginas");
		//mensaje_serializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));
		//memcpy(mensaje_serializado, mensaje->payload, sizeof(t_mensaje_r_s));
		//t_pedir_o_liberar_pagina_s* liberar_deserializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));

		t_pedir_o_liberar_pagina_s* liberar_deserializado = deserializar_mensaje_peticion_liberacion_pagina(mensaje->payload);

		loggear_info("[R_S_LIBERAR_PAGINA] [PID: %zu] La [CANTIDAD_PAGINAS_LIBERAR: %d]", liberar_deserializado->pid, liberar_deserializado->nro_pag);

		carpincho = buscar_carpincho_en_lista(liberar_deserializado->pid);
		error = borrar_x_cantidad_de_marcos(carpincho,  liberar_deserializado->nro_pag);

		if(error < 0){
			loggear_error("[R_S_LIBERAR_PAGINA] [PID: %zu] Error inesperado al liberar las paginas del carpincho", liberar_deserializado->pid);
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
		} else {
			loggear_info("[R_S_LIBERAR_PAGINA] [PID: %zu] Se liberaron las paginas del carpincho", liberar_deserializado->pid);
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		}

		free(liberar_deserializado);
		destruir_mensaje(mensaje);
		return 0;
	case DESCONEXION_TOTAL:
		loggear_error("[DESCONEXION_TOTAL] Se cerró la conexión con ram");
		destruir_mensaje(mensaje);
		return ERROR_DESCONEXION_RAM;
	case DESCONEXION:
		loggear_warning("[DESCONEXION] Se cerró el socket");

		destruir_mensaje(mensaje);

		return 0;
	break;
	case FALLO_AL_RECIBIR:
		loggear_error("[FALLO_AL_RECIBIR] Ocurrió un error al recibir el mensaje de ram");

		destruir_mensaje(mensaje);

		return ERROR_AL_RECIBIR_MENSAJE;
	break;
	default:
		loggear_error("[MENSAJE_DESCONOCIDO] Mensaje desconocido desde ram, Código: %d", mensaje->head);

		destruir_mensaje(mensaje);

		return ERROR_MENSAJE_DESCONOCIDO;
	break;
	}
}

t_carpincho_swamp* buscar_carpincho_en_lista(uint32_t pid){
	//bool es_carpincho(void * carpincho) { return pid == ((t_carpincho_swamp *) carpincho)->pid_carpincho; }

	// return list_find(lista_carpinchos, es_carpincho);

	for(int i = 0; i < list_size(lista_carpinchos); i++){
		t_carpincho_swamp* carpi = list_get(lista_carpinchos, i);
		if (carpi->pid_carpincho == pid){
			return carpi;
		}
	}
	return NULL;
}

