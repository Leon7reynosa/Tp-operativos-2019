/*
 * utilCliente.h
 *
 *  Created on: 21 abr. 2019
 *      Author: utnso
 */

#ifndef UTILCLIENTE_H_
#define UTILCLIENTE_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);

#endif /* UTILCLIENTE_H_ */
