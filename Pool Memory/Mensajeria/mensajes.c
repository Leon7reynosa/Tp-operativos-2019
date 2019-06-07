/*
 * mensajes.c
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#include"mensajes.h"

//tipoRequest es una estructura del tipo "request" osea, puede ser selectEstructura, etc, entonces mi idea es depende de lo que llegue, castear a esa estructura
void enviar_request(cod_operacion cod_op, void* tipoRequest){

	void* buffer;
	request request = crear_request(cod_op, tipoRequest);


	switch(request->cod_op){

	case SELECT:

		buffer = serializar_select(request);
		break;

	case INSERT:

		buffer= serializar_insert(request);
		break;

	case CREATE:

		buffer = serializar_create(request);
		break;

	default:
		//no deberia entrar aca
		break;
	}

	//Ahora, hay que ver el tema del lissandra, donde tenemos el socket y esas cosas

	/* aca hay que ver como le pasamos los bytes!!, por que me quedo dentro del tipo_dato y es un void* :,c
	 * capaz hay que ponerlo en la estructura request, envez de en el void* que tiene la estructura request
	 *
	 * send(socket_lissandra, buffer, request->tipo_dato, bytes!!, 0);
	 *
	 *free(buffer);
	 *
	 *liberar_request(request);
	 *
	 *
	 *
	 */

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
			liberar_dato_inser(dato->tipo_request);
			break;
		default:
			//no deberia entrar aca
			break;

	}

	free(dato);

}

