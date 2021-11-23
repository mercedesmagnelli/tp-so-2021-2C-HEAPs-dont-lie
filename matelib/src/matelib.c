
#include "matelib.h"

int mate_init(mate_instance *lib_ref, char *config) {

	int error = 0;

	init_mutex_log("./matelib.log", "Matelib", 1, LOG_LEVEL_TRACE);


	t_instance_metadata* metadata = malloc(sizeof(t_instance_metadata));
	metadata->pid = generar_pid();
	error = cargar_archivo(metadata, config);

	if(error != STATUS_OK){
		loggear_error("Hubo un error al leer el archivo");
		return EXIT_FAILURE;
	}

	lib_ref->group_info = metadata;

	t_matelib_nuevo_proceso * nuevo_proceso = shared_crear_nuevo_proceso(metadata->pid);

	loggear_debug("[PID: %zu] --- MATE_INIT ", metadata->pid);

	error = enviar_mate_init(metadata, nuevo_proceso);

	if(error !=STATUS_OK){
		loggear_error("Hubo un error al enviar el mensaje");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int mate_close(mate_instance *lib_ref) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_nuevo_proceso * nuevo_proceso = shared_crear_nuevo_proceso(metadata->pid);
	int error = enviar_mate_close(metadata, nuevo_proceso);
	free(lib_ref->group_info);

	return error;

}



int mate_sem_init(mate_instance *lib_ref, mate_sem_name sem, unsigned int value) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_semaforo* nuevo_semaforo = shared_crear_nuevo_semaforo(metadata->pid, sem, value);

	loggear_debug("[PID: %zu] --- MATE_SEM_INIT ", metadata->pid);

	int error = enviar_mate_sem_init(metadata, nuevo_semaforo);

	return error;
}

int mate_sem_wait(mate_instance *lib_ref, mate_sem_name sem) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_semaforo* semaforo = shared_crear_usar_semaforo(metadata->pid, sem);

	loggear_debug("[PID: %zu] --- MATE_SEM_WAIT ", metadata->pid);

	int error = enviar_mate_sem_wait(metadata, semaforo);

	if (error == EXITO_PROCESO_ELIMINADO) {
		loggear_warning("Destruimos el hilo actual");
		pthread_exit(NULL);
	}

	return error;

}

int mate_sem_post(mate_instance *lib_ref, mate_sem_name sem) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_semaforo* semaforo = shared_crear_usar_semaforo(metadata->pid, sem);

	int error = enviar_mate_sem_post(metadata, semaforo);

	return error;

}

int mate_sem_destroy(mate_instance *lib_ref, mate_sem_name sem) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_semaforo* semaforo = shared_crear_usar_semaforo(metadata->pid, sem);

	loggear_debug("[PID: %zu] --- MATE_SEM_DESTROY ", metadata->pid);

	int error = enviar_mate_sem_destroy(metadata, semaforo);

	return error;
}



int mate_call_io(mate_instance *lib_ref, mate_io_resource io, void *msg) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_io* entrada_salida = shared_crear_io(metadata->pid, io);

	int error = enviar_mate_call_io(metadata, entrada_salida);

	return error;
}



mate_pointer mate_memalloc(mate_instance *lib_ref, int size) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_memoria_alloc* alloc = shared_crear_nuevo_alloc(metadata->pid, size);

	mate_pointer direccion_mem = enviar_mate_memalloc(metadata, alloc);

	return direccion_mem;
}

int mate_memfree(mate_instance *lib_ref, mate_pointer addr) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_memoria_free* free = shared_crear_nuevo_free(metadata->pid, addr);

	int error = enviar_mate_memfree(metadata, free);

	return error;

}

int mate_memread(mate_instance *lib_ref, mate_pointer origin, void *dest, int size) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_memoria_read* read = shared_crear_nuevo_read(metadata->pid, size, origin);

	int error = enviar_mate_memread(metadata, read);

	return error;
}

int mate_memwrite(mate_instance *lib_ref, void *origin, mate_pointer dest, int size) {

	t_instance_metadata * metadata = (t_instance_metadata*) lib_ref->group_info;

	t_matelib_memoria_write* write = shared_crear_nuevo_write(metadata->pid, size, dest, origin);

	int error = enviar_mate_memwrite(metadata, write);

	return error;
}


