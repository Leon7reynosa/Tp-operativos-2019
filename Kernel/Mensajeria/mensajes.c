/*
 * mensajes.c
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#include"mensajes.h"

void enviar_request(cod_operacion cod_op, void* tipoRequest){

	void* buffer;
	int bytes = 0;
	request request = crear_request(cod_op, tipoRequest);

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

	default:
		//no deberia entrar aca
		break;
	}

	send(conexion_memoria, buffer, bytes, 0);

	free(buffer);

	liberar_request(request);

}


void liberar_request(request dato){

	switch(dato->cod_op){

		case SELECT:
			liberar_dato_select(dato->tipo_request);
			break;
		case INSERT:
			liberar_dato_insert(dato->tipo_request);
			break;
		case CREATE:
			liberar_dato_create(dato->tipo_request);
			break;
		default:
			printf("Algo fallo\n");
			break;

	}

	free(dato);

}
