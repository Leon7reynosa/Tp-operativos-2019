/*
 * mensajes.c
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#include"mensajes.h"

void enviar_estado(int conexion, estado_request estado){

	void* buffer = malloc(sizeof(estado_request));

	memcpy(buffer, &estado, sizeof(estado_request));

	send(conexion, buffer, sizeof(estado_request), 0);

	free(buffer);

	return;

}

//tipoRequest es una estructura del tipo "request" osea, puede ser selectEstructura, etc, entonces mi idea es depende de lo que llegue, castear a esa estructura
bool enviar_request(request request, int conexion){

	void* buffer;
	bool exito = true;
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

		buffer = serializar_describe(request);
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

	int bytes_enviados = 0;
	int bytes_restantes = bytes;
	int enviados_aux;

	if(&conexion == &socket_lissandra){

		if(!conexion_lissandra){

			socket_lissandra = conectar_servidor(ip_lfs, puerto_lfs);

			if(socket_lissandra <= -1){

				free(buffer);

				return false;

			}else{

				conexion_lissandra = true;

				conexion = socket_lissandra;

				int aux_handshake;

				recv(socket_lissandra, &aux_handshake, sizeof(int), 0); // cada vez que te conectas al LFS te da el value el chotin

			}
		}

	}

	usleep(retardo_lfs * 1000);

	while(bytes_enviados < bytes){

		enviados_aux = send(conexion, buffer + bytes_enviados, bytes_restantes, 0);

		if(enviados_aux == -1){

			bytes_enviados = bytes;

			exito = false;

			if(&socket_lissandra == &conexion){

				conexion_lissandra = false;

				close(socket_lissandra);
			}

		}else{

			bytes_enviados += enviados_aux;

			bytes_restantes -= enviados_aux;

		}
	}

	if(bytes_enviados == bytes){
		exito = true;
	}

	free(buffer);

	return exito;

}


void enviar_dato(t_dato* dato, int conexion, estado_request estado){

	int desplazamiento = 0;
	void* buffer;
	int bytes;

	if(estado == SUCCESS){

		bytes = sizeof(estado_request) + sizeof(dato->timestamp) + sizeof(dato->key) +  sizeof(dato->value->size) + dato->value->size;

		buffer = malloc(bytes);

		memcpy(buffer + desplazamiento, &estado, sizeof(estado_request));
		desplazamiento += sizeof(estado_request);

		memcpy(buffer + desplazamiento, &(dato->timestamp) , sizeof(dato->timestamp));
		desplazamiento += sizeof(dato->timestamp);

		memcpy(buffer + desplazamiento, &(dato->key) , sizeof(dato->key));
		desplazamiento += sizeof(dato->key);

		memcpy(buffer + desplazamiento, &(dato->value->size) , sizeof(dato->value->size));
		desplazamiento += sizeof(dato->value->size);

		memcpy(buffer + desplazamiento, dato->value->buffer , dato->value->size);
		desplazamiento += dato->value->size;
	}
	else{

		bytes = sizeof(estado_request);
		buffer = malloc(bytes);

		memcpy(buffer + desplazamiento, &estado, sizeof(estado_request));
		desplazamiento += sizeof(estado_request);

	}

	send(conexion, buffer, bytes, 0);

	free(buffer);

}

void enviar_metadata(t_list* lista_metadata, int conexion, estado_request estado){

	int bytes;

	int bytes_enviados = 0;
	int bytes_restantes;
	int enviados_aux;

	printf("serializo las metadatas\n");
	void* buffer = serializar_metadata(lista_metadata, &bytes, estado);

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

