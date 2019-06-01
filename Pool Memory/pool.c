/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"

int main (void){

	tamanio_memoria = 2048;
	tamanio_dato_memoria = sizeof(dato_en_memoria);
	max_value = 50;

	inicializar_memoria();

	test1();
	test2();

	return EXIT_SUCCESS;
}
