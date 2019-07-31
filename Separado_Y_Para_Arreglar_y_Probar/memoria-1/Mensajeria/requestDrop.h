/*
 * requestDrop.h
 *
 *  Created on: 18 jul. 2019
 *      Author: utnso
 */

#ifndef PROPIEDADES_FUNCIONAMIENTO_REQUESTDROP_H_
#define PROPIEDADES_FUNCIONAMIENTO_REQUESTDROP_H_

#include<stdio.h>
#include<stdlib.h>

#include"request.h"
#include<commons/string.h>

struct dropEstructura{

	int bytes;
	t_stream* tabla;

};

typedef struct dropEstructura* Drop;

Drop crear_drop(char* tabla);
void* serializar_drop(request drop);
Drop decodificar_drop(int conexion);
void liberar_drop(Drop drop_a_liberar);

#endif /* PROPIEDADES_FUNCIONAMIENTO_REQUESTDROP_H_ */
