/*
 * pagina.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"pagina.h"

struct PaginaEstructura{

	Dato referencia_memoria;  //no libero esto, por que es memoria!
	bool flag_modificado;
	time_t ultimo_uso;

};

void liberar_pagina(Pagina pagina_a_liberar){

	free(pagina_a_liberar);

}


Pagina crear_pagina(Dato dato_en_memoria){

	struct PaginaEstructura pagina = malloc(sizeof(struct PaginaEstructura));

	pagina->referencia_memoria = dato_en_memoria;

	pagina->flag_modificado = 0;

	/* pagina->ultimo_uso = ?*/

	return pagina;

}
