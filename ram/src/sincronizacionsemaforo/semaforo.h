
#ifndef SINCRONIZACIONSEMAFORO_SEMAFORO_H_
#define SINCRONIZACIONSEMAFORO_SEMAFORO_H_

#include <semaphore.h>
#include "../../../shared/logger.h"

void semaforo_iniciar();

void semaforo_destruir();

void semaforo_post_fin();
void semaforo_wait_fin();

#endif /* SINCRONIZACIONSEMAFORO_SEMAFORO_H_ */
