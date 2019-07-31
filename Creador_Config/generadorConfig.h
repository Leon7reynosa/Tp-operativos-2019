/*
 * generadorConfig.h
 *
 *  Created on: 21 jul. 2019
 *      Author: utnso
 */

#ifndef GENERADORCONFIG_H_
#define GENERADORCONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <pthread.h>

void crear_config_fileSystem(int puerto_escucha, char* punto_montaje, int retardo, int tamanio_value, int tiempo_dump);
void crear_config_metadata_fileSystem(int block_size, int blocks, char* magic_number);
void crear_config_memoria(int puerto_memoria,char* ip_lfs , int puerto_fileSystem, char* ip_seeds, char* puerto_seeds, int retardo_mem, int retardo_fs, int tam_mem, int retardo_journal, int retardo_gossiping, int mem_num);
void crear_config_kernel(char* ip_memoria, int puerto_memoria, int numero_memoria, int quantum, int multiprocesamiento, int metadata_refresh, int tiempo_gossiping,  int sleep_ejecucion, char* punto_montaje);
void generadorConfig(void);

#endif /* GENERADORCONFIG_H_ */
