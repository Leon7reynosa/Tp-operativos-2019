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

operacion_select* recibir_solicitud(int conexion){

	operacion_select* datos = malloc(sizeof(operacion_select));
	int solicitud;
	int size_tabla;
	char* nombre_tabla;
	int key;

	if(recv(conexion,solicitud,sizeof(int),0) == -1){
		perror("Fallo al recibir la solicitud.");
	}

	datos->pedido = solicitud;

	if(recv(conexion,size_tabla,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamanio.");
	}

	datos->size_tabla = size_tabla;
	nombre_tabla = malloc(size_tabla);

	if(recv(conexion,nombre_tabla,size_tabla,0) == -1){
		perror("Fallo al recibir el mensaje");
	}

	datos->nombre_tabla = malloc(datos->size_tabla);
	memcpy(datos->nombre_tabla , nombre_tabla , datos->size_tabla);

	if(recv(conexion,key,sizeof(int),0) == -1){
		perror("Fallo al recibir la key.");
	}

	datos->key = key;

	return datos;
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



