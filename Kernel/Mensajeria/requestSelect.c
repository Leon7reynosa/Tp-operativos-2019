/*
 * requestSelect.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#include"requestSelect.h"

/*
struct selectEstructura{

	int bytes;
	t_stream* tabla;
	u_int16_t key;

};
*/


select_t decodificar_select(int conexion){

	printf("entre al decodificar_select\n ");

	int bytes = 0;
	u_int16_t* key = malloc(sizeof(u_int16_t));
	int* size = malloc(sizeof(int));

	printf("entre al decodificar_select\n ");

	bytes = recv(conexion, key ,sizeof(u_int16_t), 0) ;

	printf("bytes: %d\n", bytes);

	if(bytes == -1){
		perror("Fallo al recibir la key.");
	}

	printf("key : %d\n" , *key);

	if(recv(conexion, size, sizeof(int), 0) == -1){
		perror("Fallo al recibir el tamaño de la tabla.");
	}

	printf("size de tabla: %d\n" , *size);

	char* tabla = malloc(*size);

	if(recv(conexion, tabla, *size, 0) == -1){
		perror("Fallo al recibir la tabla.");
	}

	printf("tabla: %s\n", tabla );

	select_t dato = crear_dato_select(tabla, key);

	free(key);
	free(tabla);
	free(size);

	return dato;

}


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
	dato->tabla->buffer = malloc(dato->tabla->size);
	memcpy(dato->tabla->buffer, tabla, dato->tabla->size);

	dato->bytes = sizeof(cod_operacion) + sizeof(dato->tabla->size) + dato->tabla->size + sizeof(dato->key);

	return dato;
}

void liberar_dato_select(select_t dato){

	free(dato->tabla->buffer);
	free(dato->tabla);
	free(dato);

}
