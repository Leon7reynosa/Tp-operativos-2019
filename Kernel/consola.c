/*
 * consola.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include"consola.h"

void consola(){

	printf("holaas\n");

	char* leido = " ";


	while(strcmp(leido, "exit") || string_is_empty(leido)){

		printf("Seleccione una operacion:\n");
		printf("1. SELECT [NOMBRE TABLA] [KEY]\n");
		printf("2. INSERT [NOMBRE TABLA] [KEY] [VALUE] [TIMESTAMP]\n");
		printf("3. CREATE [NOMBRE TABLA] [CONSISTENCIA] [NUMERO PARTICIONES] [TIEMPO COMPACTACION]\n");
		printf("2. DESCRIBE [NOMBRE TABLA]\n");

		printf("\nIngrese una request: ");

		leido = readline(">>");

		mandar_request(leido);

		printf("\n#######################################################\n");



	}



}
