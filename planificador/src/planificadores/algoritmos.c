#include "algoritmos.h"

float alfa;
t_algoritmo algoritmo;

t_hilo * hilo_obtener_siguiente_sjf(t_list * procesos);
t_hilo * hilo_obtener_siguiente_hrrn(t_list * procesos);

void algoritmos_iniciar() {
	alfa = (float) get_alfa() / 100.0;
	algoritmo = get_algoritmo();

	if (algoritmo == HRRN) {
		// El alfa no es necesario, asi que vamos a setear un valor que sea util para HRRN
		alfa = 0.5;
	}
}

void algoritmos_destruir() {

}

t_hilo * hilo_obtener_siguiente_hilo_a_mover_exec(t_list * procesos_ready) {
	if (algoritmo == SJF) {
		return hilo_obtener_siguiente_sjf(procesos_ready);
	} else if (algoritmo == HRRN) {
		return hilo_obtener_siguiente_hrrn(procesos_ready);
	}

	return NULL;
}

void aplicar_formula_sjf(void * proceso) {
	t_hilo * hilo = (t_hilo *) proceso;

	hilo->estimacion_actual_sjf = alfa * hilo->timestamp_tiempo_exec + (1.0 - alfa) * hilo->estimacion_anterior;

	// La siguiente vez que se llame, ya tengo el valor del estimado que dio esta vez
	hilo->estimacion_anterior = hilo->estimacion_actual_sjf;
}

void aplicar_formula_hrrn(void * proceso) {
	t_hilo * hilo = (t_hilo *) proceso;

	aplicar_formula_sjf(proceso);

	float tiempo_ready = estructuras_timestamp_diff(hilo->timestamp_entrar_ready, estructuras_current_timestamp());

	hilo->estimacion_actual_hrrn = 1 + (tiempo_ready / hilo->estimacion_actual_sjf);
}

void * get_hilo_minimo_sjf(void * hilo1, void * hilo2) {
	t_hilo * hilo_1 = (t_hilo *) hilo1;
	t_hilo * hilo_2 = (t_hilo *) hilo2;

	if (hilo_1->estimacion_actual_sjf <= hilo_2->estimacion_actual_sjf) {
		return hilo1;
	}

	return hilo2;
}

void * get_hilo_minimo_hrrn(void * hilo1, void * hilo2) {
	t_hilo * hilo_1 = (t_hilo *) hilo1;
	t_hilo * hilo_2 = (t_hilo *) hilo2;

	if (hilo_1->estimacion_actual_hrrn <= hilo_2->estimacion_actual_hrrn) {
		return hilo1;
	}

	return hilo2;
}

t_hilo * hilo_obtener_siguiente_sjf(t_list * procesos) {
	list_iterate(procesos, aplicar_formula_sjf);

	t_hilo * hilo_menor = list_get_minimum(procesos, get_hilo_minimo_sjf);

	return hilo_menor;
}

t_hilo * hilo_obtener_siguiente_hrrn(t_list * procesos) {
	list_iterate(procesos, aplicar_formula_hrrn);

	t_hilo * hilo_menor = list_get_minimum(procesos, get_hilo_minimo_hrrn);

	return hilo_menor;
}

