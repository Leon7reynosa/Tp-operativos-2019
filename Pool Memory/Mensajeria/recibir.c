/*
 * recibir.c
 *
 *  Created on: 19 abr. 2019
 *      Author: utnso
 */

#include"recibir.h"

void* recibir_buffer(int* size, int conexion){

	void* buffer;

	if(recv(conexion,size,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamanio.");
	}

	buffer = malloc(*size);

	if(recv(conexion,buffer,*size,0) == -1){
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


void recibir_mensaje(int conexion){

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
			break;
		default:
			printf("No deberia haber entrado aca por ahora\n\n");
			exit(1);
	}

	free(buffer);

}

void recibir_request_LFS(int conexion ){

	dato_t* dato_recibido = malloc(sizeof(dato_t));

	if(recv(conexion,&(dato_recibido->key),sizeof(int),0) == -1){
		perror("Fallo al recibir la solicitud.");
	}
	int size;
	//int* size = malloc(sizeof(int));

	if(recv(conexion,&size,sizeof(int),0) == -1){
		perror("Fallo al recibir la solicitud.");
	}

	printf("lololo - tamaño del value: %d\n",size);

	void* buffer = malloc(size);

	if(recv(conexion,buffer,size,0) == -1){
			perror("Fallo al recibir la solicitud.");
		}

	/*memcpy(buffer,dato_recibido->value,size);
	*((dato_recibido->value) + 1) = '\0';
	*/
	if(recv(conexion,&(dato_recibido->timestamp),sizeof(int),0) == -1){
				perror("Fallo al recibir la solicitud.");
	}
	printf("ESTO ES LO QUE LLEGO DEL LFS \n\n");
	printf("la key recibida es: %d\n", dato_recibido->key);
	printf("tamaño del value: %d\n",size);
	printf("tamaño del value: %d\n",&size);
	//printf("el value: %s\n",dato_recibido->value);
	printf("timestamp: %d\n",dato_recibido->timestamp);

}



