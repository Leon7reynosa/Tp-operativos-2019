/*
 * file_system_aux.h
 *
 *  Created on: 7 jul. 2019
 *      Author: utnso
 */

#ifndef FILE_SYSTEM_AUX_H_
#define FILE_SYSTEM_AUX_H_

#include<stdio.h>
#include<stdlib.h>
#include"comunity_func.h"
#include"configuracion.h"
#include"Lissandra.h"
#include"bloque.h"
#include"particion_contenido.h"


int existe_la_tabla(char* tabla);
void crear_directorio(char* path);
void crear_archivos_particiones(char* nombre_tabla, int numero_particiones);
void crear_archivo_particion(char* path);
void mostrar_tabla_y_particiones( char* nombre_tabla );
bool no_es_ubicacion_prohibida(char* path);
char* extension_del_archivo(char* path);
void realizar_dump();
void* ciclo_dump(void* argumentos);
#endif /* FILE_SYSTEM_AUX_H_ */
