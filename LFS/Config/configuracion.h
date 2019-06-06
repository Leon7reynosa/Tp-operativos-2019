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
#include<commons/bitarray.h>
#include<readline/readline.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>


t_config* g_config;

void creacion_bitmap(void);
void creacion_del_config_lissandra(void);
void creacion_del_config_file_system(void);
void obtener_puerto_ip(int* puerto, char** ip);

#endif /* CONFIGURACION_H_ */
