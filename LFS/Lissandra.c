/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"
#include "LissandraAPI.h"

int main(void){

	t_log* serverLogger;

	t_config* lissandraConfig;

	void iterator(char* value)
	{
		printf("%s\n", value);
	}

	serverLogger = log_create("server.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(serverLogger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);


		//while(1)
		//{
			//por protocolo voy a tener un cod_op que me indica que es.
			int cod_op = recibir_operacion(cliente_fd);

			switch(cod_op){

			//recibo el mensaje normal si el cod_op es 0
			case MENSAJE:
				recibir_mensaje(cliente_fd);
				break;

			//si es -1 lo desconecto (convencion nuestra)
			case -1:
				log_error(serverLogger,"el cliente se desconecto.");
				return EXIT_SUCCESS;
				//cliente_fd = esperar_cliente(server_fd);
				break;
			default:
				log_info(serverLogger,"Paso algo raro.");
				break;
			}
			close(cliente_fd);
			close(server_fd);
			return EXIT_SUCCESS;
			//}

	/*
	//lissandraLogger = iniciar_log();

	//lissandraConfig = leer_config();

	//config_remove_key(lissandraConfig,"TRAP ");
	//config_set_value(lissandraConfig,"TRAP","Soy un valor");
	if(config_save(lissandraConfig) == -1){
		exit(2);
	}

	char* valor;

	valor = config_get_string_value(lissandraConfig , "TRAP");

	log_info(lissandraLogger,valor);

	/*
	leer_consola(lissandraLogger);

	conexion = crear_conexion(config_get_string_value(lissandraConfig, "IP"),
								  config_get_string_value(lissandraConfig , "PUERTO"));

	enviar_mensaje(valor, conexion);



	//liberar_conexion(conexion);
	log_destroy(lissandraLogger);
	config_destroy(lissandraConfig);

	liberar_conexion(server_fd);
	*/
	return EXIT_SUCCESS;

}

