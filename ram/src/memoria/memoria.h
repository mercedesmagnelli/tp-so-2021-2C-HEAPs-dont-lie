#ifndef MEMORIA_MEMORIA_C_
#define MEMORIA_MEMORIA_C_


/*Variables globales*/

/*CODIGOS DE ERROR*/

/*Funciones que pide el TP obligatorias*/

/**
 * @NAME: memalloc
 * @DESC: Asigna un espacio dentro de la memoria principal. Si tiene que generar un espacio, lo hace al final de la
 * lista de direcciones. Si usa un espacio entre lugares ocupados y sobra, genera dos divisiones: la ocupada y la del espacio estante.
 * @RET: Retorna la direccion logica del comienzo de la asignación y NULL en caso de que no pueda asignarlo.
 *
 **/

uint32_t memalloc(uint32_t size);

/**
 * @NAME: memfree
 * @DESC: Libera espacios de memoria. Se encarga de consolidar en caso de que queden dos espacios contiguos libres
 * y de liberar paginas en caso de que con la liberación una página quede vacía.
 *
 **/

void memfree(uint32_t* direccionLogicaALiberar);

/**
 * @NAME: memread
 * @DESC: A partir de una direccion lógica a leer, se retorna lo que está guardado en esa posición.
 *
 **/

void* memread(uint32_t* direccionLogicaALeer);


/**
 * @NAME: memwrite
 * @DESC: Se encarga de escribir informacion en memoria a partir de una direccion lógica
 * Puede llegar a llamar a memalloc si lo que quiere escribirse excede.
 *
 **/


void memwrite(void* valorAEscribir, uint32_t* direccionLogicaAEscribir /*t_list* tablaPaginas*/);

/*Funciones desarrolladas extras para correcto funcionamiento de la memoria*/

#endif /* MEMORIA_MEMORIA_C_ */
