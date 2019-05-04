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
	char* value;
	time_t timestamp;


}dato_t;

typedef struct{

	dato_t dato_t;
	struct bloque_tabla *dato_sig;

}bloque_tabla;



typedef struct{

	char* nombre_tabla; //uso un string para ver el nombre de la tabla, si quieren despues cambienlo
	bloque_tabla* primer_bloque;
	struct tabla_memtable* sig_tabla;

}tabla_memtable;

void insert(char* ,int , char*,time_t);
void realizar_select(char*, int);
void setear_metadata(void);
int encontrar_tabla(char*);
int calcular_particion(int, int);
void obtener_metadata(char**, int*, int*);
dato_t *crear_dato(int , char* , time_t );
//bloque_tabla *existe_en_memtable(char* );
void ingresar_a_memtable(dato_t*, char* );
tabla_memtable* crear_tabla(char* tabla);
tabla_memtable* ultima_posicion_memtable(void);
tabla_memtable* encontrar_memtable(char* tabla);

#endif /* API_LISSANDRA_H_ */
