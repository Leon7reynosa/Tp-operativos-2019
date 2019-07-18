/*
 * requestInsert.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#include"requestInsert.h"
/*
struct insertEstructura{

	int bytes;
	t_stream* tabla;
	u_int16_t key;
	t_stream* value;
	time_t timestamp;

};
*/


insert decodificar_insert(int conexion){

	u_int16_t * key = malloc(sizeof(u_int16_t));
	int* size_tabla = malloc(sizeof(int));
	int* size_value = malloc(sizeof(int));
	time_t* timestamp = malloc(sizeof(time_t));

	if(recv(conexion,size_tabla,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamaño de la tabla.\n");
	}

	char* tabla = malloc(*size_tabla);

	if(recv(conexion, tabla, *size_tabla,0) == -1){
		perror("Fallo al recibir la tabla.\n");
	}

	if(recv(conexion,key,sizeof(u_int16_t),0) == -1){
		perror("Fallo al recibir la key.\n");
	}

	if(recv(conexion,size_value,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamaño del value.\n");
	}

	char* value = malloc(*size_value);

	if(recv(conexion,value, *size_value,0) == -1){
		perror("Fallo al recibir el value.\n");
	}

	if(recv(conexion,timestamp ,sizeof(time_t),0) == -1){
		perror("Fallo al recibir el timestamp.\n");
	}

	string_to_upper(tabla);

	insert dato = crear_dato_insert(tabla, *key, value, *timestamp);

	free(tabla);
	free(key);
	free(value);
	free(timestamp);

	return dato;

}


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
	dato->tabla->size = strlen(tabla) + 1;
	dato->tabla->buffer = malloc(dato->tabla->size);
	memcpy(dato->tabla->buffer, tabla, dato->tabla->size);

	dato->timestamp = timestamp;

	dato->value = malloc(sizeof(t_stream));
	dato->value->size = strlen(value) + 1;
	dato->value->buffer = malloc(dato->value->size);
	memcpy(dato->value->buffer, value, dato->value->size);

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
