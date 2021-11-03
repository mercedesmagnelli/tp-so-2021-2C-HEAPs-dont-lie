#include "utils.h"

uint32_t generar_pid(){

	static uint32_t id = 0;

	id++;

	if(id > 100){

		id = 1;
	}

	return id;
}
