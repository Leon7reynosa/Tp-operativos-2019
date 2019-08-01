/*
 * consola.h
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<unistd.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include"Mensajeria/requestCreate.h"
#include"Mensajeria/requestDescribe.h"
#include"Mensajeria/requestDrop.h"
#include"Mensajeria/requestInsert.h"
#include"Mensajeria/requestSelect.h"
#include"Mensajeria/mensajes.h"
#include"loggers.h"


void* consola(void* argumento);
char* obtener_string_codigo(cod_operacion codigo);
bool ejecutar_request(cod_operacion codigo_request , char* linea_request);
int obtener_parametros_select(char* linea_request, char** nombre_tabla, u_int16_t* key);
int obtener_parametros_insert(char* linea_request, char** nombre_tabla, u_int16_t* key, char** value, time_t* timestamp);
//int obtener_parametros_insert_sin_timestamp(char* linea_request, char* nombre_tabla, u_int16_t* key, char* value);
int obtener_parametros_create(char* linea_request, char** nombre_tabla, char** criterio, int* numero_particiones, int* tiempo_compactacion);
int obtener_parametros_describe(char* linea_request, char** nombre_tabla);
int obtener_parametros_drop(char* linea_request, char** nombre_tabla);
void menu(void);


#endif /* CONSOLA_H_ */
