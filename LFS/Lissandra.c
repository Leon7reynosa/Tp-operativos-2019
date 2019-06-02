/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"


int main(void){

	dato_memtable = list_create();

	dato_t* dato_prueba = crear_dato(7 , "Leon" , 200);
	dato_t* dato_prueba_2  = crear_dato(8, "lalo" , 300);

	ingresar_a_memtable(dato_prueba, "Tabla_A");

	ingresar_a_memtable(dato_prueba_2, "Tabla_A");

	printf("prueba final \n");



	return EXIT_SUCCESS;

}



