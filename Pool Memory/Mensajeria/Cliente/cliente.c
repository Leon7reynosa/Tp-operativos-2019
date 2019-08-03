/*
 * cliente.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "cliente.h"

void desconectar_lissandra(void){

	conexion_lissandra = false;

	close(socket_lissandra);

	log_info(logger_estado, "Se produjo una desconexion con lissandra");

}

bool comprobar_conexion_lissandra(char* ip_lissandra, int puerto_lissandra){

	int value_aux;

	if(!conexion_lissandra){

		log_info(logger_estado, "No se posee una conexion con lissandra, nos intentamos reconectar");

		socket_lissandra = conectar_servidor(ip_lissandra, puerto_lissandra);

		if(socket_lissandra <= -1){

			log_info(logger_estado, "No se pudo establecer la reconexion" );

			conexion_lissandra = false;

			return false;

		}else{

			recv(socket_lissandra, &value_aux, sizeof(int), 0);

			conexion_lissandra = true;

			log_info(logger_estado, "Se reestablecio la conexion");

		}

	}else{
		//esta conectada
	}

	return true;


}

int conectar_servidor(char* ip, int puerto){

	int error_conexion;

	int socket_conexion;
	struct sockaddr_in their_addr;
	int size_addr = sizeof(struct sockaddr);

	if((socket_conexion = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("Fallo al crear el socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(puerto);
	inet_aton(ip,&(their_addr.sin_addr));
	bzero(&(their_addr.sin_zero),8);

	error_conexion = connect(socket_conexion,(struct sockaddr*)&their_addr,size_addr);

	if(error_conexion <= -1){

		perror("Fallo al conectar con el Servidor");

		close(socket_conexion);

		return error_conexion;
	}

	int size;
	char* buffer = malloc(27);

	if((size = recv(socket_conexion,buffer,26,0)) == -1){
		perror("No llego la notificacion");

		close(socket_conexion);

		exit(1);
	}


	buffer[size] = '\0';
//	printf("%s",buffer);
	free(buffer);

	return socket_conexion;
}
