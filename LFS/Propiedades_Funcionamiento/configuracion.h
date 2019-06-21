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
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>


t_config* g_config;
char* magic_number;
int blocks;
int block_size;
int tamanio_value_max;
int tiempo_dump;
int puerto_lfs;
char* punto_montaje;
int retardo;
char* ip_lfs;

int tamanioDeValue;

void creacion_bitmap(void);
void creacion_del_config_fileSystem(void);
void creacion_del_metadata_fileSystem(void);
void obtener_puerto_ip(int* puerto, char** ip);
void obtener_datos_config(void);
void obtener_datos_metadata(void);
void crearYObtenerDatos(void);
void obtenerDatos(void);

#endif /* CONFIGURACION_H_ */
