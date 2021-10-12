#ifndef PAGINACION_PAGINACION_H_
#define PAGINACION_PAGINACION_H_


typedef struct{
	//índice: frame
	uint32_t estado;
	uint32_t proceso;
	uint32_t pagina;
}t_frame;


typedef struct {
	t_list* tablaPaginas; //t_list pagina
	uint32_t miss_proceso;
	uint32_t hits_proceso;
}t_proceso;



#endif /* PAGINACION_PAGINACION_H_ */
