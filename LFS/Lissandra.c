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
	dato_t* dato_prueba_3  = crear_dato(1, "chino" , 3030);


	//si lo dejo asi, cuando ingreso el ultimo tabla a, en la index 0 de la memtable, tiene tabla_B
	ingresar_a_memtable(dato_prueba, "Tabla_A");

	ingresar_a_memtable(dato_prueba_3, "Tabla_B");

	ingresar_a_memtable(dato_prueba_2, "Tabla_A");


	printf("prueba final \n");



	return EXIT_SUCCESS;

}



