
#include "matelib.h"

int mate_init(mate_instance *lib_ref, char *config) {
	return 0;
}

int mate_close(mate_instance *lib_ref) {
	return 0;
}



int mate_sem_init(mate_instance *lib_ref, mate_sem_name sem, unsigned int value) {
	return 0;
}

int mate_sem_wait(mate_instance *lib_ref, mate_sem_name sem) {
	return 0;
}

int mate_sem_post(mate_instance *lib_ref, mate_sem_name sem) {
	return 0;
}

int mate_sem_destroy(mate_instance *lib_ref, mate_sem_name sem) {
	return 0;
}



int mate_call_io(mate_instance *lib_ref, mate_io_resource io, void *msg) {
	return 0;
}



mate_pointer mate_memalloc(mate_instance *lib_ref, int size) {
	return 0;
}

int mate_memfree(mate_instance *lib_ref, mate_pointer addr) {
	return 0;
}

int mate_memread(mate_instance *lib_ref, mate_pointer origin, void *dest, int size) {
	return 0;
}

int mate_memwrite(mate_instance *lib_ref, void *origin, mate_pointer dest, int size) {
	return 0;
}
