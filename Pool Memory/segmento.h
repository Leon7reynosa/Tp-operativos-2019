/*
 * segmento.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef SEGMENTO_H_
#define SEGMENTO_H_

#include"pagina.h"

struct  SegmentoEstructura;
typedef struct SegmentoEstructura* Segmento;

Segmento crear_segmento(char* nombre_tabla);
void liberar_segmento(Segmento segmento_a_liberar);
bool existe_pagina(Segmento segmento_tabla, u_int16_t key, Pagina* pagina_encontrado);

#endif /* SEGMENTO_H_ */
