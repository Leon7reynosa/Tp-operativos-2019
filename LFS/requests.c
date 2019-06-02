/*
 * api_memory.c
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#include "requests.h"

 void request_select(char* nombre_tabla , int key){

	 dato_t* dato_mas_reciente;

	 dato_t* dato_auxiliar;

	 char* path_tabla;

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Existe la tabla en el File System\n");

		 metadata_t metadata_tabla = obtener_metadata(nombre_tabla);

		 int particion_objetivo = calcular_particion(metadata_tabla.particion , key);

		 path_tabla = obtenerPath_ParticionTabla(nombre_tabla, particion_objetivo);


	 }else{

		 printf("No existe la tabla en el File System\n");

	 }

 }


 void request_insert(char* nombre_tabla, int key, char* valor, time_t timestamp){

	 dato_t* dato_ingresar;
	 metadata_t metadata_insert;

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Existe la tabla en el File System\n");

		 metadata_insert = obtener_metadata(nombre_tabla);

		 dato_ingresar = crear_dato(key, valor, timestamp);

		 ingresar_a_memtable(dato_ingresar, nombre_tabla);

	 }else{

		 printf("Fallo el insert. \n");

	 }

 }


 void request_create(char* nombre_tabla, char* criterio, int numero_particiones, int tiempo_compactacion){

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Ya existe la tabla \n");
		 exit(1);

	 }

	 char* path_tabla = obtenerPathTabla(nombre_tabla);

	 mkdir(path_tabla, 0700);

	 crear_metadata(nombre_tabla, criterio, numero_particiones, tiempo_compactacion);

 }









