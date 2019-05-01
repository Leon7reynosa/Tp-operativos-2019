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
