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

void* compactar(char* nombre_tabla);
t_list* filtrar_dato_por_key(char* dato_particiones, t_list* datos_tmpc);
u_int16_t obtener_key_dato(char* dato);
time_t obtener_timestamp_dato(char* dato);
char* buscar_dato_actualizado(char* dato_particion, t_list* dato_tmpc);


#endif /* COMPACTADOR_H_ */
