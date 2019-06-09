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


int puerto_escucha;
char* ip_lfs;
int puerto_lfs;
//char** seeds;
//int* puertos;
time_t retardo_memoria;
time_t retardo_lfs;
int tamanio;
time_t tiempo_journal;
time_t tiempo_gossiping;
int numero_memoria;


t_config* g_config;


void creacion_del_config(void);
void obtener_datos_config(void);

#endif /* CONFIGURACION_H_ */
