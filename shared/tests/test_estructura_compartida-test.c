
#include "../shared-test.h"
#include "../estructura_compartida.h"
#include <cspecs/cspec.h>
#include <stdlib.h>

context(test_shared_estructura_compartida) {
	describe("estructura_compartida.c - serializar y deserializar") {
		it ("CREAR_NUEVO_PROCESO - serializar - deserializar") {
			t_matelib_nuevo_proceso * proceso1 = shared_crear_nuevo_proceso(1);

			void * serializado = serializiar_crear_proceso(proceso1);

			t_matelib_nuevo_proceso * proceso2 = deserializar_crear_proceso(serializado);

			should_int(proceso1->pid) be equal to (proceso2->pid);

			free(proceso1);
			free(serializado);
			free(proceso2);
		} end

		it ("CREAR_NUEVO_SEMAFORO - serializar - deserializar") {
			t_matelib_semaforo * proceso1 = shared_crear_nuevo_semaforo(1, "asd", 10);

			void * serializado = serializar_semaforo(proceso1);

			t_matelib_semaforo * proceso2 = deserializar_semaforo(serializado);

			should_int(proceso1->pid) be equal to (proceso2->pid);
			should_string(proceso1->semaforo_nombre) be equal to (proceso2->semaforo_nombre);
			should_int(proceso1->semaforo_valor) be equal to (proceso2->semaforo_valor);

			free(proceso1);
			free(serializado);
			free(proceso2->semaforo_nombre);
			free(proceso2);
		} end

		it ("CREAR_USAR_SEMAFORO - serializar - deserializar") {
			t_matelib_semaforo * proceso1 = shared_crear_usar_semaforo(1, "asd");

			void * serializado = serializar_semaforo(proceso1);

			t_matelib_semaforo * proceso2 = deserializar_semaforo(serializado);

			should_int(proceso1->pid) be equal to (proceso2->pid);
			should_string(proceso1->semaforo_nombre) be equal to (proceso2->semaforo_nombre);

			free(proceso1);
			free(serializado);
			free(proceso2->semaforo_nombre);
			free(proceso2);
		} end

		it ("CREAR_IO - serializar - deserializar") {
			t_matelib_io * proceso1 = shared_crear_io(1, "asd");

			void * serializado = serializar_io(proceso1);

			t_matelib_io * proceso2 = deserializar_io(serializado);

			should_int(proceso1->pid) be equal to (proceso2->pid);
			should_string(proceso1->io_nombre) be equal to (proceso2->io_nombre);

			free(proceso1);
			free(serializado);
			free(proceso2->io_nombre);
			free(proceso2);
		} end

		it ("CREAR_NUEVO_ALLOC - serializar - deserializar") {
			t_matelib_memoria_alloc * proceso1 = shared_crear_nuevo_alloc(1, 25);

			void * serializado = serializar_memoria_alloc(proceso1);

			t_matelib_memoria_alloc * proceso2 = deserializar_memoria_alloc(serializado);

			should_int(proceso1->pid) be equal to (proceso2->pid);
			should_int(proceso1->memoria_size) be equal to (proceso2->memoria_size);

			free(proceso1);
			free(serializado);
			free(proceso2);
		} end

		it ("CREAR_NUEVO_READ - serializar - deserializar") {
			t_matelib_memoria_read * proceso1 = shared_crear_nuevo_read(1, 25, 30);

			void * serializado = serializar_memoria_read(proceso1);

			t_matelib_memoria_read * proceso2 = deserializar_memoria_read(serializado);

			should_int(proceso1->pid) be equal to (proceso2->pid);
			should_int(proceso1->memoria_size) be equal to (proceso2->memoria_size);
			should_int(proceso1->memoria_mate_pointer) be equal to (proceso2->memoria_mate_pointer);

			free(proceso1);
			free(serializado);
			free(proceso2);
		} end

		it ("CREAR_NUEVO_WRITE - serializar - deserializar") {
			t_matelib_memoria_write * proceso1 = shared_crear_nuevo_write(1, sizeof(char) * 8, 30, "PRINTER");

			void * serializado = serializar_memoria_write(proceso1);

			t_matelib_memoria_write * proceso2 = deserializar_memoria_write(serializado);

			should_int(proceso1->pid) be equal to (proceso2->pid);
			should_int(proceso1->memoria_size) be equal to (proceso2->memoria_size);
			should_int(proceso1->memoria_mate_pointer) be equal to (proceso2->memoria_mate_pointer);
			should_string(proceso1->memoria_write) be equal to (proceso2->memoria_write);

			free(proceso1);
			free(serializado);
			free(proceso2->memoria_write);
			free(proceso2);
		} end

		it ("CREAR_NUEVO_FREE - serializar - deserializar") {
			t_matelib_memoria_free * proceso1 = shared_crear_nuevo_free(1, 30);

			void * serializado = serializar_memoria_free(proceso1);

			t_matelib_memoria_free * proceso2 = deserializar_memoria_free(serializado);

			should_int(proceso1->pid) be equal to (proceso2->pid);
			should_int(proceso1->memoria_mate_pointer) be equal to (proceso2->memoria_mate_pointer);

			free(proceso1);
			free(serializado);
			free(proceso2);
		} end

		it ("GET_TEXT_SHARED: Retorna 'Este texto viene de la lib shared'") {
			char * value = get_text_shared();

			should_string(value) be equal to("Este texto viene de la lib shared");
		} end
	} end
}
