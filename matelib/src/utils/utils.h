#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <stdint.h>

typedef struct {
	uint32_t pid;
	char * ip;
	int port;
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;
}t_instance_metadata;

#endif /* UTILS_UTILS_H_ */
