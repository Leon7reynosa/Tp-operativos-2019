/*
 * recibir.c
 *
 *  Created on: 1 may. 2019
 *      Author: utnso
 */


#include"recibir.h"

void* recibir_buffer(int* size, int conexion){

	void* buffer;

	if(recv(conexion,size,sizeof(int),MSG_WAITALL) == -1){
		perror("Fallo al recibir el tamanio.");
	}

	buffer = malloc(*size);

	if(recv(conexion,buffer,*size,MSG_WAITALL) == -1){
		perror("Fallo al recibir el mensaje");
	}

	return buffer;

}

cod_op determinar_operacion(char* buffer){

	int size = strlen(buffer);

	char* aux = malloc(size+1);
	memcpy(aux,buffer,size);
	aux[size + 1] = '\0';
	memcpy(aux,buffer,size);

	int i;

	for(i = 0; i<size; i++){
		aux [i] = toupper(aux[i]);
	}

	switch(strcmp(aux,"EXIT")){

		case 0:
			free(aux);
			return DESCONEXION;
			break;
		default:
			free(aux);
			return MENSAJE;
	}

}

void desconectar_cliente(int conexion){

	close(conexion);
	printf("Cliente %d Desconectado\n", conexion);

}


void recibir_mensaje(int conexion, fd_set* master){

	int size;
	char* buffer;

	buffer = recibir_buffer(&size,conexion);

	buffer[size] = '\0';

	switch(determinar_operacion(buffer)){

		case MENSAJE:
			printf("[Cliente %d] Mensaje : %s \n",conexion,buffer);
			break;
		case DESCONEXION:
			desconectar_cliente(conexion);
			FD_CLR(conexion,master);
			break;
		default:
			printf("No deberia haber entrado aca por ahora\n\n");
			exit(1);
	}

	free(buffer);

}
