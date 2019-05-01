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
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include<sys/time.h>
#include<sys/types.h>
#include<sys/unistd.h>

#include"Config/configuracion.h"
#include"Servidor/servidor.h"
#include"Mensajeria/mandar.h"
#include"Mensajeria/recibir.h"

void creacion_del_config();

#endif /* POOL_H_ */
