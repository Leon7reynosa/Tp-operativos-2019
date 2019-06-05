/*
 * comunity_func.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef COMUNITY_FUNC_H_
#define COMUNITY_FUNC_H_

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

typedef struct{

	int key;
	char* value;
	time_t timestamp;

}dato_t;


dato_t* crear_dato(int clave, char* valor, time_t tiempo);
int calcular_particion( int particion_metadata ,int key);
dato_t* timestamp_mas_grande(dato_t* primer_dato , dato_t* segundo_dato);

#endif /* COMUNITY_FUNC_H_ */
