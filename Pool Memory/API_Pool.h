/*
 * API_Pool.h
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#ifndef API_POOL_H_
#define API_POOL_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<time.h>
#include<commons/config.h>
#include<string.h>
#include<sys/stat.h>
#include<stdbool.h>

#include"Config/configuracion.h"
#include"Mensajeria/recibir.h"
#include"Mensajeria/mensajes.h"

//estos dos creo que estarian en servidor y cliente (deberian)
int socket_sv;
int conexion_lissandra;

void request_select(char* tabla, u_int16_t key);

#endif /* API_POOL_H_ */
