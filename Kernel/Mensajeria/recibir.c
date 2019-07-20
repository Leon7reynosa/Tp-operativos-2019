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

void desconectar_cliente(int conexion){

	close(conexion);
	printf("Cliente %d Desconectado\n", conexion);

}



t_list* recibir_describe(int conexion){

	int numero_tablas;
	int error_recv;

	int size;
	char* tabla_recibida;
	char* consistencia_recibida;
	int particiones_recibidas, compactacion_recibida;

	printf("estoy esperando respuesta\n");


	error_recv = recv(conexion, &numero_tablas, sizeof(int), MSG_WAITALL);

	printf("ya recibi respuesta\n");

	t_list* datos_metadata = list_create();

	if(error_recv == -1){
		perror("NO SE RECIBIO LA CANTIDAD DE TABLAS DESCRIBE");
	}

	printf("Tablas a recibir: %i\n", numero_tablas);

	for(int i = 0; i < numero_tablas; i++){

		printf("Empiezo a recibir la %i° tabla\n", i);

		Metadata metadata_recibida;

		error_recv = recv(conexion, &size, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL SIZE DE LA TABLA");
		}

		printf("Size de tabla: %i\n", size);

		tabla_recibida = malloc(size);

		error_recv = recv(conexion, tabla_recibida, size, 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO LA TABLA");
		}

		printf("Tabla: %s\n", tabla_recibida);

		error_recv = recv(conexion, &size, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL SIZE DE LA CONSISTENCIA");
		}

		printf("Size de Consistencia: %i\n", size);

		consistencia_recibida = malloc(size);

		error_recv = recv(conexion, consistencia_recibida, size, 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO LA CONSISTENCIA");
		}

		printf("Consistencia: %s\n", consistencia_recibida);

		error_recv = recv(conexion, &particiones_recibidas, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL NUMERO DE PARTICIONES");
		}

		printf("Particiones: %i\n", particiones_recibidas);

		error_recv = recv(conexion, &compactacion_recibida, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL TIEMPO DE COMPACTACION");
		}

		printf("Compactacion: %i\n", compactacion_recibida);

		printf("Creo la estructura metadata\n");
		metadata_recibida = crear_metadata(tabla_recibida, consistencia_recibida, particiones_recibidas, compactacion_recibida);
		printf("Cree la estructura metadata y la meto en la lista\n");

		list_add(datos_metadata, metadata_recibida);

		printf("La meti en la lista\n");

		free(tabla_recibida);
		free(consistencia_recibida);

	}

	printf("Termine de recibir las tablas\n");

	return datos_metadata;

}


