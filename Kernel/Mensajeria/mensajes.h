/*
 * mensajes.h
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#ifndef MENSAJES_H_
#define MENSAJES_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netdb.h>
#include<readline/readline.h>
#include<readline/history.h>

typedef struct{

	int size;
	void* buffer;

}t_stream;

typedef enum{
	SELECT,
	INSERT
}request;


typedef struct{
	request pedido;
	int size_tabla;
	void* nombre_tabla;
	u_int16_t key;

}operacion_select;

typedef struct{
	request pedido;
	int size_tabla;
	void* nombre_tabla;
	u_int16_t key;
	int size_value;
	void* value;
	time_t timestamp;

}operacion_insert;

//void* serializar_mensaje(t_stream* bufferA_serializar, int bytes);
void* serializar_mensaje_select(operacion_select* bufferA_serializar, int bytes);
void* serializar_mensaje_insert(operacion_insert* bufferA_serializar, int bytes);
void mandar_mensaje(int conexion);
void eliminar_tStream(t_stream* tStream);
void eliminar_operacion_select(operacion_select* buffer);


#endif /* MENSAJES_H_ */
