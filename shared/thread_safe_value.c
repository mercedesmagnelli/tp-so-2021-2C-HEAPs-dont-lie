#include "thread_safe_value.h"

t_safe_value * crear_value_safe(void * value) {
	t_safe_value * safe_value = malloc(sizeof(t_safe_value));

	pthread_mutex_init(&safe_value->mutex_t, NULL);
	safe_value->value = value;

	return safe_value;
}

t_safe_value * crear_value_safe_int(int new_value) {
	int * value = malloc(sizeof(int));
	*value = new_value;

	return crear_value_safe((void *) value);
}

void set_value_safe(t_safe_value * safe_value, void * value) {
	pthread_mutex_lock(&safe_value->mutex_t);

	free(safe_value->value);
	safe_value->value = value;

	pthread_mutex_unlock(&safe_value->mutex_t);
}

void set_value_safe_int(t_safe_value * safe_value, int new_value) {
	int *valor = (int *) malloc(sizeof(int));
	*valor = new_value;

	set_value_safe(safe_value, (void *) valor);
}

void * get_value_safe(t_safe_value * safe_value) {
	void * value_return;
	pthread_mutex_lock(&safe_value->mutex_t);

	value_return = safe_value->value;

	pthread_mutex_unlock(&safe_value->mutex_t);
	return value_return;
}

int get_value_safe_int(t_safe_value * safe_value) {
	int * socket_sabotajes = (int *) get_value_safe(safe_value);

	return * socket_sabotajes;
}

void destroy_value_safe(t_safe_value * safe_value) {
	pthread_mutex_destroy(&safe_value->mutex_t);
	free(safe_value->value);
	free(safe_value);
}
