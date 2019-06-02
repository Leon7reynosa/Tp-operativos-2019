/*
 * funciones_requests.h
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#ifndef FUNCIONES_REQUESTS_H_
#define FUNCIONES_REQUESTS_H

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

typedef struct{

	char* nombre_tabla;
	t_list* primer_elemento; //el data va a tener un puntero a dato_t

}memoria_t ;

t_list* dato_memtable; // el data va a tener un puntero a memoria_t

int existe_la_tabla(char* tabla);
int calcular_particion( int particion_metadata ,int key);
dato_t* crear_dato(int clave, char* valor, time_t tiempo);
void ingresar_a_memtable(dato_t* dato_a_ingresar, char* nombre_tabla);
memoria_t* obtener_memoria_tabla(char* nombre_tabla);
#endif /* FUNCIONES_REQUESTS_H_ */
