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

void enviar_metadata(t_list* metadatas, int conexion){

	int bytes;
	void* buffer;

	int bytes_restantes, bytes_enviados = 0, enviados_aux;

	printf("Serializo las metadatas\n");
	buffer = serializar_metadata(metadatas, &bytes);

	printf("Serialize las metadatas\n");

	bytes_restantes = bytes;

	printf("bytes a enviar = %i\n", bytes);

	while(bytes_enviados < bytes){

		printf("Envio las metadata serializada\n");
		enviados_aux = send(conexion, buffer + bytes_enviados, bytes_restantes, 0);

		if(enviados_aux == -1){
			perror("Send tiro -1");
			//ver que hacer aca
		}

		bytes_enviados += enviados_aux;
		bytes_restantes -= enviados_aux;

	}

}

void* serializar_dato_t(dato_t* dato_a_serializar, int* bytes){

	void* dato_serializado;

	int size_value = string_length(dato_a_serializar->value) + 1;
	*bytes = sizeof(u_int16_t) + sizeof(time_t) + sizeof(int) + size_value;

	printf("BYTES A ENVIAR: %i\n", *bytes);

	dato_serializado = malloc(*bytes);

	int desplazamiento = 0;

	memcpy(dato_serializado + desplazamiento, &(dato_a_serializar->timestamp), sizeof(time_t));
	desplazamiento += sizeof(time_t);


	memcpy(dato_serializado + desplazamiento, &(dato_a_serializar->key), sizeof(u_int16_t));
	desplazamiento += sizeof(u_int16_t);

	memcpy(dato_serializado + desplazamiento, &(size_value), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(dato_serializado + desplazamiento, dato_a_serializar->value, size_value);
	desplazamiento += size_value;

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
