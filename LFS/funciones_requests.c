/*
 * funciones_requests.c
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#include "funciones_requests.h"

int existe_la_tabla(char* tabla){

 	char* path_tabla = obtenerPathTabla(tabla);

 	DIR* dir = opendir(path_tabla);

 	if(dir){

 		return 1;

 	}

 	closedir(dir);

 	return 0;
}


int calcular_particion( int particion_metadata ,int key){

	return key % particion_metadata;

}

dato_t* crear_dato(int clave, char* valor, time_t tiempo){

	dato_t* nuevo = (dato_t*) malloc(sizeof(dato_t));

	nuevo->key = clave;
	nuevo->value =  valor;
	nuevo->timestamp = tiempo;

	return nuevo;
}

//tenemos que hacer que la memtable siempre este inicializada.
void ingresar_a_memtable(dato_t* dato_a_ingresar, char* nombre_tabla){

	memoria_t* tabla_ingresar = malloc(sizeof(memoria_t));

	tabla_ingresar = obtener_memoria_tabla(nombre_tabla);

	list_add(tabla_ingresar->primer_elemento , dato_a_ingresar);

}


memoria_t* obtener_memoria_tabla(char* nombre_tabla){

	memoria_t* ubicacion_tabla = malloc(sizeof(memoria_t));

	bool encontrado =  false;

	printf("cantidad: %d\n" , dato_memtable->elements_count);

	for(int i = 0 ; i < dato_memtable->elements_count ; i++){

		printf("iteracion : %d\n", i);
		ubicacion_tabla = (memoria_t*) list_get(dato_memtable, i);


		printf("contiene: %s\n" ,ubicacion_tabla->nombre_tabla);

		if(strcmp(ubicacion_tabla->nombre_tabla, nombre_tabla) == 0){

			i = dato_memtable->elements_count;

			encontrado = true;

		}
	}

	//si no encuentra una lista con el nombre de la tabla, la crea.
	if(!encontrado){

		ubicacion_tabla->nombre_tabla = nombre_tabla;

		ubicacion_tabla->primer_elemento = list_create();

		list_add(dato_memtable, ubicacion_tabla);
	}

	return ubicacion_tabla;

}
