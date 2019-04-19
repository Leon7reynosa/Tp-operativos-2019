/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"
#include "LissandraAPI.h"

int main(void){

	t_log* serverLogger = iniciar_log_server();

	//t_config* lissandraConfig; //Todavia sin usar

	void iterator(char* value)	//Esta función todavia no esta en uso
	{
		printf("%s\n", value);
	}

	int server_fd = iniciar_servidor();


		while(1)
		{

			log_info(serverLogger, "Servidor listo para recibir nuevo cliente");
			int cliente_fd = esperar_cliente(server_fd);

			if(!fork()){

				log_info(serverLogger, "Soy el proceso hijo, voy a recibir y mandar un mensaje");

				close(server_fd);

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
					return EXIT_FAILURE;
				//cliente_fd = esperar_cliente(server_fd);
					break;
				default:
					log_info(serverLogger,"Paso algo raro.");
					break;
				}

				char* mensaje = "Listo!";

				int tamanio = strlen(mensaje);
				log_info(serverLogger , "%d" , tamanio);
				log_info(serverLogger , "%s" , mensaje);


				send(cliente_fd,mensaje,strlen(mensaje) + 1,0);
				send(cliente_fd,"Chau nv",8,0);

				close(cliente_fd);
				printf("Termina el proceso hijo.");
				exit(0);

			}

			close(cliente_fd);

		}

	return EXIT_SUCCESS;

}

t_log* iniciar_log_server(void){
	  		return log_create("server.log", "Servidor", 1, LOG_LEVEL_DEBUG);
}
