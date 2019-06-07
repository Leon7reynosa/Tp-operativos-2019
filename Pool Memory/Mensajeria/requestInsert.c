/*
 * requestInsert.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#include"requestInsert.h"

struct insertEstructura{

	int bytes;
	t_stream* tabla;
	u_int16_t key;
	t_stream* value;
	time_t timestamp;

};

void* serializar_insert(request request_insert){

	insert dato = (insert)(request_insert->tipo_request);
	void* buffer_serializado = malloc(dato->bytes);
	int desplazamiento = 0;


	memcpy(buffer_serializado + desplazamiento, &(request_insert->cod_op), sizeof(request_insert->cod_op));
	desplazamiento += sizeof(request_insert->cod_op);

	memcpy(buffer_serializado + desplazamiento, &(dato->tabla->size), sizeof(dato->tabla->size));
	desplazamiento += sizeof(dato->tabla->size);

	memcpy(buffer_serializado + desplazamiento, dato->tabla->buffer, dato->tabla->size);
	desplazamiento += dato->tabla->size;

	memcpy(buffer_serializado + desplazamiento, &(dato->key), sizeof(dato->key));
	desplazamiento += sizeof(dato->key);

	memcpy(buffer_serializado + desplazamiento, &(dato->value->size), sizeof(dato->value->size));
	desplazamiento += sizeof(dato->value->size);

	memcpy(buffer_serializado + desplazamiento, dato->value->buffer, dato->value->size);
	desplazamiento += dato->value->size;

	memcpy(buffer_serializado + desplazamiento, &(dato->timestamp), sizeof(dato->timestamp));
	desplazamiento += sizeof(dato->timestamp);


	return buffer_serializado;
}

insert crear_dato_insert(char* tabla, u_int16_t key, char* value, time_t timestamp){

	struct insertEstructura* dato = malloc(sizeof(struct insertEstructura));

	dato->key = key;

	dato->tabla = malloc(sizeof(t_stream));
	dato->tabla->buffer = tabla;
	dato->tabla->size = strlen(tabla) + 1;

	dato->timestamp = timestamp;

	dato->value = malloc(sizeof(t_stream));
	dato->value->buffer = value;
	dato->value->size = strlen(value) + 1;

	dato->bytes = sizeof(cod_operacion) + sizeof(dato->tabla->size) + dato->tabla->size + sizeof(dato->key)
				  + sizeof(dato->value->size) + dato->value->size + sizeof(dato->timestamp);

	return dato;
}

void liberar_dato_insert(insert dato){

	free(dato->tabla->buffer);
	free(dato->tabla);

	free(dato->value->buffer);
	free(dato->value);

	free(dato);

}
