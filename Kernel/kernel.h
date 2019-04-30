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
#include<commons/config.h>
#include<readline/readline.h>
#include<Cliente/cliente.h>
#include<commons/config.h>

char* obtenerIp(void);
char* obtenerPuerto (void);


#endif /* KERNEL_H_ */
