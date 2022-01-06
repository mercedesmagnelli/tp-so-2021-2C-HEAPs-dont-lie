#include "estructuras.h"

uint32_t pid(t_hilo * hilo) {
	return hilo->pid;
}

double estructuras_timestamp_diff(double time1, double time2) {
	return time2 - time1;
}

double estructuras_current_timestamp() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long long result = (((unsigned long long)tv.tv_sec) * 1000 + ((unsigned long long)tv.tv_usec) / 1000);
	double a = result;
	return a;
}
