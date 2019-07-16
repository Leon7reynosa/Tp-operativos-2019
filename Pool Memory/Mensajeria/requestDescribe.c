/*
 * requestDescribe.c
 *
 *  Created on: 30 jun. 2019
 *      Author: utnso
 */

#include"requestDescribe.h"

void* serializar_metadata(t_list* lista_metadata, int* bytes_a_enviar){

	int bytes = 0;
	int desplazamiento = 0;

	int size_tabla, size_consistencia, cantidad_tablas;

	void* buffer = malloc(sizeof(int)); //inicialmente con la cantidad de tablas;

	//le mando la cantidad de tablas primero
	cantidad_tablas = list_size(lista_metadata);

	memcpy(buffer + desplazamiento, &cantidad_tablas, sizeof(int));
	desplazamiento += sizeof(int);

	bytes += sizeof(int);

	void _serializar_metadata(void* _metadata){

		Metadata metadata = (Metadata)_metadata;

		bytes += sizeof(int) + metadata->tabla->size + sizeof(int) + metadata->consistencia->size + sizeof(int)*2 ;

		buffer = realloc(buffer, bytes);

		memcpy(buffer + desplazamiento, &(metadata->tabla->size), sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(buffer + desplazamiento, metadata->tabla->buffer, metadata->tabla->size);
		desplazamiento += metadata->tabla->size;

		memcpy(buffer + desplazamiento, &(metadata->consistencia->size), sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(buffer + desplazamiento, metadata->consistencia->buffer, metadata->consistencia->size);
		desplazamiento += metadata->consistencia->size;

		memcpy(buffer + desplazamiento, &(metadata->particiones), sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(buffer + desplazamiento, &(metadata->tiempo_compactacion), sizeof(int));
		desplazamiento += sizeof(int);
	}

	list_iterate(lista_metadata, _serializar_metadata);

	*bytes_a_enviar = bytes;

	return buffer;
}

Metadata crear_metadata(char* tabla, char* consistencia, int particiones, int compactacion){

	Metadata dato_metadata = malloc(sizeof(struct metadataEstructura));

	dato_metadata->tabla = malloc(sizeof(t_stream));
	dato_metadata->tabla->size = string_length(tabla) + 1;
	dato_metadata->tabla->buffer = malloc(dato_metadata->tabla->size);
	memcpy(dato_metadata->tabla->buffer, tabla, dato_metadata->tabla->size);

	dato_metadata->consistencia = malloc(sizeof(t_stream));
	dato_metadata->consistencia->size = string_length(consistencia) + 1;
	dato_metadata->consistencia->buffer = malloc(dato_metadata->consistencia->size);
	memcpy(dato_metadata->consistencia->buffer, consistencia, dato_metadata->consistencia->size);

	dato_metadata->particiones = particiones;
	dato_metadata->tiempo_compactacion = compactacion;

	return dato_metadata;

}

void liberar_metadata(Metadata metadata_a_liberar){

	free(metadata_a_liberar->tabla->buffer);
	free(metadata_a_liberar->tabla);

	free(metadata_a_liberar->consistencia->buffer);
	free(metadata_a_liberar->consistencia);

	free(metadata_a_liberar);
}


describe_t decodificar_describe(int conexion){

	int error_recv;
	int* size_tabla = malloc(sizeof(int));
	bool global;
	char* tabla;


	error_recv = recv(conexion, &global ,sizeof(bool),0);

	if(error_recv == -1){
		perror("Fallo al recibir si es global o no.\n");
	}

	printf("Es global?: %i\n", global);

	if(global){

		printf("perfecto, es global\n");
		tabla = NULL;

	}else{

		printf("perfecto, no es global\n");

		error_recv = recv(conexion, size_tabla ,sizeof(int),0);

		if(error_recv == -1){
			perror("Fallo al recibir el tamaño de la tabla.\n");
		}

		printf("size de tabla: %i\n", *size_tabla);

		tabla = malloc(*size_tabla);

		if(recv(conexion,tabla , *size_tabla,0) == -1){
			perror("Fallo al recibir la tabla.\n");
		}

		printf("Tabla recibida: %s\n", tabla);

	}

	printf("Creo el dato describe\n");
	describe_t dato = crear_dato_describe(tabla);

	free(size_tabla);

	if(!global){
		free(tabla);
	}

	return dato;
}


void* serializar_describe(request request_describe){

	describe_t dato_describe = (describe_t) (request_describe->tipo_request);

	void* buffer_serializado = malloc(dato_describe->bytes);
	int desplazamiento = 0;

	memcpy(buffer_serializado + desplazamiento, &(request_describe->cod_op) , sizeof(request_describe->cod_op) );
	desplazamiento += sizeof(request_describe->cod_op);

	memcpy(buffer_serializado + desplazamiento, &(dato_describe->global), sizeof(dato_describe->global));
	desplazamiento += sizeof(dato_describe->global);

	if(!dato_describe->global){

		memcpy(buffer_serializado + desplazamiento, &(dato_describe->tabla->size) , sizeof(dato_describe->tabla->size) );
		desplazamiento += sizeof(dato_describe->tabla->size);

		memcpy(buffer_serializado + desplazamiento, dato_describe->tabla->buffer, dato_describe->tabla->size);
		desplazamiento += dato_describe->tabla->size;
	}

	return buffer_serializado;
}


describe_t crear_dato_describe(char* nombre_tabla){
	describe_t dato_describe = malloc(sizeof(struct describeEstructura));

	if(nombre_tabla != NULL){

		dato_describe->global = false;

		dato_describe->tabla = malloc(sizeof(t_stream));

		dato_describe->tabla->size = string_length(nombre_tabla) + 1;

		dato_describe->tabla->buffer = malloc(dato_describe->tabla->size);

		memcpy(dato_describe->tabla->buffer , nombre_tabla , dato_describe->tabla->size);

		dato_describe->bytes = sizeof(cod_operacion) + sizeof(bool) + sizeof(dato_describe->tabla->size) + dato_describe->tabla->size;

		printf("bytes a enviar: %i\n",dato_describe->bytes);

	}
	else{
		dato_describe->global = true;
		dato_describe->tabla = NULL;
		dato_describe->bytes = sizeof(cod_operacion) + sizeof(bool);

	}
	return dato_describe;
}

void liberar_dato_describe(describe_t dato){

	if(dato->tabla != NULL){
		free(dato->tabla->buffer);
		free(dato->tabla);
	}

	free(dato);
}
