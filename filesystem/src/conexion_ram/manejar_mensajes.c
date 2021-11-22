
#include "manejar_mensajes.h"

t_carpincho_swamp* buscar_carpincho_en_lista(uint32_t pid); //busca el carpincho en la lista, no se si debería estar aca pero para avanzar por el momento queda aqui.


int recibir_mensaje(int socket_ram) {
	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket_ram);

	return manejar_mensajes(mensaje);
}

int manejar_mensajes(t_prot_mensaje * mensaje) {

	void* mensaje_serializado;
	t_carpincho_swamp* carpincho;
	switch (mensaje->head) {
	case HANDSHAKE_R_P:
		loggear_info("Llegó un handshake de la ram! La aceptamos <3");

		destruir_mensaje(mensaje);
		return 0;
	case R_S_PROCESO_NUEVO:

		loggear_error("ENTRO AQUI");
		mensaje_serializado = malloc(sizeof(t_mensaje_r_s));
		loggear_error("ENTRO AQUIx2");
			memcpy(mensaje_serializado, mensaje->payload, sizeof(t_mensaje_r_s));
			loggear_error("ENTRO AQUIx3");

			t_mensaje_r_s* mensaje_deserializado_nuevo = malloc(sizeof(t_mensaje_r_s));
			loggear_error("ENTRO AQUIx4");

			if(get_asignacion() == FIJA){
				carpincho = crear_carpincho(mensaje_deserializado_nuevo->pid,mensaje_deserializado_nuevo->cant_pag);
				if(carpincho->estado_carpincho > 0){
						destroy_carpinchos_swamp(carpincho);
						loggear_debug("NO HAY MAS ESPACIO POR LO QUE SE ENVIA A LA RAM EL MENSAJE DE ERROR AL GUARDAR");
						enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL); //TODO esto no se si es así revisar bien.
					}
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			}else{
				loggear_error("ENTRO AQUIx5");
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			}

		free(mensaje_serializado);
		free(mensaje_deserializado_nuevo);
		loggear_error("ENTRO AQUIx6");
		destruir_mensaje(mensaje);
		return 0;
	case R_S_SOLICITUD_ESPACIO:

		loggear_error("ENTRO AQUIx7");
		mensaje_serializado = malloc(sizeof(t_mensaje_r_s));
		memcpy(mensaje_serializado, mensaje->payload, sizeof(t_mensaje_r_s));
		loggear_error("ENTRO AQUIx8");
		t_mensaje_r_s* mensaje_deserializado = malloc(sizeof(t_mensaje_r_s));

		mensaje_deserializado = deserializar_mensaje_solicitud_r_s(mensaje_serializado);

		loggear_error("ENTRO AQUIx9");
		carpincho = crear_carpincho(mensaje_deserializado->pid,mensaje_deserializado->cant_pag);

		if(carpincho->estado_carpincho > 0){
			destroy_carpinchos_swamp(carpincho);
			loggear_debug("NO HAY MAS ESPACIO POR LO QUE SE ENVIA A LA RAM EL MENSAJE DE ERROR AL GUARDAR");
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL); //TODO esto no se si es así revisar bien.
		}

		loggear_error("ENTRO AQUIx10");
		reservar_marcos(carpincho,mensaje_deserializado->cant_pag, particion_a_escribir(carpincho->pid_carpincho));

		enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);

		loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,0));
		loggear_error("asdadas %s", list_get(carpincho->marcos_reservados,1));


		free(mensaje_serializado);
		free(mensaje_deserializado);

		return 0;
	case R_S_ESCRIBIR_EN_PAGINA:
		 mensaje_serializado = malloc(sizeof(t_write_s));

		 t_write_s* write_deserializado = malloc(sizeof(t_write_s));

		 write_deserializado = deserializar_mensaje_write_s(mensaje_serializado);
		 carpincho = buscar_carpincho_en_lista(write_deserializado->pid); //TODO HACER FUNCION

		int error = escribir_particion(carpincho, write_deserializado->nro_pag, "gola don pepito9", particion_a_escribir(carpincho->pid_carpincho));
		if(error < 0){
			loggear_debug("OCURRIO UN ERROR INESPERADO AL QUERER ESCRIBIR EL ARCHIVO DE SWAP NO SE GUARDO CORRECTAMENTE");
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
		}
		enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);

		free(write_deserializado->data);
		free(write_deserializado);

		return 0;
	case R_S_PEDIR_PAGINA:
		mensaje_serializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));

		t_pedir_o_liberar_pagina_s* pedir_deserializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));

		pedir_deserializado = deserializar_mensaje_peticion_liberacion_pagina(mensaje_serializado);

		carpincho  = buscar_carpincho_en_lista(pedir_deserializado->pid);

		char* pagina_info = malloc(get_tamanio_pagina() + 1);

		pagina_info = leer_particion(pedir_deserializado->nro_pag, particion_a_escribir(carpincho->pid_carpincho), carpincho); //TODO resta bien hacer lo del error en esto
		int codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, R_S_PEDIR_PAGINA, string_length(pagina_info) + 1, pagina_info);

					if (codigo_mensaje < 0) {
						loggear_error("Ocurrio un error al pedir la lectura de la pagina");
					} else {
						loggear_info("Se mando a la RAM la pagina solicitada");
					}

		free(pagina_info);
		return 0;
	case R_S_ELIMINAR_PROCESO:

		return 0;
	case R_S_LIBERAR_PAGINA:

		return 0;;
	case DESCONEXION_TOTAL:
		loggear_error("Se cerró la conexión con ram");

		return ERROR_DESCONEXION_RAM;
	case DESCONEXION:
		loggear_warning("Se cerró la conexión con ram");

		destruir_mensaje(mensaje);

		return 0;
	break;
	case FALLO_AL_RECIBIR:
		loggear_error("Ocurrió un error al recibir el mensaje de ram");

		destruir_mensaje(mensaje);

		return ERROR_AL_RECIBIR_MENSAJE;
	break;
	default:
		loggear_error("Mensaje desconocido desde ram, Código: %d", mensaje->head);

		destruir_mensaje(mensaje);

		return ERROR_MENSAJE_DESCONOCIDO;
	break;
	}
}

t_carpincho_swamp* buscar_carpincho_en_lista(uint32_t pid){
	for(int i = 0; i < list_size(lista_carpinchos); i++){
	t_carpincho_swamp* carpi = list_get(lista_carpinchos, i);
	if(carpi->pid_carpincho == pid){
		return carpi;
	}
	}
	return NULL;

}









