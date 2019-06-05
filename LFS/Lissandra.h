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

typedef struct{

	char* nombre_tabla;
	t_list* primer_elemento; //el data va a tener un puntero a dato_t

}memoria_t ;

t_list* dato_memtable; // el data va a tener un puntero a memoria_t

void ingresar_a_memtable(dato_t* dato_a_ingresar, char* nombre_tabla);
memoria_t* obtener_memoria_tabla(char* nombre_tabla);

#endif /* LISSANDRA_H_ */
