/*
 * compactador.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef COMPACTADOR_H_
#define COMPACTADOR_H_

#include "configuracion.h"
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"Mensajeria/mensajes.h"
#include"comunity_func.h"
#include"metadata.h"
#include"loggers.h"

#include"pthread.h"

typedef struct{

	pthread_t hilo_compactacion;
	pthread_mutex_t mutex_tabla;
	int tiempo_compactacion;
	char* nombre_tabla;

}thread_args;

t_dictionary* diccionario_compactador;

void* compactar(char* nombre_tabla);
t_list* filtrar_dato_por_key(char* dato_particiones, t_list* datos_tmpc);
void* ciclo_compactacion(thread_args* argumentos);
u_int16_t obtener_key_dato(char* dato);
time_t obtener_timestamp_dato(char* dato);
char* buscar_dato_actualizado(char* dato_particion, t_list* dato_tmpc);
char* comparar_y_obtener_dato_actualizado(char* dato_a_ser_comparado, char* dato_a_analizar);


#endif /* COMPACTADOR_H_ */
