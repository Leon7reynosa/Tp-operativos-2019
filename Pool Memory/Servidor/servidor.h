/*
 * servidor.h
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#ifndef CLIENTE_SERVIDOR_H_
#define CLIENTE_SERVIDOR_H_

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
#include<signal.h>

//saque el char* ip;
int iniciar_servidor(int puerto);
int aceptar_conexion(int);

#endif /* CLIENTE_SERVIDOR_H_ */
