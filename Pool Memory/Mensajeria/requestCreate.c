/*
 * requestCreate.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#include"requestCreate.h"

struct createEstructura{
	int bytes;
	t_stream* tabla;
	t_stream* consistencia;
	int numero_particiones;
	time_t compactacion;

};

void* serializar_create(request request_create){

	create dato_create = (create)(request_create->tipo_request);
	void* buffer_serializado = malloc(dato_create->bytes);
	int desplazamiento = 0;

	memcpy(buffer_serializado + desplazamiento, &(request_create->cod_op), sizeof(request_create->cod_op) );
	desplazamiento +=  sizeof(request_create->cod_op);

	memcpy(buffer_serializado + desplazamiento, &(dato_create->tabla->size), sizeof(dato_create->tabla->size));
	desplazamiento +=  sizeof(dato_create->tabla->size);

	memcpy(buffer_serializado + desplazamiento, dato_create->tabla->buffer, dato_create->tabla->size);
	desplazamiento += dato_create->tabla->size;

	memcpy(buffer_serializado + desplazamiento, &(dato_create->consistencia), sizeof(dato_create->consistencia->size));
	desplazamiento += sizeof(dato_create->consistencia->size);

	memcpy(buffer_serializado + desplazamiento, dato_create->consistencia->buffer, dato_create->consistencia->size);
	desplazamiento += dato_create->consistencia->size;

	memcpy(buffer_serializado + desplazamiento, &(dato_create->numero_particiones), sizeof(dato_create->numero_particiones));
	desplazamiento += sizeof(dato_create->numero_particiones);

	memcpy(buffer_serializado + desplazamiento, &(dato_create->compactacion), sizeof(dato_create->compactacion));
	desplazamiento += sizeof(dato_create->compactacion);

	return buffer_serializado;

}

create crear_dato_create(char* tabla, char* consistencia, int particiones, time_t compactacion){

	struct createEstructura* dato = malloc(sizeof(struct createEstructura));

	dato->tabla = malloc(sizeof(t_stream));

	dato->tabla->buffer = tabla;
	dato->tabla->size = strlen(tabla) + 1;

	dato->consistencia = malloc(sizeof(t_stream));

	dato->consistencia->buffer = consistencia;
	dato->consistencia->size = strlen(consistencia) + 1;

	dato->numero_particiones = particiones;
	dato->compactacion = compactacion;

	dato->bytes = sizeof(cod_operacion) + sizeof(dato->tabla->size) + dato->tabla->size + sizeof(dato->consistencia->size) + dato->consistencia->size
				  + sizeof(dato->numero_particiones) + sizeof(dato->compactacion);

	return dato;

}

void liberar_dato_create(create dato){

	free(dato->consistencia->buffer);
	free(dato->consistencia);

	free(dato->tabla->buffer);
	free(dato->tabla);

	free(dato);

}

