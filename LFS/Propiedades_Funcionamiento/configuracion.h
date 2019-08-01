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
#include"loggers.h"
#include"compactador.h"


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

pthread_rwlock_t semaforo_bitmap;

pthread_rwlock_t semaforo_tiempo_dump;

pthread_rwlock_t semaforo_retardo;

int tamanioDeValue;

char* obtener_path_config(void);
void creacion_bitmap(void);
void creacion_del_config_fileSystem(void);
void creacion_del_metadata_fileSystem(void);
void obtener_puerto_ip(int* puerto, char** ip);
void obtener_datos_config(void);
void obtener_datos_metadata(void);
void crearYObtenerDatos(void);
void obtenerDatos(void);
bool existe_el_bitmap(void);
void inicializar_loggers(void);
void destruir_loggers(void);
void inicializar_compactador(void);
void inicializar_conexiones(void);
void liberar_globales(void);
void realizar_handshake(int conexion);


#endif /* CONFIGURACION_H_ */
