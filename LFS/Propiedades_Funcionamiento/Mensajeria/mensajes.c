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

void enviar_metadata(t_list* metadatas, int conexion, estado_request estado){

	int bytes;
	void* buffer;

	int bytes_restantes;
	int bytes_enviados = 0;
	int enviados_aux;

//	printf("Serializo las metadatas\n");
	buffer = serializar_metadata(metadatas, &bytes, estado);

//	printf("Serialize las metadatas\n");

	bytes_restantes = bytes;

//	printf("bytes a enviar = %i\n", bytes);

	while(bytes_enviados < bytes){

//		printf("Envio las metadata serializada\n");
		enviados_aux = send(conexion, buffer + bytes_enviados, bytes_restantes, 0);

		if(enviados_aux == -1){
			perror("Send tiro -1");
			//ver que hacer aca NADA!
		}

		bytes_enviados += enviados_aux;
		bytes_restantes -= enviados_aux;

	}

	free(buffer);

}

void* serializar_dato_t(dato_t* dato_a_serializar, int* bytes, estado_request estado){

	void* dato_serializado;
	int desplazamiento = 0;

	if(estado == ERROR){

		*bytes = sizeof(estado_request);

		dato_serializado = malloc(*bytes);

		memcpy(dato_serializado + desplazamiento, &(estado), sizeof(estado_request));
		desplazamiento += sizeof(estado_request);

	}
	else{
		int size_value = string_length(dato_a_serializar->value) + 1;
		*bytes = sizeof(estado_request) + sizeof(u_int16_t) + sizeof(time_t) + sizeof(int) + size_value;

		printf("BYTES A ENVIAR: %i\n", *bytes);

		dato_serializado = malloc(*bytes);

		memcpy(dato_serializado + desplazamiento, &(estado), sizeof(estado_request));
		desplazamiento += sizeof(estado_request);

		memcpy(dato_serializado + desplazamiento, &(dato_a_serializar->timestamp), sizeof(time_t));
		desplazamiento += sizeof(time_t);


		memcpy(dato_serializado + desplazamiento, &(dato_a_serializar->key), sizeof(u_int16_t));
		desplazamiento += sizeof(u_int16_t);

		memcpy(dato_serializado + desplazamiento, &(size_value), sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(dato_serializado + desplazamiento, dato_a_serializar->value, size_value);
		desplazamiento += size_value;

		printf("[REQUEST] SE ENVIO KEY: %i\n", dato_a_serializar->key);
		printf("[REQUEST] SE ENVIO EL TIMESTAMP: %i\n", dato_a_serializar->timestamp);
		printf("[REQUEST] SE ENVIO EL SIZE: %i\n", size_value);
		printf("[REQUEST] SE ENVIO VALUE: %s\n", dato_a_serializar->value);

	}

	printf("[REQUEST] SE ENVIO EL ESTADO: %s\n", estado == SUCCESS ? "SUCCESS" : "ERROR");

	return dato_serializado;

}

void mandar_select(int conexion , dato_t* dato, estado_request estado){

	int bytes;
	void* buffer;

//	printf("[REQUEST] Mando el estado de la request\n");

	buffer = serializar_dato_t(dato, &bytes, estado);

	send(conexion, buffer, bytes, 0);

	free(buffer);

//	printf("[REQUEST] Se mando el estado de la request\n");

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

void mandar_estado(int conexion, estado_request estado){

	void* buffer = malloc(sizeof(estado_request));

	memcpy(buffer, &estado , sizeof(estado_request));

	send(conexion, buffer, sizeof(estado_request) ,  0);

	free(buffer);

}

void eliminar_tStream(t_stream* tStream){

	free(tStream->buffer);
	free(tStream);

}
