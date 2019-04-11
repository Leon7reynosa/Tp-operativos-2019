/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"

int main(void){

	t_log* lissandraLogger;

	t_config* lissandraConfig;

		void iterator(char* value)
		{
			printf("%s\n", value);
		}

		logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

		int server_fd = iniciar_servidor();
		log_info(logger, "Servidor listo para recibir al cliente");
		int cliente_fd = esperar_cliente(server_fd);

		t_list* lista;
		while(1)
		{
			/*char* buffer = malloc(12);

			if(recv(cliente_fd,buffer,11,0) != -1){
				log_info(logger,"me llego el mensaje: %s",buffer);
				free(buffer);
			}
			else if(recv(cliente_fd,buffer,11,0) == 0){
				log_info(logger,"SE DESCONECTO!");
				return EXIT_SUCCESS;
			}
			else{
				printf("no piolin");
				exit(1);
			}

			free (buffer);*/

			int cod_op = recibir_operacion(cliente_fd);
			switch(cod_op)
			{
			case MENSAJE:
				recibir_mensaje(cliente_fd);
				break;
			case PAQUETE:
				lista = recibir_paquete(cliente_fd);
				printf("Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				return EXIT_FAILURE;
			default:
				log_warning(logger, "Operacion desconocida. No quieras meter la pata");
				recibir_mensaje(cliente_fd);
				break;
			}
		}

	lissandraLogger = iniciar_log();

	lissandraConfig = leer_config();

	//config_remove_key(lissandraConfig,"TRAP ");
	//config_set_value(lissandraConfig,"TRAP","Soy un valor");
	/*if(config_save(lissandraConfig) == -1){
		exit(2);
	}*/

	char* valor;

	valor = config_get_string_value(lissandraConfig , "TRAP");

	log_info(lissandraLogger,valor);

	/*
	leer_consola(lissandraLogger);

	conexion = crear_conexion(config_get_string_value(lissandraConfig, "IP"),
								  config_get_string_value(lissandraConfig , "PUERTO"));

	enviar_mensaje(valor, conexion);
	*/


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

