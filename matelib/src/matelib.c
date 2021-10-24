
#include "matelib.h"

int mate_init(mate_instance *lib_ref, char *config) {

	//cargar_archivo(config);
	t_instance_metadata* metadata = malloc(sizeof(t_instance_metadata));
	metadata->pid = generar_pid();
	//mas info a definir que hay que cargar
	lib_ref->group_info = metadata;

	t_matelib_nuevo_proceso * nuevo_proceso = shared_crear_nuevo_proceso(metadata->pid);
	int error = enviar_mate_init(nuevo_proceso);

	return error;
}

int mate_close(mate_instance *lib_ref) {

	t_instance_metadata * metadata = lib_ref->group_info;

	t_matelib_nuevo_proceso * nuevo_proceso = shared_crear_nuevo_proceso(metadata->pid);
	int error = enviar_mate_close(nuevo_proceso);
	free(lib_ref->group_info);

	return error;

}



int mate_sem_init(mate_instance *lib_ref, mate_sem_name sem, unsigned int value) {

	t_instance_metadata * metadata = lib_ref->group_info;

	t_matelib_semaforo* nuevo_semaforo = shared_crear_nuevo_semaforo(metadata->pid, sem, value);

	int error = enviar_mate_sem_init(nuevo_semaforo);

	return error;
}

int mate_sem_wait(mate_instance *lib_ref, mate_sem_name sem) {

	t_instance_metadata * metadata = lib_ref->group_info;

	t_matelib_semaforo* semaforo = shared_crear_usar_semaforo(metadata->pid, sem);

	int error = enviar_mate_sem_wait(semaforo);

	return error;

}

int mate_sem_post(mate_instance *lib_ref, mate_sem_name sem) {

	t_instance_metadata * metadata = lib_ref->group_info;

	t_matelib_semaforo* semaforo = shared_crear_usar_semaforo(metadata->pid, sem);

	int error = enviar_mate_sem_post(semaforo);

	return error;

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
