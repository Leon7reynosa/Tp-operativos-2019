/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"

int main(void){

	iniciar_log();

	leer_consola();



	return EXIT_SUCCESS;
}


void iniciar_log(){

	char* lisandra_ruta = "LFS.log";

	lissandraLogger = log_create(lisandra_ruta , "Lissandrovichoide de rucula" , 1 , LOG_LEVEL_INFO);
}

void leer_consola()
{
	char* leido;

	leido = readline(">");


	while(*leido != '\0') {

		log_info(lissandraLogger, leido);
		free(leido);
		leido = readline(">");
	}

	free (leido);

}
