/*
 * particion_contendio.h
 *
 *  Created on: 7 jul. 2019
 *      Author: utnso
 */

#ifndef PROPIEDADES_FUNCIONAMIENTO_PARTICION_CONTENIDO_H_
#define PROPIEDADES_FUNCIONAMIENTO_PARTICION_CONTENIDO_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>
#include"Mensajeria/mensajes.h"
#include"fcntl.h"
#include"stdbool.h"
#include"configuracion.h"
#include"bloque.h"
#include"metadata.h"

struct particionContenido{
	int size;
	t_list* bloques;
};

typedef struct particionContenido* Particion;

Particion crear_particion(int size);
void liberar_particion(Particion particion);
Particion leer_particion(char* path_particion);
void actualiar_particion(char* path_particion);
void mostrar_particion(Particion particion);
bool es_dato_final(char* dato_a_analizar);
bool es_dato_cortado(char* dato_a_analizar);
dato_t* buscar_dato_en_particion(char*	 path , int key);
bool bloque_valido(int bloque);
void aniadir_bloque(Particion particion, int bloque);

#endif /* PROPIEDADES_FUNCIONAMIENTO_PARTICION_CONTENIDO_H_ */
