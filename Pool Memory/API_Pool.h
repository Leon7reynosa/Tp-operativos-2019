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
#include"memoria.h"


//estos dos creo que estarian en servidor y cliente (deberian)

Dato request_select(Memoria memoria, char* tabla, u_int16_t key);
void request_insert(Memoria memoria, char* tabla, u_int16_t key, char* value );
void request_create(Memoria memoria,  char* tabla, char* consistencia, int numero_particiones, int compactacion  );

#endif /* API_POOL_H_ */
