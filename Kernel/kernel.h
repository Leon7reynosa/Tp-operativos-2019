/*
 * kernel.h
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<readline/readline.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/list.h>


#include"Config/configuracion.h"
#include"Cliente/cliente.h"
#include"Mensajeria/mensajes.h"

t_list* cola_exec;


void parsear_LQL(FILE* archivo_lql);
int identificar_request(char* request_lql);
void mandar_request(char* request_lql, int conexion);
/*
//###########################################DESTRIPADORES DEL REQUEST ##########################################################################
void obtener_parametros_select(char* linea_request, char* nombre_tabla, u_int16_t key);
void obtener_parametros_insert(char* linea_request, char* nombre_tabla, u_int16_t key, char* value, time_t timestamp);
void obtener_parametros_insert_sin_timestamp(char* linea_request, char* nombre_tabla, u_int16_t key, char* value);
void obtener_parametros_create(char* linea_request, char* nombre_tabla, char* criterio, int numero_particiones, int tiempo_compactacion);
void obtener_parametros_describe_de_una_tabla(char* linea_request, char* nombre_tabla);
void obtener_parametros_describe(char* linea_request);
void obtener_parametros_drop(char* linea_request, char* nombre_tabla);
*/
//###############################################################################################################################################
#endif /* KERNEL_H_ */
