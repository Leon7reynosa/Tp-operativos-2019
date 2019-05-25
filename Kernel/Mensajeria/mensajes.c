/*
 * mensajes.c
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#include"mensajes.h"

void* serializar_mensaje_select(operacion_select* bufferA_serializar, int bytes){

	void* msg_Ser = malloc(bytes);
	int desplazamiento = 0;

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->pedido),sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->size_tabla),sizeof(int));
	desplazamiento +=sizeof(int);

	memcpy(msg_Ser + desplazamiento,bufferA_serializar->nombre_tabla,bufferA_serializar->size_tabla);
	desplazamiento +=bufferA_serializar->size_tabla;

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->key),sizeof(u_int16_t));
	desplazamiento += sizeof(u_int16_t);

	return msg_Ser;
}

void* serializar_mensaje_insert(operacion_insert* bufferA_serializar, int bytes){
	void* msg_Ser = malloc(bytes);
	int desplazamiento = 0;

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->pedido),sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->size_tabla),sizeof(int));
	desplazamiento +=sizeof(int);

	memcpy(msg_Ser + desplazamiento,bufferA_serializar->nombre_tabla,bufferA_serializar->size_tabla);
	desplazamiento +=bufferA_serializar->size_tabla;

	memcpy(msg_Ser + desplazamiento,&(bufferA_serializar->key),sizeof(u_int16_t));
	desplazamiento += sizeof(u_int16_t);

	memcpy(msg_Ser + desplazamiento, &(bufferA_serializar->size_value), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(msg_Ser + desplazamiento, bufferA_serializar->value, bufferA_serializar->size_value);
	desplazamiento += bufferA_serializar->size_value;

	memcpy(msg_Ser + desplazamiento, &(bufferA_serializar->timestamp), sizeof(time_t));
	desplazamiento += sizeof(time_t);


	return msg_Ser;
}
/*
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
*/
void eliminar_operacion_select(operacion_select* buffer){

	free(buffer->nombre_tabla);
	free(buffer);

}

void eliminar_tStream(t_stream* tStream){

	free(tStream->buffer);
	free(tStream);

}
