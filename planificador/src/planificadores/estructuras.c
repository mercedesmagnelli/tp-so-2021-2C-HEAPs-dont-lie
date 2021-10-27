#include "estructuras.h"

uint32_t pid(t_hilo * hilo) {
	return hilo->pid;
}

float estructuras_timestamp_diff(t_timestamp time1, t_timestamp time2) {
	return (time2.tv_sec - time1.tv_sec) + 1e-9 * (time2.tv_nsec - time1.tv_nsec);
}

t_timestamp estructuras_current_timestamp() {
	struct timespec timestamp;

	clock_gettime(CLOCK_REALTIME, &timestamp);

	return timestamp;
}
