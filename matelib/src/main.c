#include "matelib.h"
#include <pthread.h>
#include <commons/string.h>

//void cerrar_todo();

#define SEMAFORO_SALUDO "SEM_HELLO"

int main(int argc, char** argv) {

	  if(argc < 2){
	        printf("No se ingresó archivo de configuración");
	        exit(EXIT_FAILURE);
	    }

	    char* config = argv[1];

		printf("MAIN - Utilizando el archivo de config: %s\n", config);

		mate_instance instance;

		mate_init(&instance, (char*)config);

	    char saludo[] = "No, ¡hola humedal!\n";

	    mate_pointer saludoRef = mate_memalloc(&instance, strlen(saludo));

	    mate_memwrite(&instance, saludo, saludoRef, strlen(saludo));

	    mate_sem_init(&instance, SEMAFORO_SALUDO, 0);

	    mate_sem_wait(&instance, SEMAFORO_SALUDO);

	    mate_memread(&instance, saludoRef, saludo, strlen(saludo));

	    printf(saludo);



	return EXIT_SUCCESS;
}
/*
void cerrar_todo() {
	conexiones_cerrar_conexiones();
	loggear_info("Cerrada conexion con backend");
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
}
*/
