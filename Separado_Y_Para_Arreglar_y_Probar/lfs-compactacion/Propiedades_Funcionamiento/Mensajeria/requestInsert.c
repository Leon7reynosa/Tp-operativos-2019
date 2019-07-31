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

	printf("SIZE TABLA: %i\n", *size_tabla);

	char* tabla = malloc(*size_tabla);

	if(recv(conexion, tabla, *size_tabla,0) == -1){
		perror("Fallo al recibir la tabla.\n");
	}

	printf("tabla: %s\n", tabla);

	if(recv(conexion,key,sizeof(u_int16_t),0) == -1){
		perror("Fallo al recibir la key.\n");
	}

	printf("key: %i\n", *key);

	if(recv(conexion,size_value,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamaño del value.\n");
	}

	printf("Size value: %i\n", *size_value);

	char* value = malloc(*size_value);

	if(recv(conexion,value, *size_value,0) == -1){
		perror("Fallo al recibir el value.\n");
	}

	printf("value: %s\n", value);

	if(recv(conexion,timestamp ,sizeof(time_t),0) == -1){
		perror("Fallo al recibir el timestamp.\n");
	}

	printf("timestamp: %i\n", *timestamp);

	string_to_upper(tabla);

	insert dato = crear_dato_insert(tabla, *key, value, *timestamp);

	free(tabla);
	free(key);
	free(value);
	free(timestamp);

	return dato;

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
