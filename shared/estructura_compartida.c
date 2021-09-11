
#include "estructura_compartida.h"

const size_t SIZE_PID = sizeof(uint32_t);
const size_t SIZE_TID = sizeof(uint32_t);
const size_t SIZE_TAMANIO_TAREAS = sizeof(uint32_t);
const size_t SIZE_TAMANIO_TRIPUS = sizeof(uint32_t);
const size_t SIZE_POSICION = sizeof(uint32_t);
const size_t SIZE_ESTADO = sizeof(t_estado_tripulante);

const size_t SIZE_PARAMETRO_TAREA = sizeof(uint32_t);
const size_t SIZE_ESPERA_TAREA = sizeof(uint32_t);

const size_t SIZE_TAREA = sizeof(uint32_t) + sizeof(char*) + sizeof(uint32_t)*4;
const size_t SIZE_TRIPULANTE = sizeof(uint32_t) * 4 + sizeof(t_estado_tripulante);


char* convertir_estado_a_texto(t_estado_tripulante estado){
   switch (estado) {
	   case NUEVO: return "NEW";
	   case READY: return "READY";
	   case EXEC: return "EXEC";
	   case BLOCK: return "BLOCK I/O";
	   case FINISH: return "FINISH";
   }
   return "No tiene estado conocido";
}

size_t SIZE_TAREAS(uint32_t cantidad_tareas) { return cantidad_tareas * sizeof(char); }

void * serializar_tripulante(t_tripulante * tripulante) {
	size_t offset = 0;

	void * buffer = malloc(SIZE_TRIPULANTE);

	memcpy(buffer + offset, &tripulante->pid, SIZE_PID);
	offset += SIZE_PID;

	memcpy(buffer + offset, &tripulante->tid, SIZE_TID);
	offset += SIZE_TID;

	memcpy(buffer + offset, &tripulante->pos_x, SIZE_POSICION);
	offset += SIZE_POSICION;

	memcpy(buffer + offset, &tripulante->pos_y, SIZE_POSICION);
	offset += SIZE_POSICION;

	memcpy(buffer + offset, &tripulante->estado, SIZE_ESTADO);
	offset += SIZE_ESTADO;

	return buffer;
}

t_tripulante * deserializar_tripulante(void * puntero) {
	size_t offset = 0;
	t_tripulante * tripulante = malloc(sizeof(t_tripulante));

	memcpy(&tripulante->pid, puntero + offset, SIZE_PID);
	offset += SIZE_PID;

	memcpy(&tripulante->tid, puntero + offset, SIZE_TID);
	offset += SIZE_TID;
	memcpy(&tripulante->pos_x, puntero + offset, SIZE_POSICION);
	offset += SIZE_POSICION;

	memcpy(&tripulante->pos_y, puntero + offset, SIZE_POSICION);
	offset += SIZE_POSICION;

	memcpy(&tripulante->estado, puntero + offset, SIZE_ESTADO);
	offset += SIZE_ESTADO;

	return tripulante;
}


void * serializar_tarea(t_tarea * tarea, size_t * size_final) {

	size_t SIZE_NOMBRE_TAREA = strlen(tarea->tarea) + 1;

	size_t tamanio_buffer = sizeof(uint32_t) + SIZE_NOMBRE_TAREA + SIZE_PARAMETRO_TAREA + SIZE_POSICION *2 + SIZE_ESPERA_TAREA;

	size_t offset = 0;

	void * buffer = malloc(tamanio_buffer);

	memcpy(buffer + offset, &SIZE_NOMBRE_TAREA, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(buffer + offset, tarea->tarea, SIZE_NOMBRE_TAREA);
	offset += SIZE_NOMBRE_TAREA;

	memcpy(buffer + offset, &tarea->parametro, SIZE_PARAMETRO_TAREA);
	offset += SIZE_PARAMETRO_TAREA;

	memcpy(buffer + offset, &tarea->pos_x, SIZE_POSICION);
	offset += SIZE_POSICION;

	memcpy(buffer + offset, &tarea->pos_y, SIZE_POSICION);
	offset += SIZE_POSICION;

	memcpy(buffer + offset, &tarea->tiempo, SIZE_ESPERA_TAREA);
	offset += SIZE_ESPERA_TAREA;

	if (size_final != NULL) {
		*size_final = tamanio_buffer;
	}
	return buffer;
}

t_tarea * deserializar_tarea(void * puntero) {
	size_t offset = 0;

	t_tarea * tarea = malloc(sizeof(t_tarea));

	size_t * SIZE_NOMBRE_TAREA = malloc(sizeof(size_t));

	memcpy(SIZE_NOMBRE_TAREA, puntero + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	free(SIZE_NOMBRE_TAREA);

	tarea->tarea = string_new();
	string_append(&tarea->tarea, puntero + offset);
	offset += string_length(tarea->tarea) + 1;

	memcpy(&tarea->parametro, puntero + offset, SIZE_PARAMETRO_TAREA);
	offset += SIZE_PARAMETRO_TAREA;

	memcpy(&tarea->pos_x, puntero + offset, SIZE_POSICION);
	offset += SIZE_POSICION;

	memcpy(&tarea->pos_y, puntero + offset, SIZE_POSICION);
	offset += SIZE_POSICION;

	memcpy(&tarea->tiempo, puntero + offset, SIZE_ESPERA_TAREA);
	offset += SIZE_ESPERA_TAREA;

	return tarea;
}

void * serializar_patota(t_patota * patota, size_t * size_final) {
    const size_t SIZE_TRIPUS = patota->size_tripulantes * SIZE_TRIPULANTE;
    size_t SIZE_TAREAS = strlen(patota->tareas) + 1;

    size_t tamanio_buffer = sizeof(uint32_t) * 2 + SIZE_TAREAS + SIZE_TRIPUS;
    size_t offset = 0;

    void * buffer = malloc(tamanio_buffer);

    memcpy(buffer + offset, &patota->pid, SIZE_PID);
    offset += SIZE_PID;

    memcpy(buffer + offset, patota->tareas, SIZE_TAREAS);
    offset += SIZE_TAREAS;

    memcpy(buffer + offset, &patota->size_tripulantes, SIZE_TAMANIO_TRIPUS);
    offset += SIZE_TAMANIO_TRIPUS;

    for (int i = 0; i < patota->size_tripulantes; i++) {
        void * tripulante = serializar_tripulante(patota->tripulantes[i]);

        memcpy(buffer + offset, tripulante, SIZE_TRIPULANTE);
        offset += SIZE_TRIPULANTE;

        free(tripulante);
    }

    if (size_final != NULL) {
        *size_final = tamanio_buffer;
    }

    return buffer;
}

t_patota * deserializar_patota(void * puntero) {
    t_patota * patota = malloc(sizeof(t_patota));

    size_t offset = 0;

    memcpy(&patota->pid, puntero + offset, SIZE_PID);
    offset += SIZE_PID;

    patota->tareas = string_new();
    string_append(&patota->tareas, puntero + offset);
    offset += string_length(patota->tareas) + 1;

    //patota->size_tareas = string_length(patota->tareas) + 1;

    memcpy(&patota->size_tripulantes, puntero + offset, SIZE_TAMANIO_TRIPUS);
    offset += SIZE_TAMANIO_TRIPUS;

    void* tripulante_serializado = malloc(SIZE_TRIPULANTE);
    t_tripulante* tripulante;

    patota->tripulantes = calloc(patota->size_tripulantes, sizeof(t_tripulante));
    for (int i = 0; i < patota->size_tripulantes; i++) {

        memcpy(tripulante_serializado, puntero + offset, SIZE_TRIPULANTE);
        offset += SIZE_TRIPULANTE;

        tripulante = deserializar_tripulante(tripulante_serializado);
        patota->tripulantes[i] = tripulante;
    }

    free(tripulante_serializado);

    return patota;
}

t_tripulante * mock_tripulante(uint32_t pid, uint32_t tid, uint32_t x, uint32_t y) {
	t_tripulante * tripulante = malloc(sizeof(t_tripulante));

	tripulante->pid = pid;
	tripulante->tid = tid;
	tripulante->pos_x = x;
	tripulante->pos_y = y;
	tripulante->estado = NUEVO;

	return tripulante;
}

t_patota * mock_patota(uint32_t pid, uint32_t cantidad_tripulantes, uint32_t cantidad_tareas) {
	t_patota * patota = malloc(sizeof(t_patota));

	static int tid = 1;

	patota->pid = pid;

	patota->size_tripulantes = cantidad_tripulantes;

	patota->tareas = string_new();

	string_append(&patota->tareas, "BARRER;7;7;4\n");
	string_append(&patota->tareas, "GENERAR_OXIGENO 12;0;0;5\n");

	patota->tripulantes = calloc(cantidad_tripulantes, sizeof(t_tripulante));
	for (int i = 0; i < cantidad_tripulantes; i++) {
		t_tripulante * mock_tripu = mock_tripulante(pid, tid++, i + rand() % 10 + 1, i + rand() % 10 + 1);

		patota->tripulantes[i] = mock_tripu;
	}

	return patota;
}

t_tripulante * crear_tripulante(uint32_t pid, uint32_t tid, uint32_t x, uint32_t y) {
	t_tripulante * tripulante = malloc(sizeof(t_tripulante));

	tripulante->pid = pid;
	tripulante->tid = tid;
	tripulante->pos_x = x;
	tripulante->pos_y = y;
	tripulante->estado = NUEVO;

	return tripulante;
}

t_patota * crear_patota(uint32_t pid, uint32_t cantidad_tripulantes, char* tareas) {
	t_patota * patota = malloc(sizeof(t_patota));

	patota->pid = pid;

	patota->size_tripulantes = cantidad_tripulantes;

	patota->tareas = string_new();

	string_append(&patota->tareas, tareas);


	patota->tripulantes = calloc(cantidad_tripulantes, sizeof(t_tripulante));
	for (int i = 0; i < cantidad_tripulantes; i++) {
		t_tripulante * mock_tripu = mock_tripulante(pid,0,0,0);
		patota->tripulantes[i] = mock_tripu;
	}

	return patota;
}

t_tarea * mock_tarea(uint32_t max_x, uint32_t max_y, uint32_t max_espera) {
	t_tarea * tarea = malloc(sizeof(t_tarea));

	tarea->tarea = "GENERAR_BASURA";
	tarea->parametro = 222;

	tarea->pos_x = rand() % max_x;
	tarea->pos_y = rand() % max_y;

	tarea->tiempo = max_x + max_y;

	return tarea;
}

char obtener_estado(t_estado_tripulante estado) {
	switch (estado) {
	case NUEVO:
		return 'N';
	case READY:
		return 'R';
	case EXEC:
		return 'E';
	case BLOCK:
		return 'B';
	case FINISH:
		return 'F';
	default:
		return 'N';
	}
}

t_tarea * mock_tarea_sabotaje(uint32_t max_x, uint32_t max_y, uint32_t max_espera) {
	t_tarea * tarea = malloc(sizeof(t_tarea));

	tarea->tarea = "SABOTAJE!!!";
	tarea->parametro = 0;

	tarea->pos_x = max_x;
	tarea->pos_y = max_y;

	tarea->tiempo = max_espera;

	return tarea;

}

void destruir_patota_sin_tripulantes(t_patota * patota)
{
	free(patota->tareas);
	free(patota->tripulantes);
	free(patota);
}

void destruir_patota(t_patota * patota
		) {
	free(patota->tareas);
	destruir_tripulantes(patota);
	free(patota->tripulantes);
	free(patota);
}

void destruir_tripulantes(t_patota * patota)
{
	for (int i = 0; i < patota->size_tripulantes; ++i)
	{
		free(patota->tripulantes[i]);
	}
}




