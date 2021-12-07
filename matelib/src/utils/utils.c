#include "utils.h"

uint32_t generar_pid(){
	static uint32_t id = 0;

	id++;

	return id;
}


