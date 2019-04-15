/*
 * utilServer.h
 *
 *  Created on: 11 abr. 2019
 *      Author: utnso
 */

#ifndef LFS_UTILSERVER_H_
#define LFS_UTILSERVER_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>

#define IP "127.0.0.1"
#define PUERTO "4444"

t_log* logger;

void* recibir_buffer(int*, int);

int iniciar_servidor(void);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);


#endif /* LFS_UTILSERVER_H_ */
