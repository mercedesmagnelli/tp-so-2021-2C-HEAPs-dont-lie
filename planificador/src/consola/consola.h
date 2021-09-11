#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <commons/string.h>
#include <commons/temporal.h>

void levantar_consola();

typedef void Funcion();

typedef struct{
	char* nombre;
	Funcion* funcion;
} COMANDOS;

#endif
