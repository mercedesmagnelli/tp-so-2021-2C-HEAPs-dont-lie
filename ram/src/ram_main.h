
#ifndef RAM_MAIN_H_
#define RAM_MAIN_H_

#include "configuracion/ram_configuracion.h"

#include "../../shared/logger.h"
#include "../../shared/conexion.h"
#include "../../shared/protocolo.h"
#include "memoria/memoria.h"
#include "conexion_planificador/conexion_planificador.h"
#include "manejo_signals/manejo_signals.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>

#include <readline/readline.h>



/*BORRAR!!!!!!!!!!!! son para testing*/


void testeamos();
void imprimir_hdm();
void imprimir_procesos();

#endif /* RAM_MAIN_H_ */
