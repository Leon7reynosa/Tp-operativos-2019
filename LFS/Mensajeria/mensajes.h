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

void* serializar_mensaje(t_stream* bufferA_serializar, int bytes);
void mandar_mensaje(int conexion);
void eliminar_tStream(t_stream* tStream);


#endif /* MENSAJES_H_ */
