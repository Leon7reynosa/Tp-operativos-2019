/*
 * config.h
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>


t_config* g_config;


void creacion_del_config(void);
void obtener_puerto_ip(int* puerto, char** ip);
int obtener_tamanio_memoria(void);

#endif /* CONFIGURACION_H_ */
