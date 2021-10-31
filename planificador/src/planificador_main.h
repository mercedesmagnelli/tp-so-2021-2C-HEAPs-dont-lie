
#ifndef PLANIFICADOR_MAIN_H_
#define PLANIFICADOR_MAIN_H_

#include <pthread.h>
#include <readline/readline.h>

#include "planificadores/planificadores.h"

#include "configuracion/configuracion.h"	// Carga del archivo de configuracion

#include "conexion_ram/conexion_ram.h"		// Conexión entre Planificador y Ram
#include "conexion_lib/conexion_lib.h"		// Conexión entre Planificador y Lib

#include "../../shared/logger.h"			// Mutex para escribir en consola y en archivo de log
#include "../../shared/conexion.h"			// Crear y conectarse a servidor
#include "../../shared/protocolo.h"			// Enviar mensajes a travez de sockets
#include "../../shared/manejo_hilos.h"		// Manejar cierre de hilos y loggear en caso de error

#endif /* PLANIFICADOR_MAIN_H_ */
