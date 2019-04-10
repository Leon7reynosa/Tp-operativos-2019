/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"

int main(void){

	int conexion;
	char* ip;
	char* puerto;

	iniciar_log();

	leer_config();

	leer_consola();

	ip = config_get_string_value(lissandraConfig, "PUERTO");
	puerto = config_get_string_value(lissandraConfig , "IP");

	crear_conexion(ip, puerto);

	char* valor;

	valor = config_get_string_value(lissandraConfig, "TRAP");

	enviar_mensaje(valor, conexion);

	terminar_programa(conexion);

	printf("hola chinito, by Giuli <3");

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


void leer_config(){
	char* valor;

	lissandraConfig = config_create("Lissandra.config");

	valor = config_get_string_value(lissandraConfig , "CLAVE");

	log_info(lissandraLogger, valor);
}

void terminar_programa(int conexion){
	liberar_conexion(conexion);
	log_destroy(lissandraLogger);
	config_destroy(lissandraConfig);
}

