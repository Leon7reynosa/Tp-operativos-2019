/*
 * consola.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include"consola.h"

void* consola(void* argumento){

	char* leido = string_new();

	menu();

	while(!string_equals_ignore_case(leido, "exit")){
		printf("Entre al while\n");
		free(leido);

		leido = readline("");

		printf("solicitud : %s\n" , leido);
	}

	free(leido);

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

