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
	estado_request estado;

	int size;
	char* tabla_recibida;
	char* consistencia_recibida;
	int particiones_recibidas, compactacion_recibida;

	error_recv = recv(conexion, &estado , sizeof(estado_request), 0);

	if(estado == ERROR){

		return NULL;

	}

	error_recv = recv(conexion, &numero_tablas, sizeof(int), MSG_WAITALL);

	t_list* datos_metadata = list_create();

	if(error_recv <= 0){
		perror("NO SE RECIBIO LA CANTIDAD DE TABLAS DESCRIBE");

		return datos_metadata;

	}

	for(int i = 0; i < numero_tablas; i++){

		Metadata metadata_recibida;

		error_recv = recv(conexion, &size, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL SIZE DE LA TABLA");
		}

		tabla_recibida = malloc(size);

		error_recv = recv(conexion, tabla_recibida, size, 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO LA TABLA");
		}

		error_recv = recv(conexion, &size, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL SIZE DE LA CONSISTENCIA");
		}

		consistencia_recibida = malloc(size);

		error_recv = recv(conexion, consistencia_recibida, size, 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO LA CONSISTENCIA");
		}

		error_recv = recv(conexion, &particiones_recibidas, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL NUMERO DE PARTICIONES");
		}

		error_recv = recv(conexion, &compactacion_recibida, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL TIEMPO DE COMPACTACION");
		}

		metadata_recibida = crear_metadata(tabla_recibida, consistencia_recibida, particiones_recibidas, compactacion_recibida);

		list_add(datos_metadata, metadata_recibida);

		free(tabla_recibida);
		free(consistencia_recibida);

	}

	if( numero_tablas == 0 ){

		printf("\n--No Hay Tablas--\n");

	}

	return datos_metadata;

}

t_dato* recibir_dato_memoria(int conexion){


	estado_request estado;

	printf("AHORA EL RECV\n");

	recv(conexion, &estado , sizeof(estado_request) , MSG_WAITALL);

	printf("PASE EL RECV\n");

	if(estado == SUCCESS){

		printf("sALIO BIEN\n");

		t_dato* dato_recibido = malloc(sizeof(t_dato));

		dato_recibido->value = malloc(sizeof(t_stream));

		printf("voy a recibir\n");

		int bytes = recv(conexion,&(dato_recibido->timestamp),sizeof(time_t), 0);

		if(bytes == -1){
			perror("NO RECIBIO EL TIMESTAMP;");
		}

		printf("Timestamp: %i\n", dato_recibido->timestamp);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bytes = recv(conexion,&(dato_recibido->key),sizeof(u_int16_t), 0);

		if(bytes == -1){
				perror("NO RECIBIO LA KEY;");
		}

		printf("Key: %i\n", dato_recibido->key);

		///////////////////////////////////////////////////////////////////////////////////
		bytes = recv(conexion, &(dato_recibido->value->size),sizeof(int), 0);

		if(bytes == -1){
			perror("NO RECIBIO EL TAMANIO DEL VALUE;");
		}

		printf("Size: %i\n", dato_recibido->value->size);

		//////////////////////////////////////////////////////////////////////////////

		dato_recibido->value->buffer = malloc(dato_recibido->value->size);

		bytes = recv(conexion, dato_recibido->value->buffer, dato_recibido->value->size, 0);

		if(bytes == -1){
			perror("NO RECIBIO EL VALUE;");
		}

		///////////////////////////////////////////////////////////////////////////////////

		return dato_recibido;

	}else{

		printf("SALIO MAL\n");

		return NULL;

	}
}

estado_request recibir_estado_request(int conexion){

	estado_request estado;

	recv(conexion , &estado , sizeof(estado_request) , 0);

	return estado;

}


