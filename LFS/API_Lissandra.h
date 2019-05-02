/*
 * API_Lissandra.h
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */

#ifndef API_LISSANDRA_H_
#define API_LISSANDRA_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<time.h>
#include<commons/config.h>

typedef struct{

	int key;
	int valor;
	time_t timestamp;

}dato_t;

typedef struct{

	dato_t dato_t;
	bloque_tabla* dato_sig;

}bloque_tabla;

typedef struct{

	 bloque_tabla bloques;
	 tabla_memtable* sig_tabla;

}tabla_memtable;

void insert(char* ,int , char*,unsigned);
void realizar_select(char*, int);
void setear_metadata(void);
int existe_la_tabla(char*);
int calcular_particion(int, int);
void obtener_metadata(char**, int*, int*);

#endif /* API_LISSANDRA_H_ */
