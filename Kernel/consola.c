/*
 * consola.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include"consola.h"

void* consola(){

	char* leido;

	menu();

	leido = readline("");

	while( !string_equals_ignore_case(leido, "EXIT") ){

		t_scripts* nuevo_script = malloc(sizeof(t_scripts));

		nuevo_script->cola_requests = queue_create();

		nuevo_script->path_lql = "consola";

		queue_push(nuevo_script->cola_requests , (void*) leido);

		queue_push(cola_ready , (void*) nuevo_script);

		sem_post(&semaforo_ready);

		leido = readline("");

	}

	printf("\n> Se realizo un EXIT el programa terminara\n");

	free(leido);

	return NULL;
}


void menu(){

	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>CONSOLA<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	printf("\n1. SELECT [NOMBRE TABLA] [KEY]\n");
	printf("2. INSERT [NOMBRE TABLA] [KEY] \"[VALUE]\" [TIMESTAMP]\n");
	printf("3. CREATE [NOMBRE TABLA] [CONSISTENCIA] [NUMERO PARTICIONES] [TIEMPO COMPACTACION]\n");
	printf("4. ADD MEMORY [NUMERO MEMORIA] TO [CRITERIO]\n");
	printf("5. RUN [ARCHIVO LQL]\n");
	printf("6. DROP [NOMBRE TABLA]\n");
	printf("7. DESCRIBE ([NOMBRE TABLA])\n");
	printf("8. JOURNAL\n");
	printf("9. METRICS\n");
	printf("10. EXIT\n");
	printf("Escriba una request: ");
	printf("\n");
}



