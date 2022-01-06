#include "../src/configuracion/configuracion.h"
#include <cspecs/cspec.h>
#include <stdlib.h>

context(test_archivo_configuracion) {
	describe("configuracion.h - son_validos_los_parametros") {
		it("Si mando 1 argumento devuelve false") {
			char *datos[1] = {"./asdasdads"};

			bool resultado = son_validos_los_parametros(1, datos);

			should_bool(resultado) be falsey;
		} end

		it("Si mando 2 argumentos, pero no es '.config' devuelve false") {
			char *datos[2] = {"./asdasdads", "archivo_config.no_config"};

			bool resultado = son_validos_los_parametros(2, datos);

			should_bool(resultado) be falsey;
		} end

		it("Mando un archivo '.config' valido devuelve true") {
			char *datos[2] = {"./asdasdads", "archivo.config"};

			bool resultado = son_validos_los_parametros(2, datos);

			should_bool(resultado) be truthy;
		} end
	} end

	describe("configuracion.h - iniciar_configuracion") {
		it("Si el archivo es invalido, devuelve error") {
			char *datos[2] = {"./asdasdads", "archivo.invalido"};
			int error = iniciar_configuracion(2, datos);

			should_int(error) be equal to (CONFIG_ARGUMENTOS_INVALIDOS);
		} end

		// No se puede testear mas esta funcion porque requiere de leer el archivo y no vamos a hacer inyeccion de dependencias
	} end
}
