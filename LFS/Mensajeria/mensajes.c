/*
 * mensajes.c
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#include"mensajes.h"

void* serializar_mensaje(t_stream* bufferA_serializar, int bytes){

	void* msg_Ser = malloc(bytes);
	int desplazamiento = 0;

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->size),sizeof(int));
	desplazamiento += sizeof(int);
	//4 .
	memcpy(msg_Ser + desplazamiento,bufferA_serializar->buffer,bufferA_serializar->size);
	desplazamiento += bufferA_serializar->size;
	//4hola
	return msg_Ser;
}

void* serializar_dato_t(dato_t* dato_a_serializar, int* bytes){

	void* dato_serializado;

	t_dato_serializado* dato_aux = malloc(dato_serializado);

	/* en vez de tabla es value, esta mal el nombre*/
	dato_aux->tabla = malloc(sizeof(t_stream));

	memcpy(&(dato_aux->key), &(dato_a_serializar->key), sizeof(int));

	dato_aux->tabla->size = strlen(dato_a_serializar->value) + 1;

	dato_aux->tabla->buffer = malloc(dato_aux->tabla->size);

	memcpy(dato_aux->tabla->buffer, dato_a_serializar->value, dato_aux->tabla->size);

	memcpy(&(dato_aux->timestamp), &(dato_a_serializar->timestamp), sizeof(int));

	*bytes = 3*sizeof(int) + dato_aux->tabla->size;

	dato_serializado = malloc(*bytes);

	printf("PRUEBAS LEON \n");
	printf("KEY = %d\n",dato_aux->key);
	printf("SIZE = %d\n",dato_aux->tabla->size);

	char* value = malloc(dato_aux->tabla->size);
	memcpy(value, dato_aux->tabla->buffer, dato_aux->tabla->size);

	printf("VALUE = %s\n",value);
	printf("TIMESTAMP = %d\n",dato_aux->timestamp);

	free(value);

	int desplazamiento = 0;

	memcpy(dato_serializado + desplazamiento, &(dato_aux->key), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(dato_serializado + desplazamiento, &(dato_aux->tabla->size), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(dato_serializado + desplazamiento, dato_aux->tabla->buffer, dato_aux->tabla->size);
	desplazamiento += dato_aux->tabla->size;

	memcpy(dato_serializado + desplazamiento, &(dato_aux->timestamp), sizeof(int));
	desplazamiento += sizeof(int);

	free(dato_aux->tabla->buffer);
	free(dato_aux->tabla);
	free(dato_aux);

	return dato_serializado;

}

void mandar_select(int conexion , dato_t* dato){

	int bytes;
	void* buffer;

	buffer = serializar_dato_t(dato, &bytes);

	printf("SE SERIALIZO TODO PIOLA WACHIM\n");

	send(conexion, buffer, bytes, 0);

	printf("SE MANDO TODO PIOLA WACHIM\n");

	free(buffer);

}

void mandar_mensaje(int conexion){

	char* buffer = readline(">>");

	t_stream* bufferA_serializar = malloc(sizeof(t_stream));

	bufferA_serializar->size = strlen(buffer) + 1;
	bufferA_serializar->buffer = malloc(bufferA_serializar->size);
	memcpy(bufferA_serializar->buffer,buffer,bufferA_serializar->size);

	void* buffer_serializado;
	//tamaño + mensaje
	int bytes = sizeof(int) + bufferA_serializar->size;

	buffer_serializado = serializar_mensaje(bufferA_serializar, bytes);

	send(conexion, buffer_serializado, bytes, 0);

	free(buffer_serializado);
	eliminar_tStream(bufferA_serializar);

}

void eliminar_tStream(t_stream* tStream){

	free(tStream->buffer);
	free(tStream);

}
