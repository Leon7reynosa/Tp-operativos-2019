/*
 * pool.h
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#ifndef POOL_H_
#define POOL_H_

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include"Mensajeria/mensajes.h"
#include"Config/configuracion.h"
#include"Servidor/servidor.h"

int socket_sv;
int conexion_lissandra;

void* conectar_lissandra(void);
void* conectar_kernel(void);

#endif /* POOL_H_ */
