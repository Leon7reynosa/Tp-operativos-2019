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

bool enviar_request(cod_operacion cod_op, void* tipoRequest, int  conexion_memoria){

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

		printf("timestamp que vamos a enviar: %d\n" , ((insert)(request->tipo_request))->timestamp);

		break;

	case CREATE:

		buffer = serializar_create(request);
		bytes = ((create)(request->tipo_request))->bytes;
		break;

	case DESCRIBE:

		buffer = serializar_describe(request);
		bytes = ((describe_t)(request->tipo_request))->bytes;

		break;

	case DROP:

		buffer = serializar_drop(request);
		bytes = ((Drop)(request->tipo_request))->bytes;

		break;

	case JOURNAL:

		buffer = serializar_journal(request);
		bytes = sizeof(cod_operacion);

		break;

	default:
		//no deberia entrar aca
		break;
	}

	int error_send = send(conexion_memoria , buffer, bytes, 0);

	if(error_send < 0){
		perror("FALLO EL SEND");

		return false;
	}

	//free(buffer);

	free(request);

	return true;
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
		case DESCRIBE:
			liberar_dato_describe(dato->tipo_request);
			break;
		case DROP:

			liberar_drop(dato->tipo_request);
			break;

		case JOURNAL:

			//NO SE LIBERA EL TIPO_REQUEST POR QUE ES NULL

			break;

		default:
			printf(">>No se pudo liberar la request\n");
			break;

	}

	free(dato);

}
