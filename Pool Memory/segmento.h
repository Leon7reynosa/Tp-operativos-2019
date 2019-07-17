/*
 * segmento.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef SEGMENTO_H_
#define SEGMENTO_H_

#include<stdio.h>
#include<stdlib.h>
#include"pagina.h"
#include<commons/collections/list.h>

struct SegmentoEstructura{

	char* nombre_tabla;
	t_list* Tabla_paginas; //en el campo "data" va a contener un puntero a una estructura "pagina"


};

//struct  SegmentoEstructura;
typedef struct SegmentoEstructura* Segmento;

Segmento crear_segmento(char* nombre_tabla);
void liberar_segmento(Segmento segmento_a_liberar);
bool existe_pagina(Segmento segmento_tabla, u_int16_t key, Pagina* pagina_encontrado);
void sacar_pagina_segmento(Segmento segmento, Pagina pagina);
void agregar_pagina(Segmento segmento, Pagina pagina);

Segmento agregar_segmento(char* tabla, t_list* segmentos);

#endif /* SEGMENTO_H_ */
