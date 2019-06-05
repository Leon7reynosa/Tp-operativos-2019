/*
 * kernel.h
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<readline/readline.h>
#include<commons/config.h>
#include<commons/string.h>


#include"Config/configuracion.h"
#include"Cliente/cliente.h"
#include"Mensajeria/mensajes.h"


void parsear_LQL(char* path_archivo);

#endif /* KERNEL_H_ */
