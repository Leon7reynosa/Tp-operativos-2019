/*
 * api_memory.h
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#ifndef REQUESTS_H_
#define REQUESTS_H_

#include"comunity_func.h"
#include"Lissandra.h"
#include"file_system.h"
#include"compactador.h"


dato_t* request_select(char* nombre_tabla , int key);
void request_insert(char* nombre_tabla, int key, char* valor, time_t timestamp);
void request_create(char* nombre_tabla, char* consistencia, int particiones, int tiempo_compactacion);


#endif /* REQUESTS_H_ */
