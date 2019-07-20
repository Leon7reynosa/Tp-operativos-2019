/*
 * mensajes.c
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#include"mensajes.h"

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
		break;

	case CREATE:

		buffer = serializar_create(request);
		bytes = ((create)(request->tipo_request))->bytes;
		break;

	case DESCRIBE:

		printf("Serializo el describe\n");
		buffer = serializar_describe(request);
		bytes = ((describe_t)(request->tipo_request))->bytes;

		printf("Bytes: %i\n",bytes);
		break;

	case DROP:

		printf("Serializo el drop\ņ");
		buffer = serializar_drop(request);
		bytes = ((Drop)(request->tipo_request))->bytes;

		break;

	default:
		//no deberia entrar aca
		break;
	}

	printf("llegue a enviar\n");
	int error_send = send(conexion_memoria , buffer, bytes, 0);

	if(error_send < 0){
		perror("FALLO EL SEND");

		return false;
	}
	printf("Se envio!\n");
	//free(buffer);

	printf("Libero la request_aux\n");
	liberar_request(request);

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
		default:
			printf("Algo fallo\n");
			break;

	}

	free(dato);

}
