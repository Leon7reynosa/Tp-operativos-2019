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

void* serializar_dato_t(dato_t* dato_a_serializar){

	//KEY-TAMANIOVALOR-VALOR-TIMESTAMṔ
	int bytes = sizeof(int) + sizeof(int) + sizeof(dato_a_serializar->value) + sizeof(time_t);
	void* serializado = malloc(bytes);
	int desplazamiento = 0;

	printf("el valor es: %s\n", dato_a_serializar->value);
	printf("el timestamp es: %d\n" , dato_a_serializar->timestamp );
	int tamanio = strlen(dato_a_serializar->value);
	printf("el tamanio es: %d\n", tamanio );

	memcpy(serializado + desplazamiento , &(dato_a_serializar->key) , sizeof(int));
	desplazamiento += sizeof(int);

	 //si no le llega bien, el problema esta aca

	memcpy(serializado + desplazamiento , tamanio , sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(serializado + desplazamiento , dato_a_serializar->value , tamanio );
	desplazamiento += sizeof(int);

	memcpy(serializado + desplazamiento , &(dato_a_serializar->timestamp) , sizeof(time_t));
	desplazamiento += sizeof(time_t);

	return serializado;
}

void mandar_select(int conexion , dato_t* dato){

	printf("dato value: %s\n" , dato->value);

	void* buffer = serializar_dato_t(dato);

	send(conexion, buffer, sizeof(buffer) , 0);

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
