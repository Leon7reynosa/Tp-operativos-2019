/*
 * pagina.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef PAGINA_H_
#define PAGINA_H_

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include"dato.h"


struct PaginaEstructura;
typedef struct PaginaEstructura* Pagina;

Pagina crear_pagina(Dato nuevo_dato);
void liberar_pagina(Pagina pagina_a_liberar);

#endif /* PAGINA_H_ */
