/*
 * cliente.h
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#ifndef CLIENTE_CLIENTE_H_
#define CLIENTE_CLIENTE_H_


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

int socket_lissandra;
int socket_kernel;

int conectar_servidor(char* ip, int puerto);


#endif /* CLIENTE_CLIENTE_H_ */
