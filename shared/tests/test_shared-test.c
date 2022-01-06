/*
 * test_saludo.c
 *
 *  Created on: 30 abr. 2021
 *      Author: utnso
 */


#include "../shared-test.h"
#include <cspecs/cspec.h>

context(test_shared_test) {
	describe("shared-test.c - get_saludo") {
		it ("GET_TEXT_SHARED: Retorna 'Este texto viene de la lib shared'") {
			char * value = get_text_shared();

			should_string(value) be equal to("Este texto viene de la lib shared");
		} end
	} end
}
