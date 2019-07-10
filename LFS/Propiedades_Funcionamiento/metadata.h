/*
 * metadata.h
 *
 *  Created on: 19 may. 2019
 *      Author: utnso
 */

#ifndef METADATA_METADATA_H_
#define METADATA_METADATA_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<time.h>
#include<commons/config.h>
#include<commons/string.h>
#include"configuracion.h"
#include<string.h>
#include<sys/stat.h>

typedef enum criterio{
	SC, //Strong Consistency
	SHC, // Strong Hash Consistency
	EC //Eventual Consistency
}criterio_t;


typedef struct{
	int particion;
	int compactacion;
	criterio_t consistencia;
}metadata_t;




void setear_metadata(void);
metadata_t* obtener_metadata(char*);
void crear_metadata(char* nombre_tabla, char* consistencia, int particion, int tiempo_Compactacion);
char* obtener_path_metadata_de_tabla(char* nombre_Tabla);
void mostrar_metadata_de_tabla(char*);
char* obtenerPathTabla(char* nombre_Tabla);
char* obtenerPath_ParticionTabla(char* nombre_tabla, int particion);
char* obtenerPath_Bloque(int indice);
char* obtenerPathDirectorio_Tablas(void);
char* obtenerPathParaTemporalEnLaTabla(char* nombreTabla);
void transformar_tmp_a_tmpc(char* nombre_tabla);
int obtener_cantidad_de_archivos_tmpc(char* nombre_tabla);
#endif /* METADATA_METADATA_H_ */
