/*
 * consola.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include"consola.h"

void* consola(){

	char* leido = " ";


	while(strcmp(leido, "exit") || string_is_empty(leido)){

		menu();

		leido = readline(">>");

		t_scripts* nuevo_script = malloc(sizeof(t_scripts));

		nuevo_script->path_lql = "consola";

		queue_push(nuevo_script->cola_requests , leido);

		queue_push(cola_ready , (void*) nuevo_script);

		mandar_request(leido);



	}


	return NULL;
}


void menu(){

	printf("Seleccione una operacion:\n");
	printf("1. SELECT [NOMBRE TABLA] [KEY]\n");
	printf("2. INSERT [NOMBRE TABLA] [KEY] [VALUE] [TIMESTAMP]\n");
	printf("3. CREATE [NOMBRE TABLA] [CONSISTENCIA] [NUMERO PARTICIONES] [TIEMPO COMPACTACION]\n");
	printf("4. ADD MEMORY [NUMERO MEMORIA] TO [CRITERIO]\n");

	printf("\nEscriba una request: ");

}

