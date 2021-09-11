#include "consola.h"

int es_numero(char* check)
{
	int recount = 0;
	for(recount = 0; recount < strlen(check); recount ++)
	{
		if(!isdigit(check[recount]))
		{
			return 0;
		}
	}
	return 1;
}

void mandar_sabotaje(char** args){}
// check_argumentos : Verifica si la cantidad de parametros ingresada es la mínima para ejectuar el programa
int check_argumentos(char** args, int cantArgs)
{
    int i;
    for (i=1; i < cantArgs; i++)
    {
    	if(args[i] == NULL)
    	{
    		puts("No se recibieron suficientes parámetros.");
    		return 0; //Si alguno de los parametros es null, no me sirve.
    	}
    }
    return 1; //Si todos los parametros existen, procedo al siguiente paso
}

int obtener_cant_tripulantes(char* parametroCantTripulante)
{
	if(es_numero(parametroCantTripulante))
	{
		return atoi(parametroCantTripulante);
	}
	else
	{
		return 0;
	}
}

char* leer_archivo(char** args)
{
	 char* path = string_duplicate(args[2]);
	 FILE *file = fopen(path, "r");
	 fseek(file, 0, SEEK_END);
	 long fileSize = ftell(file);
	 fseek(file, 0, SEEK_SET);
	 char *contenido = malloc(fileSize + 1);
	 fread(contenido, 1, fileSize, file);
	 fclose(file);

	 contenido[fileSize] = 0;
	 free(path);
	 return contenido;

}

int es_un_archivo(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
void mockear_patota(char** args)
{
	 if(check_argumentos(args, 2)){
		 puts("Se mockea la patota");

		 if (0) {
			 puts("La patota se creó exitosamente.");
		 } else {
			 puts("ERROR: no se pudo crear la patota");
		 }
	 }
}
//////////////////////////////////////////   COMANDOS DE CONSOLA   //////////////////////////////////////////
void ini_patota(char** args){
	 if(check_argumentos(args, 3)){
		uint32_t cantTripulantes = obtener_cant_tripulantes(args[1]);
		char* path = string_duplicate(args[2]);

		 if (es_un_archivo(path))
		 {
			 if (cantTripulantes != 0)
			 {
				 char* tareas = leer_archivo(args);
				 free(tareas);
				 int params = 3; //Inicializado en 3 porque va a ser el 1er parámetro que va a leer como posición
				 int posiciones = 0; //Cantidad de posiciones que se pasaron por parámetro
				 while (args[params] != NULL)
				 {
					params++;
					posiciones++;
				 }
				 for (int i = 0; i < cantTripulantes; i++)
				 {
					if(i < posiciones && strcmp(args[i+3], ""))
					{
						char** posArgs = string_split(args[i+3], "|"); //+3 por lo mismo de antes, 1er parámetro en el string
						string_iterate_lines(posArgs,(void*)free);
						free(posArgs);
					}
					else
					{

					}
				 }
				if (0) {
					 puts("La patota se creó exitosamente.");
				 }
				 else
				 {
					 puts("ERROR: no se pudo crear la patota");
				 }
			}
		 }
		 else
		 {
			 puts("ERROR: No se pudo encontrar el archivo.");
		 }
		 free(path);

	 }
}

void list_tripulantes(char** args){
}

void ini_planificacion(char** args){
	puts("Arranca planificación.");
}

void pau_planificacion(char** args){
	puts("Pausa planificación.");
}

void obt_bitacora(char** args){
	 if(check_argumentos(args, 2)) {
		 if(es_numero(args[1])) {
			 printf("Mostramos la bitácora del tripulante con TID %s.\n", args[1]);
			 puts("Vos juzgás si laburó o no.");
		 }
	 }
}

// Lista de comandos y su función a ejecutar.
COMANDOS comandos[] = {
        { "INICIAR_PATOTA", 		ini_patota },
        { "LISTAR_TRIPULANTES",		list_tripulantes },
        { "INICIAR_PLANIFICACION", 	ini_planificacion },
        { "PAUSAR_PLANIFICACION", 	pau_planificacion },
        { "OBTENER_BITACORA", 		obt_bitacora },
		{ "MOCK_PATOTA", 			mockear_patota },
		{ "MANDAR_SABOTAJE", 		mandar_sabotaje },
        {(char *) NULL, (Funcion *) NULL}
};

// buscar_comando : revisa si está en el array de comandos a ejecutar.
COMANDOS * buscar_comando(char* linea) {
    int i;

    for (i = 0; comandos[i].nombre; i++){
        // Paso a miniscula y comparo
        string_to_lower(linea);
        char* comando = string_duplicate(comandos[i].nombre);
        string_to_lower(comando);

        if (strcmp (linea, comando) == 0){
            free(comando);
            return (&comandos[i]);
        }
        free(comando);

    }
    return ((COMANDOS *) NULL);
}


// mostrar_comandos : muestra los comandos a ejecutar. Puede ser invocado con 'AYUDA'
void mostrar_comandos (){
	puts("--------------------------------------------------------------------");
	puts("                          CONSOLA PLANIFICADOR");
	puts("                           Escriba un comando");
	puts(" - INICIAR_PATOTA  <cantTripulantes> <archivo> <posX|posY>(opcional)");
	puts(" - LISTAR_TRIPULANTES");
	puts(" - INICIAR_PLANIFICACION");
	puts(" - PAUSAR_PLANIFICACION");
	puts(" - OBTENER_BITACORA <idTripulante>");
	puts(" - AYUDA (Muestra los comandos de la consola.)");
	puts(" - EXIT (Sale del programa.)");
	puts("--------------------------------------------------------------------");
}

// ejecutar_linea : Busca ejecutar el comando indicado
int ejecutar_linea (char * linea){
    char * linea_aux = string_duplicate(linea);
    int i = 0;
    char * funcion;

    if(string_contains(linea, " "))
    {
        while (linea_aux[i] != ' ') i++;

        funcion = malloc((sizeof(char) * i) + 1);
        strncpy(funcion, linea_aux, i);
        funcion[i] = '\0';
    }
    else
    {
        funcion = string_duplicate(linea_aux);
    }

    COMANDOS * comando = buscar_comando(funcion);
    if (comando == NULL)
    {
    	free(funcion);
    	free(linea_aux);
    	return 0;
    }
    else
    {
    	free(funcion);
    	char** args;
		args = string_split(linea_aux, " ");
		//EJECUCION DE FUNCION
		(*(comando->funcion)) (args);
    	free(linea_aux);
    	string_iterate_lines(args, (void*)free);
    	free(args);
    	return 1;
    }
}

// levantar consola : inicializa la consola y todos sus componentes.
void levantar_consola (){
	mostrar_comandos();
    while(1){
    	char* linea = readline("\nIngrese un comando: ");
    	char* aux = malloc(strlen(linea) + 1);
    	strcpy(aux, linea);
    	string_to_upper(aux);
    	if (!strcmp(aux, "EXIT")){
    		free(linea);
    		free(aux);
    		clear_history();
    		puts("Adiós! Gracias por venir!");
    		sleep(1);
    		break;
    	}
    	if (!strcmp(aux, "AYUDA")){
    		add_history(linea);
    		mostrar_comandos();
    	}
    	else if (ejecutar_linea(linea)){
    		add_history(linea);
    	}
    	else
    	{
    		add_history(linea);
    		puts("No se reconoció el comando.");
    	}
    	free(linea);
    	free(aux);
    }
}

