/*
 * mensajes.c
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#include"mensajes.h"

struct requestEstructura{

	cod_operacion cod_op;
	int bytes;
	void* tipo_request;

};

void* serializar_mensaje(operacion_select* bufferA_serializar, int bytes){

	void* msg_Ser = malloc(bytes);
	int desplazamiento = 0;

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->pedido),sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->size_tabla),sizeof(int));
	desplazamiento +=sizeof(int);

	memcpy(msg_Ser + desplazamiento,bufferA_serializar->nombre_tabla,bufferA_serializar->size_tabla);
	desplazamiento +=bufferA_serializar->size_tabla;

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->key),sizeof(int));
	desplazamiento += sizeof(int);

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
//tipoRequest es una estructura del tipo "request" osea, puede ser selectEstructura, etc, entonces mi idea es depende de lo que llegue, castear a esa estructura
void enviar_request(cod_operacion cod_op, void* tipoRequest){

	void* buffer;
	int bytes;

	switch(cod_op){

	case SELECT:

		//fijense si quieren hacerlo de otra manera

	case INSERT:

	case CREATE:

	}

}


void eliminar_operacion_select(operacion_select* buffer){

	free(buffer->nombre_tabla);
	free(buffer);

}


void eliminar_tStream(t_stream* tStream){

	free(tStream->buffer);
	free(tStream);

}
