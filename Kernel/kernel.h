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


#include"API_Kernel.h"
#include"Config/configuracion.h"
#include"Mensajeria/Cliente/cliente.h"
#include"Mensajeria/mensajes.h"


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
