/*
 * dato.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef DATO_H_
#define DATO_H_

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

struct DatoEstructura{

	time_t timestamp;
	u_int16_t key;
	char* value;

};

//struct DatoEstructura;
typedef struct DatoEstructura* Dato;

int tamanio_value;
int tamanio_dato;

Dato crear_dato(u_int16_t key, char* value, time_t timestamp);
void liberar_dato(Dato dato_a_liberar);

#endif /* DATO_H_ */
