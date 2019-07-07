/*
 * requestDescribe.c
 *
 *  Created on: 30 jun. 2019
 *      Author: utnso
 */

#include"requestDescribe.h"


describe_t decodificar_describe(int conexion){

	int* size_tabla = malloc(sizeof(int));

	if(recv(conexion,size_tabla ,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamaño de la tabla.\n");
	}

	char* tabla = malloc(*size_tabla);

	if(recv(conexion,tabla , *size_tabla,0) == -1){
		perror("Fallo al recibir la tabla.\n");
	}

	describe_t dato = crear_dato_describe(tabla);

	free(size_tabla);
	free(tabla);

	return dato;
}


void* serializar_describe(request request_describe){

	describe_t dato_describe = (describe_t) (request_describe->tipo_request);
	void* buffer_serializado = malloc(dato_describe->bytes);
	int desplazamiento = 0;

	memcpy(buffer_serializado + desplazamiento, &(request_describe->cod_op) , sizeof(request_describe->cod_op) );
	desplazamiento += sizeof(request_describe->cod_op);

	memcpy(buffer_serializado + desplazamiento, &(dato_describe->tabla->size) , sizeof(dato_describe->tabla->size) );
	desplazamiento += sizeof(dato_describe->tabla->size);

	memcpy(buffer_serializado + desplazamiento, dato_describe->tabla->buffer, dato_describe->tabla->size);
	desplazamiento += dato_describe->tabla->size;

	return buffer_serializado;
}


describe_t crear_dato_describe(char* nombre_tabla){

	describe_t dato_describe = malloc(sizeof(struct describeEstructura));

	dato_describe->tabla = malloc(sizeof(t_stream));

	dato_describe->tabla->size = sizeof(nombre_tabla) + 1;

	dato_describe->tabla = malloc(dato_describe->tabla->size);

	memcpy(dato_describe->tabla->buffer , nombre_tabla , dato_describe->tabla->size);

	dato_describe->bytes = sizeof(cod_operacion) + sizeof(dato_describe->tabla->size) + dato_describe->tabla->size;

	return dato_describe;
}

void liberar_dato_describe(describe_t dato){

	free(dato->tabla->buffer);
	free(dato->tabla);

	free(dato);
}
