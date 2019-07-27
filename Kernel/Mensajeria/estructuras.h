/*
 * estructuras.h
 *
 *  Created on: 19 jul. 2019
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct{

	int numero_memoria;
	char* ip;
	int puerto;
	int socket;
	int contador_requests;

}memoria_t;

typedef struct{

	int size;
	void* buffer;

}t_stream;



#endif /* ESTRUCTURAS_H_ */
