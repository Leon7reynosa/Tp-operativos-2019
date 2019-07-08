/*
 * bloque.h
 *
 *  Created on: 7 jul. 2019
 *      Author: utnso
 */

#ifndef PROPIEDADES_FUNCIONAMIENTO_BLOQUE_H_
#define PROPIEDADES_FUNCIONAMIENTO_BLOQUE_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<commons/collections/list.h>
#include"particion_contenido.h"
#include"configuracion.h"
#include"Mensajeria/mensajes.h"

typedef enum{

	LIBRE,
	OCUPADO

}estadoBloque;

//void mostrar_bloque(char* path_bloque);
void mostrar_bloques(t_list* bloques);
void cargar_a_particion(char* path_particion, dato_t* dato_escribir, int bloque);
int tamanio_del_bloque( int bloque );
char* llenar_bloque(int bloque , char* dato);
void set_estado(int indiceBloque, estadoBloque estado);
bool get_estado(int indice);
int buscar_primer_indice_vacio(void);
char* convertir_dato_en_string( dato_t* dato );
void mostrar_bloque(int bloque);



#endif /* PROPIEDADES_FUNCIONAMIENTO_BLOQUE_H_ */
