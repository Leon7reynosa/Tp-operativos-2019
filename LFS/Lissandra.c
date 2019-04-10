/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"

int main(void){

	//int conexion;

	t_log* lissandraLogger;

	t_config* lissandraConfig;

	lissandraLogger = iniciar_log();

	lissandraConfig = leer_config();

	config_remove_key(lissandraConfig,"TRAP ");
	//config_set_value(lissandraConfig,"TRAP","Soy un valor");
	if(config_save(lissandraConfig) == -1){
		exit(2);
	}
	char* valor;

	valor = config_get_string_value(lissandraConfig , "TRAP");

	log_info(lissandraLogger,valor);


	leer_consola(lissandraLogger);

	//conexion = crear_conexion(config_get_string_value(lissandraConfig, "IP"),
	//							  config_get_string_value(lissandraConfig , "PUERTO"));

	//enviar_mensaje(valor, conexion);



	//liberar_conexion(conexion);
	log_destroy(lissandraLogger);
	config_destroy(lissandraConfig);

	return EXIT_SUCCESS;
}


t_log* iniciar_log(){

	char* lisandra_ruta = "LFS.log";

	return log_create(lisandra_ruta , "Lissandrovichoide de rucula" , 1 , LOG_LEVEL_INFO);
}

void leer_consola(t_log* logger)
{
	char* leido;

	leido = readline(">");


	while(*leido != '\0') {

		log_info(logger, leido);
		free(leido);
		leido = readline(">");
	}

	free (leido);

}


t_config* leer_config(){
	return  config_create("LFS/Lissandra.config");

}

