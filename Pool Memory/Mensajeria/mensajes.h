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

#include"requestCreate.h"
#include"requestInsert.h"
#include"requestSelect.h"
#include"Cliente/cliente.h"


void enviar_request(cod_operacion cod_op, void* tipoRequest);
void liberar_request(request dato);
void mandar_mensaje(int conexion);
void eliminar_tStream(t_stream* tStream);


#endif /* MENSAJES_H_ */
