/*
 * requestCreate.h
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#ifndef MENSAJERIA_REQUESTCREATE_H_
#define MENSAJERIA_REQUESTCREATE_H_

#include<stdio.h>
#include<stdlib.h>

struct createEstructura;
typedef struct createEstructura* create;

void* serializar_create(request request_create);
create crear_dato_create(char* tabla, char* consistencia, int particiones, time_t compactacion);
void liberar_dato_create(create dato);

#endif /* MENSAJERIA_REQUESTCREATE_H_ */
