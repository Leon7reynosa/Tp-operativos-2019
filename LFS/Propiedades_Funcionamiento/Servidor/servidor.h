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

#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int iniciar_servidor(char* ip, int puerto);
char* obtener_ip_address(void);
int aceptar_conexion(int);

#endif /* CLIENTE_SERVIDOR_H_ */
