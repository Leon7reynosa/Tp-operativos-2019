/*
 * Lissandra.h
 *
 *  Created on: 9 abr. 2019
 *      Author: utnso
 */

#ifndef LISSANDRA_H_
#define LISSANDRA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

//#include "utils.h"
#include "utilServer.h"

	//Por ahora pongo aca

	/*
	 * typedef struct{
	 * 		key (Tiene q ser uint 16)
	 * 		value
	 * 		timestamp
	 *
	 * 		} dato
	 *
	 */


	//aca tenemos la metadata en la cual se va a almacenar los "dato" para luego hacerlos temporales.
	// dato memtable[ 10 ] ;



		//obtiene el valor de una key dentro de una tabla

		//crea una nueva tabla dentro del file system
		void create( /* char *nombre_tabla , int *key */);
			//Por cada tabla creada, hay que hacer un metadata propio de ese (Esta en un issue)


		//ingresa o actualiza una key dentro de una tabla
		void insert( void /* char *nombre_tabla , int timestamp , int key , char *valor */);
			/*
			 * Ejm. por consola pasas
			 *
			 * insert(TablaEjm, 1000, 1, "Martin")
			 *
			 * 		se deberia llamar a la funcion "insertarRegistro(con los mismos parametros)" y crear el registro del tipo "dato" (definido en el struct)
			 * 		insertarRegistro() va a meter el "dato" en el array "memtable", luego de cierto tiempo se crea el archivo temporal con los datos de la memtable y se vacia.
			 */

		//permite obtener la metadata de una tabla
		void describe( void /* char *nombre_tabla */ );

		//elimina una tabla
		void drop( void /* char *nombre_tabla */);


t_log* iniciar_log(void);

void leer_consola(t_log*);

t_config* leer_config(void);

void terminar_programa(int);

void* recibir_buffer(int*, int);


#endif /* LISSANDRA_H_ */
