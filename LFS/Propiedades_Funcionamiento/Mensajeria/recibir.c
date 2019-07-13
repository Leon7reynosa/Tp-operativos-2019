/*
 * recibir.c
 *
 *  Created on: 19 abr. 2019
 *      Author: utnso
 */

#include"recibir.h"


request recibir_request(int conexion){

	cod_operacion* cod_op = malloc(sizeof(cod_operacion));
	void* tipo_request;
	int pene = 0;
	pene = recv(conexion, cod_op,sizeof(cod_operacion),MSG_WAITALL);

	if(pene == -1){
			perror("Fallo al recibir el codigo de operacion.");
			//aca deberiamos sacar a la memoria de la lista?
			//aca deberiamos terminar el hilo?
		}

	printf("codigo_op : %d\n" , *cod_op);
	printf("pene: %d\n",pene);

	switch(*cod_op){

		case SELECT:

			tipo_request = decodificar_select(conexion);
			break;

		case INSERT:

			tipo_request = decodificar_insert(conexion);
			break;

		case CREATE:

			tipo_request = decodificar_create(conexion);
			break;

		default:

			break;

	}

	request request = crear_request(*cod_op, tipo_request);

	return request;

}


///////////////// FUNCIONES VIEJAS ///////////////////////////////7


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


	printf("\n\n\n");

	if(recv(conexion,&(datos->pedido),sizeof(int),0) == -1){
		perror("Fallo al recibir la solicitud.");
	}

	printf("\neste numero es: %d\n" , datos->pedido);

	if(recv(conexion,&(datos->size_tabla),sizeof(int),0) == -1){
		perror("Fallo al recibir el tamanio.");
	}

	printf("el tamaño de la tabla es: %d\n", datos->size_tabla);

	void* nombre_tabla = malloc(datos->size_tabla);

	if(recv(conexion,nombre_tabla,datos->size_tabla,0) == -1){
		perror("Fallo al recibir el mensaje");
	}

	printf("el nombre de la tabla es= %s\n" , nombre_tabla);

	datos->nombre_tabla = malloc(datos->size_tabla);
	memcpy(datos->nombre_tabla , nombre_tabla , datos->size_tabla);

	if(recv(conexion,&(datos->key),sizeof(int),0) == -1){
		perror("Fallo al recibir la key.");
	}

	printf("este key es: %i\n" , datos->key);

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



