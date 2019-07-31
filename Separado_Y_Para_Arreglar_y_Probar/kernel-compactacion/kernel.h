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
#include<pthread.h>
#include<commons/log.h>
#include<commons/collections/queue.h>


#include"API_Kernel.h"
#include"Mensajeria/Cliente/cliente.h"
#include"Mensajeria/mensajes.h"
#include"consola.h"
#include"gossiping.h"
#include"metadata.h"
#include"metricas.h"
#include"inotify_kernel.h"

pthread_t hilo_gossiping;
pthread_t hilo_consola;
pthread_t hilo_planificador;
pthread_t hilo_refresh_metadata;
pthread_t hilo_metrics;
pthread_t hilo_inotify;


void inicializar_semaforos_consistencias(void);
void incializar_semaforos_metricas(void);

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
