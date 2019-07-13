/*
 * consola.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include"consola.h"

void* consola(){

	char* leido = " ";

	menu();

	while(strcmp(leido, "exit") || string_is_empty(leido)){


		leido = readline(">>");

		printf("solicitud : %S\n" , leido);
	}


	return NULL;
}


void menu(){

	printf("\n################################################################################\n");
	printf("\n1. SELECT [NOMBRE TABLA] [KEY]\n");
	printf("2. INSERT [NOMBRE TABLA] [KEY] [VALUE] [TIMESTAMP]\n");
	printf("3. CREATE [NOMBRE TABLA] [CONSISTENCIA] [NUMERO PARTICIONES] [TIEMPO COMPACTACION]\n");
	printf("4. ADD MEMORY [NUMERO MEMORIA] TO [CRITERIO]\n");
	printf("5. RUN [ARCHIVO LQL]\n");
	printf("Escriba una request: ");
	printf("\n");
}

