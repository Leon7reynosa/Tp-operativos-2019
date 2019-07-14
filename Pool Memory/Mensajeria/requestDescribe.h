/*
 * requestDescribe.h
 *
 *  Created on: 30 jun. 2019
 *      Author: utnso
 */

#ifndef MENSAJERIA_REQUESTDESCRIBE_H_
#define MENSAJERIA_REQUESTDESCRIBE_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"request.h"


struct describeEstructura{
	int bytes;
	bool global;
	t_stream* tabla;
};

typedef struct describeEstructura* describe_t;

struct metadataEstructura{

	char* tabla;
	char* consistencia;
	int particiones;
	int tiempo_compactacion;
};

typedef struct metadataEstructura*  Metadata;

describe_t decodificar_describe(int conexion);
void* serializar_describe(request request_describe);
describe_t crear_dato_describe(char* nombre_tabla);
void liberar_dato_describe(describe_t dato);

Metadata crear_metadata(char* tabla, char* consistencia, int particiones, int compactacion);
void liberar_metadata(Metadata metadata_a_liberar);

#endif /* MENSAJERIA_REQUESTDESCRIBE_H_ */
