#include "algoritmos.h"

double alfa;
t_algoritmo algoritmo;
double current_timestamp;

t_hilo * hilo_obtener_siguiente_sjf(t_list * procesos);
t_hilo * hilo_obtener_siguiente_hrrn(t_list * procesos);

void imprimir_estado_proceso(void * h) {
	t_hilo * hilo = (t_hilo *) h;

	loggear_trace("****************************************");
	loggear_trace("PID: %zu", hilo->pid);
	if (algoritmo == SJF) {
		loggear_trace("\tEstimacion SJF: %2.4f", hilo->estimacion_actual_sjf);
	} else {
		loggear_trace("\tEstimacion HRRN: %2.4f", hilo->estimacion_actual_hrrn);
	}
	loggear_trace("\tEstimacion Anterior: %f", hilo->estimacion_anterior);
	loggear_trace("\tTimestamp Entrar Exec: %f", hilo->timestamp_entrar_exec);
	loggear_trace("\tTimestamp Entrar Ready: %f", hilo->timestamp_entrar_ready);
	loggear_trace("\tTimestamp Salir Exec: %f", hilo->timestamp_salir_exec);
	loggear_trace("\tTimestamp Tiempo Exec: %f", hilo->timestamp_tiempo_exec);
}

void imprimir_estado_colas(t_list * procesos_ready, t_hilo * hilo_seleccionado) {
	loggear_trace("----- Estado de los procesos para planificacion -----");
	if (algoritmo == SJF) {
		loggear_trace("----- Algoritmo: SJF");
	} else {
		loggear_trace("----- Algoritmo: HRRN");
	}
	loggear_trace("----- Alfa: %2.4f", alfa);

	list_iterate(procesos_ready, imprimir_estado_proceso);

	loggear_trace("----- Hilo seleccionado: %zu", hilo_seleccionado->pid);
}

void algoritmos_iniciar() {
	alfa = (double) get_alfa() / 100.0;
	algoritmo = get_algoritmo();

	if (algoritmo == HRRN) {
		// El alfa no es necesario, asi que vamos a setear un valor que sea util para HRRN
		alfa = 0.5;
	}
}

void algoritmos_destruir() {

}

t_hilo * hilo_obtener_siguiente_hilo_a_mover_exec(t_list * procesos_ready) {
	t_hilo * hilo_mover = NULL;
	if (algoritmo == SJF) {
		hilo_mover = hilo_obtener_siguiente_sjf(procesos_ready);
	} else if (algoritmo == HRRN) {
		hilo_mover = hilo_obtener_siguiente_hrrn(procesos_ready);
	}

	imprimir_estado_colas(procesos_ready, hilo_mover);

	return hilo_mover;
}

void aplicar_formula_sjf(void * proceso) {
	t_hilo * hilo = (t_hilo *) proceso;

	hilo->estimacion_actual_sjf = alfa * hilo->timestamp_tiempo_exec + (1.0 - alfa) * hilo->estimacion_anterior; // 0.7 * 2000 => 1400

	// La siguiente vez que se llame, ya tengo el valor del estimado que dio esta vez
	hilo->estimacion_temporal_sjf = hilo->estimacion_actual_sjf;
	//hilo->estimacion_anterior = hilo->estimacion_actual_sjf; // El 2000 pasa a ser 1400
}

void aplicar_formula_hrrn(void * proceso) {
	t_hilo * hilo = (t_hilo *) proceso;

	aplicar_formula_sjf(proceso); // Todos quedan con estimacion_actual_sjf Y estimacion_temporal_sjf

	double tiempo_ready = estructuras_timestamp_diff(hilo->timestamp_entrar_ready, current_timestamp); // Ms desde que entro a READY hasta ahora

	hilo->estimacion_actual_hrrn = 1.0 + (tiempo_ready / hilo->estimacion_actual_sjf);
}

void * get_hilo_minimo_sjf(void * hilo1, void * hilo2) {
	t_hilo * hilo_1 = (t_hilo *) hilo1;
	t_hilo * hilo_2 = (t_hilo *) hilo2;

	//if (hilo_1->estimacion_actual_sjf <= hilo_2->estimacion_actual_sjf) {
	if (hilo_1->estimacion_temporal_sjf <= hilo_2->estimacion_temporal_sjf) {
		return hilo1;
	}

	return hilo2;
}

void * get_hilo_minimo_hrrn(void * hilo1, void * hilo2) {
	t_hilo * hilo_1 = (t_hilo *) hilo1;
	t_hilo * hilo_2 = (t_hilo *) hilo2;

	if (hilo_1->estimacion_actual_hrrn <= hilo_2->estimacion_actual_hrrn) {
		return hilo2;
	}

	return hilo1;
}

t_hilo * hilo_obtener_siguiente_sjf(t_list * procesos) {
	list_iterate(procesos, aplicar_formula_sjf);

	t_hilo * hilo_menor = list_get_minimum(procesos, get_hilo_minimo_sjf);

	hilo_menor->estimacion_anterior = hilo_menor->estimacion_temporal_sjf;

	return hilo_menor;
}

t_hilo * hilo_obtener_siguiente_hrrn(t_list * procesos) {
	current_timestamp = estructuras_current_timestamp();

	list_iterate(procesos, aplicar_formula_hrrn);

	t_hilo * hilo_menor = list_get_minimum(procesos, get_hilo_minimo_hrrn);

	return hilo_menor;
}

