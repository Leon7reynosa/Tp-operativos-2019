/*
 * API_Pool.h
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#ifndef API_POOL_H_
#define API_POOL_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<time.h>
#include<commons/config.h>
#include<string.h>
#include<sys/stat.h>

#include"Config/configuracion.h"

#define TAMANIO_MAX_VALUE 50

typedef enum{
	SELECT, INSERT
}request;

typedef enum{
	FALSE, TRUE
} boolean;

typedef struct{

	int key;
	char* value;
	time_t timestamp;

}dato_t;

typedef struct{

	int numero_pagina;
	dato_t dato;
	boolean flag_modificado;
	struct pagina* siguiente_pagina;

}pagina;

typedef struct{

	char* tabla; //ESTO DESPUES TIENE QUE SER EL PATH
	pagina* primera_pagina;
	struct segmento* siguiente_segmento;

}segmento;

int tamanio_memoria;
segmento* memoria_principal;


void realizar_select(char* nombre_tabla, int key);
void insert(char* nombre_tabla, int key, char* value);
dato_t* buscar_key(int key, segmento* segmento_tabla);
dato_t* pedir_key_a_LFS(int key, char* nombre_tabla);


#endif /* API_POOL_H_ */
