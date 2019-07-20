/*
 * mensajes.c
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#include"mensajes.h"

//tipoRequest es una estructura del tipo "request" osea, puede ser selectEstructura, etc, entonces mi idea es depende de lo que llegue, castear a esa estructura
void enviar_request(request request, int conexion){

	void* buffer;
	int bytes = 0;

	switch(request->cod_op){

	case SELECT:

		buffer = serializar_select(request);
		bytes = ((select_t)(request->tipo_request))->bytes;
		break;

	case INSERT:

		buffer= serializar_insert(request);
		bytes = ((insert)(request->tipo_request))->bytes;
		break;

	case CREATE:

		buffer = serializar_create(request);
		bytes = ((create)(request->tipo_request))->bytes;
		break;

	case DESCRIBE:

		printf("Serializo buffer\n");
		buffer = serializar_describe(request);
		printf("Saco los bytes\n");
		bytes =  ((describe_t)(request->tipo_request))->bytes;

		break;

	case DROP:

		buffer = serializar_drop(request);
		bytes = ((Drop)(request->tipo_request))->bytes;

		break;

	case GOSSIP:

		buffer = serializar_request_gossiping(request);
		bytes = sizeof(cod_operacion) + ((tabla_gossip_dto)(request->tipo_request))->bytes;

		break;

	default:
		//no deberia entrar aca
		break;
	}
	// ver este send, si manda todo o ke
	printf("mando la request\n");

	int bytes_enviados = 0;
	int bytes_restantes = bytes;
	int enviados_aux;

	while(bytes_enviados < bytes){

		printf("Soy un printf\n");
		enviados_aux = send(conexion, buffer + bytes_enviados, bytes_restantes, 0);

		printf("terminaste despues del printf\n");

		if(enviados_aux == -1){
			perror("Send tiro -1");
			//ver que hacer aca
		}

		bytes_enviados += enviados_aux;
		bytes_restantes -= enviados_aux;

	}

	free(buffer);

}


void enviar_dato(t_dato* dato, int conexion){

	int desplazamiento = 0;
	int bytes = sizeof(dato->timestamp) + sizeof(dato->key) +  sizeof(dato->value->size) + dato->value->size;

	void* buffer = malloc(bytes);

	memcpy(buffer + desplazamiento, &(dato->timestamp) , sizeof(dato->timestamp));
	desplazamiento += sizeof(dato->timestamp);

	memcpy(buffer + desplazamiento, &(dato->key) , sizeof(dato->key));
	desplazamiento += sizeof(dato->key);

	memcpy(buffer + desplazamiento, &(dato->value->size) , sizeof(dato->value->size));
	desplazamiento += sizeof(dato->value->size);

	memcpy(buffer + desplazamiento, dato->value->buffer , dato->value->size);
	desplazamiento += dato->value->size;

	send(conexion, buffer, bytes, 0);

	free(buffer);

}

void enviar_metadata(t_list* lista_metadata, int conexion){

	int bytes;

	int bytes_enviados = 0;
	int bytes_restantes;
	int enviados_aux;

	printf("serializo las metadatas\n");
	void* buffer = serializar_metadata(lista_metadata, &bytes);

	printf("Bytes a enviar: %i\n", bytes);

	bytes_restantes = bytes;

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

	free(buffer);


}

