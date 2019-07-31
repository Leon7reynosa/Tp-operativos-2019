/*
 * cliente.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "cliente.h"

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

	if(error_conexion == -1){
//		perror("Fallo al conectar con el Servidor");

		return error_conexion;
	}

	int size;
	char* buffer = malloc(27);

	if((size = recv(socket_conexion,buffer,26,0)) == -1){
		perror("No llego la notificacion");
		exit(1);
	}

	buffer[size] = '\0';
	printf("%s",buffer);
	free(buffer);

	return socket_conexion;
}
