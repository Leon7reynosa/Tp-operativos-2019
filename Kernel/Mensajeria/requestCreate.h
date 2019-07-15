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

#include"request.h"

struct createEstructura{
	int bytes;
	t_stream* tabla;
	t_stream* consistencia;
	int numero_particiones;
	time_t compactacion;

};


//struct createEstructura;
typedef struct createEstructura* create;

create decodificar_create(int conexion);
void* serializar_create(request request_create);
create crear_dato_create(char* tabla, char* consistencia, int particiones, time_t compactacion);
void liberar_dato_create(create dato);

#endif /* MENSAJERIA_REQUESTCREATE_H_ */
