
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
		loggear_info("Llegó un handshake de la ram! La aceptamos <3");

		destruir_mensaje(mensaje);
		return 0;
	case R_S_PROCESO_NUEVO:
		loggear_info("LLEGO UN PROCESO NUEVO");;

		t_matelib_nuevo_proceso * mensaje_deserializado_nuevo = deserializar_crear_proceso(mensaje->payload);

		loggear_debug("Se procede a crear la estructura administrativa del carpincho [PID: %d]", mensaje_deserializado_nuevo->pid);
		//if(get_asignacion() == FIJA){
		//loggear_trace("La asignación es fija");
			carpincho = crear_carpincho(mensaje_deserializado_nuevo->pid, get_marcos_maximos());
			if(carpincho->estado_carpincho > 0){
					destroy_carpinchos_swamp(carpincho);
					loggear_debug("NO HAY MAS ESPACIO POR LO QUE SE ENVIA A LA RAM EL MENSAJE DE ERROR AL GUARDAR");
					enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL); //TODO esto no se si es así revisar bien.
				}
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		/*}else{
			loggear_trace("La asignación es global");
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		}*/

		free(mensaje_deserializado_nuevo);
		destruir_mensaje(mensaje);
		return 0;
	case R_S_SOLICITUD_ESPACIO:
		loggear_info("LLEGO SOLICITUD ESPACIO");
		t_mensaje_r_s* mensaje_deserializado = deserializar_mensaje_solicitud_r_s(mensaje->payload);

		loggear_warning("EL PROCESO SOLICITA ESPACIO [PID: %d]", mensaje_deserializado->pid);
		carpincho = buscar_carpincho_en_lista(mensaje_deserializado->pid);
		if( carpincho == NULL){
			loggear_debug("EL CARPINCHO NO ESTA EN LA LISTA, PORQUE LA ASIGNACION ES GLOBAL, SE COMIENZA A CREAR AHORA LA ESTRUCTURA ADMINISTRATIVA Y RESERVAR LOS MARCOS CORRESPONDIENTES [PID: %d]", mensaje_deserializado->pid); //esto quiere decir que no fue inicialziado anteriormente porque la asignacion es global
			carpincho = crear_carpincho(mensaje_deserializado->pid,mensaje_deserializado->cant_pag);
			if(get_asignacion() == GLOBAL){
				error = reservar_marcos(carpincho,mensaje_deserializado->cant_pag, particion_a_escribir(carpincho->pid_carpincho));
				if(error < 0){
					enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
				}else{
					enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
				}
			}
		}else{
			error = reservar_marcos(carpincho,mensaje_deserializado->cant_pag, particion_a_escribir(carpincho->pid_carpincho));
			if(error < 0){
				enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
			}else{
				enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
			}
		}

		free(mensaje_deserializado);
		destruir_mensaje(mensaje);
		return 0;
	case R_S_ESCRIBIR_EN_PAGINA:
		loggear_info("llega una solicitud de escritura desde la RAM");

		 t_write_s* write_deserializado = deserializar_mensaje_write_s(mensaje->payload);
		 carpincho = buscar_carpincho_en_lista(write_deserializado->pid); //TODO HACER FUNCION
		 loggear_warning("EL PROCESO QUE QUIERE ESCRIBIR ES %d", write_deserializado->pid);
		 loggear_error("Se esta escribiendo %s", write_deserializado->data);
		error = escribir_particion(carpincho, write_deserializado->nro_pag, write_deserializado->data, particion_a_escribir(carpincho->pid_carpincho));
		if(error < 0){
			loggear_debug("OCURRIO UN ERROR INESPERADO AL QUERER ESCRIBIR EL ARCHIVO DE SWAP NO SE GUARDO CORRECTAMENTE");
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
		}else{
			loggear_debug("Se logro escribir la pagina mandada en SWAP");
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		}


		free(write_deserializado->data);
		free(write_deserializado);
		destruir_mensaje(mensaje);
		return 0;
	case R_S_PEDIR_PAGINA:
		loggear_info("LLEGO UNA PETICION DE PAGINA DE LA RAM");
		//mensaje_serializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));
		//memcpy(mensaje_serializado, mensaje->payload, sizeof(t_mensaje_r_s));
		//t_pedir_o_liberar_pagina_s* pedir_deserializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));

		t_pedir_o_liberar_pagina_s* pedir_deserializado = deserializar_mensaje_peticion_liberacion_pagina(mensaje->payload);

		loggear_info("PIDE LA PAGINA %zu [PID: %d]", pedir_deserializado->nro_pag, pedir_deserializado->pid); //TODO xq me llega cualquier verdura aca.
		carpincho  = buscar_carpincho_en_lista(pedir_deserializado->pid);

		char* pagina_info = leer_particion(pedir_deserializado->nro_pag, particion_a_escribir(carpincho->pid_carpincho), carpincho); //TODO resta bien hacer lo del error en esto
		int codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, R_S_PEDIR_PAGINA, string_length(pagina_info) + 1, pagina_info);

		if (codigo_mensaje < 0) {
			loggear_error("Ocurrio un error al pedir la lectura de la pagina");
		} else {
			loggear_info("Se mando a la RAM la pagina solicitada");
		}

		//free(mensaje_serializado);
		free(pedir_deserializado);
		free(pagina_info); //TODO porque no me deja hacer este free???
		destruir_mensaje(mensaje);
		return 0;
	case R_S_ELIMINAR_PROCESO:
		loggear_info("llego una peticion para eliminar un proceso"); //TODO necesito saber cual es el struct
		t_matelib_nuevo_proceso* PID_proceso_eliminar = deserializar_crear_proceso(mensaje->payload);
		loggear_info("Se solicita eliminar el proceso [PID: %d]", PID_proceso_eliminar->pid);

		carpincho = buscar_carpincho_en_lista(PID_proceso_eliminar->pid);
		if (carpincho == NULL) {
			loggear_error("No encontro el carpincho en la lista para borrar [PID: %d]", carpincho->pid_carpincho);
			error = -1;
		} else {
			error = eliminar_proceso(carpincho);
		}

		if(error < 0){
			loggear_warning("OCURRIO UN ERROR AL ELIMINAR EL PROCESO QUE SOLICITO LA RAM");
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
		}else{
			enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
		}

		loggear_trace("TENEMOS %d cantidad de carpinchos en swamp", list_size(lista_carpinchos));
		free(PID_proceso_eliminar);
		destruir_mensaje(mensaje);
		return 0;
	case R_S_LIBERAR_PAGINA:
		loggear_info("llego una peticion liberar paginas");
		//mensaje_serializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));
		//memcpy(mensaje_serializado, mensaje->payload, sizeof(t_mensaje_r_s));
		//t_pedir_o_liberar_pagina_s* liberar_deserializado = malloc(sizeof(t_pedir_o_liberar_pagina_s));

		t_pedir_o_liberar_pagina_s* liberar_deserializado = deserializar_mensaje_peticion_liberacion_pagina(mensaje->payload);
		loggear_warning("la cantidad de paginas a liberar es %zu [PID: %d]", liberar_deserializado->nro_pag);
		carpincho  = buscar_carpincho_en_lista(liberar_deserializado->pid);
		error = borrar_x_cantidad_de_marcos(carpincho,  liberar_deserializado->nro_pag);
		if(error < 0){
			loggear_debug("OCURRIO UN ERROR INESPERADO AL QUERER LIBERAR LAS PAGINAS SOLICITIDAS PARA EL PROCESO %zu", liberar_deserializado->pid);
			enviar_mensaje_protocolo(mensaje->socket, FALLO_EN_LA_TAREA, 0, NULL);
		}

		enviar_mensaje_protocolo(mensaje->socket, EXITO_EN_LA_TAREA, 0, NULL);
	//	free(mensaje_serializado);
		free(liberar_deserializado);
		destruir_mensaje(mensaje);
		return 0;;
	case DESCONEXION_TOTAL:
		loggear_error("Se cerró la conexión con ram");
		destruir_mensaje(mensaje);
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
	loggear_trace("SE PROCEDE A BUSCAR EL CARPINCHO EN LA LISTA [PID: %d]", pid);
	for(int i = 0; i < list_size(lista_carpinchos); i++){
		t_carpincho_swamp* carpi = list_get(lista_carpinchos, i);
		if(carpi->pid_carpincho == pid){
			return carpi;
		}
	}
	loggear_info("NO SE ENCONTRO EL CARPINCHO");
	return NULL;
}

