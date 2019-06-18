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

int grado_multiprocesamiento;
int ip_memoria;
int puerto_memoria;
int quantum;
int tiempo_gossiping_kernel;


void creacion_del_config(void);
void obtener_datos_config(void);

#endif /* CONFIGURACION_H_ */
