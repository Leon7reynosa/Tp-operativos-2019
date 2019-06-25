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
#include"Mensajeria/requestGossiping.h"
#include"memoria.h"


//estos dos creo que estarian en servidor y cliente (deberian)

Dato request_select(select_t dato);
void request_insert(insert dato);
void request_create(create dato_create);

#endif /* API_POOL_H_ */
