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
#include<commons/collections/list.h>
#include<unistd.h>

#include"Cliente/cliente.h"

char* ip_escucha;
int puerto_escucha;
char* ip_lfs;
int puerto_lfs;
t_list* ip_seeds;
t_list* puerto_seeds;
time_t retardo_memoria;
time_t retardo_lfs;
int tamanio;
time_t tiempo_journal;
time_t tiempo_gossiping;
int numero_memoria;

int tamanio_value;
int tamanio_dato;


t_config* g_config;

void realizar_handshake(void);
void creacion_del_config(void);
void obtener_datos_config(void);

#endif /* CONFIGURACION_H_ */
