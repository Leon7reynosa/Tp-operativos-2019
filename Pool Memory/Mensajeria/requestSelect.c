/*
 * requestSelect.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#include"requestSelect.h"


struct selectEstructura{

	int bytes;
	t_stream* tabla;
	u_int16_t key;

};

void* serializar_select(request request_select){

	select_t dato_select = (select_t)(request_select->tipo_request);

	void* buffer_serializado = malloc(dato_select->bytes);
	int desplazamiento = 0;

	//primero mando el codigo de operacion
	memcpy(buffer_serializado + desplazamiento, &(request_select->cod_op), sizeof(request_select->cod_op));
	desplazamiento += sizeof(request_select->cod_op);

	//le mando la key
	memcpy(buffer_serializado + desplazamiento, &(dato_select->key), sizeof(dato_select->key));
	desplazamiento += sizeof(dato_select->key);

	//el tamanio de la tabla
	memcpy(buffer_serializado + desplazamiento, &(dato_select->tabla->size), sizeof(dato_select->tabla->size));
	desplazamiento += sizeof(sizeof(dato_select->tabla->size));

	//le mando la tabla
	memcpy(buffer_serializado + desplazamiento, dato_select->tabla->buffer, dato_select->tabla->size);
	desplazamiento += dato_select->tabla->size;

	return buffer_serializado;

}

select_t crear_dato_select(char* tabla, u_int16_t key){

	struct selectEstructura* dato = malloc(sizeof(struct selectEstructura));


	dato->key = key;
	dato->tabla = malloc(sizeof(t_stream));
	dato->tabla->size = strlen(tabla) + 1;
	dato->tabla->buffer = tabla;

	dato->bytes = sizeof(cod_operacion) + sizeof(dato->tabla->size) + dato->tabla->size + sizeof(dato->key);

	return dato;
}

void liberar_dato_select(select_t dato){

	free(dato->tabla->buffer);
	free(dato->tabla);
	free(dato);

}
