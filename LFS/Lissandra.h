/*
 * Lissandra.h
 *
 *  Created on: 9 abr. 2019
 *      Author: utnso
 */

#ifndef LISSANDRA_H_
#define LISSANDRA_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stddef.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<time.h>
#include<commons/config.h>
#include<commons/collections/node.h>
#include<commons/collections/list.h>
#include<string.h>
#include<sys/stat.h>
#include"Metadata/metadata.h"

#include"comunity_func.h"

t_dictionary* memtable;

void inicializar_memtable(void);
void ingresar_a_memtable(dato_t* dato_a_ingresar, char* nombre_tabla);
t_list* obtener_tabla(char* nombre_tabla);
dato_t* obtener_dato_con_mayor_timestamp_tabla(char* nombre_tabla, u_int16_t key);

#endif /* LISSANDRA_H_ */
