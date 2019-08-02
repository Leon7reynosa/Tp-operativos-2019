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
#include<stdbool.h>

#include<commons/log.h>

int socket_lissandra;

bool conexion_lissandra;

t_log* logger_estado;

int conectar_servidor(char* ip, int puerto);
bool comprobar_conexion_lissandra(char* ip_lissandra, int puerto_lissandra);
void desconectar_lissandra(void);

#endif /* CLIENTE_CLIENTE_H_ */
