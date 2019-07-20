/*
 * t_dato.h
 *
 *  Created on: 23 jun. 2019
 *      Author: utnso
 */

#ifndef MENSAJERIA_T_DATO_H_
#define MENSAJERIA_T_DATO_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"request.h"

typedef struct{

	int size;
	void* buffer;

}t_stream;


typedef struct{

	int key;
	t_stream* value;
	time_t timestamp;

}t_dato;


t_dato* crear_t_dato(u_int16_t key, time_t timestamp , char* value);
void liberar_t_dato(t_dato*);

#endif /* MENSAJERIA_T_DATO_H_ */
