/*
 * requestInsert.h
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#ifndef MENSAJERIA_REQUESTINSERT_H_
#define MENSAJERIA_REQUESTINSERT_H_

#include<stdio.h>
#include<stdlib.h>
#include"request.h"

struct insertEstructura{

	int bytes;
	t_stream* tabla;
	u_int16_t key;
	t_stream* value;
	time_t timestamp;

};

//struct insertEstructura;
typedef struct insertEstructura* insert;

insert decodificar_insert(int conexion);
insert crear_dato_insert(char* tabla, u_int16_t key, char* value, time_t timestamp);

#endif /* MENSAJERIA_REQUESTINSERT_H_ */
