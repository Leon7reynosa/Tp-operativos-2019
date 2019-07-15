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
#include"requestSelect.h"
#include"requestInsert.h"
#include"requestCreate.h"

#include<commons/collections/list.h>

#define TAMANIO_MAX_VALUE 50

typedef struct{

	u_int16_t key;
	char* value;
	time_t timestamp;

}dato_t;

typedef struct{

	int key;
	t_stream* tabla;
	time_t timestamp;

}t_dato_serializado;



void* serializar_mensaje(t_stream* bufferA_serializar, int bytes);
void* serializar_dato_t(dato_t* dato_a_serializar, int* bytes);
void mandar_mensaje(int conexion);
void mandar_select(int conexion , dato_t* dato);
void eliminar_tStream(t_stream* tStream);

void enviar_metadata(t_list* metadatas, int conexion);


#endif /* MENSAJES_H_ */
