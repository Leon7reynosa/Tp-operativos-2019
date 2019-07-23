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

		free(leido);

		leido = readline(">>");

		if ( string_equals_ignore_case(leido, "EXIT") ){

			continue;

		}

		t_scripts* nuevo_script = malloc(sizeof(t_scripts));

		nuevo_script->cola_requests = queue_create();

		nuevo_script->path_lql = "consola";

		queue_push(nuevo_script->cola_requests , (void*) leido);

		queue_push(cola_ready , (void*) nuevo_script);

		sem_post(&semaforo_ready);

	}

	free(leido);

	return NULL;
}


void menu(){

	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>CONSOLA<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	printf("\n1. SELECT [NOMBRE TABLA] [KEY]\n");
	printf("2. INSERT [NOMBRE TABLA] [KEY] \"[VALUE]\" [TIMESTAMP]\n");
	printf("3. CREATE [NOMBRE TABLA] [CONSISTENCIA] [NUMERO PARTICIONES] [TIEMPO COMPACTACION]\n");
	printf("4. ADD MEMORY [NUMERO MEMORIA] TO [CRITERIO]\n");
	printf("5. RUN [ARCHIVO LQL]\n\n");
	printf("Escriba una request: ");
	printf("\n");
}



