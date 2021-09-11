#include "../src/conexion_filesystem/conexion_filesystem.h"
#include "../src/configuracion/configuracion_guardada.h"
#include "../../shared/logger.h"
#include <cspecs/cspec.h>
#include <stdlib.h>

context(test_conexion_filesystem) {
	describe("conexion_filesystem.h - _manejar_mensajes_filesystem") {

		it("Si llega un mensaje de desconexión retorna error") {

			t_prot_mensaje* mensaje_test = (t_prot_mensaje*)  malloc(sizeof(t_prot_mensaje));

			mensaje_test->head = DESCONEXION;
			mensaje_test->payload = NULL;

			int resultado = manejar_mensajes_filesystem(mensaje_test);

			should_bool(resultado) be equal to (-1);
		} end

		it("Si llega un mensaje de error retorna error") {

			t_prot_mensaje* mensaje_test = (t_prot_mensaje*)  malloc(sizeof(t_prot_mensaje));

			mensaje_test->head = FALLO_AL_RECIBIR;
			mensaje_test->payload = NULL;

			int resultado = manejar_mensajes_filesystem(mensaje_test);

			should_bool(resultado) be equal to (-2);
		} end

		it("Si llega otro tipo de mensaje retorna error") {

			t_prot_mensaje* mensaje_test = (t_prot_mensaje*)  malloc(sizeof(t_prot_mensaje));

			mensaje_test->head = CACHO_DE_TEXTO;
			mensaje_test->payload = NULL;

			int resultado = manejar_mensajes_filesystem(mensaje_test);

			should_bool(resultado) be equal to (-3);
		} end

		it("Si llega un mensaje de sabotaje retorna OK") {

			t_prot_mensaje* mensaje_test = (t_prot_mensaje*)  malloc(sizeof(t_prot_mensaje));

			mensaje_test->head = SABOTAJE_M_D;
			mensaje_test->payload = NULL;

			int resultado = manejar_mensajes_filesystem(mensaje_test);

			should_bool(resultado) be equal to (0);
		} end

	} end


}
