/*
 * requestDescribe.c
 *
 *  Created on: 30 jun. 2019
 *      Author: utnso
 */

#include"requestDescribe.h"

Metadata crear_metadata(char* tabla, char* consistencia, int particiones, int compactacion){

	Metadata dato_metadata = malloc(sizeof(struct metadataEstructura));

	int size = string_length(tabla) + 1;

	dato_metadata->tabla = malloc(size);
	memcpy(dato_metadata->tabla, tabla, size);

	size = string_length(consistencia) + 1;

	dato_metadata->consistencia = malloc(size);
	memcpy(dato_metadata->consistencia, consistencia, size);

	dato_metadata->particiones = particiones;
	dato_metadata->tiempo_compactacion = compactacion;

	return dato_metadata;

}

void liberar_metadata(Metadata metadata_a_liberar){

	free(metadata_a_liberar->tabla);

	free(metadata_a_liberar->consistencia);

	free(metadata_a_liberar);
}

void* serializar_describe(request request_describe){

	describe_t dato_describe = (describe_t) (request_describe->tipo_request);

	void* buffer_serializado = malloc(dato_describe->bytes);
	int desplazamiento = 0;

	memcpy(buffer_serializado + desplazamiento, &(request_describe->cod_op) , sizeof(request_describe->cod_op) );
	desplazamiento += sizeof(request_describe->cod_op);

	memcpy( buffer_serializado + desplazamiento , &(dato_describe->global) , sizeof(dato_describe->global) );
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

	}
	else{
		dato_describe->global = true;
		dato_describe->tabla = NULL;
		dato_describe->bytes = sizeof(cod_operacion) + sizeof(bool);

	}
	return dato_describe;
}

void liberar_dato_describe(describe_t dato){

	if(!dato->global){
		free(dato->tabla->buffer);
		free(dato->tabla);
	}
	free(dato);
}


void mostrar_lista_describe(t_list* lista_describe){

	if(list_size(lista_describe) == 0){

		printf("\n>No se recibieron TABLAS\n");


	}

	void _mostrar_metadata(void* dato_metadata){

		Metadata dato_describe = (Metadata) dato_metadata;

		printf("\n---TABLA: %s ---\n", dato_describe->tabla);

		printf("consistencia: %s\n" , dato_describe->consistencia);

		printf("particiones: %d\n" , dato_describe->particiones);

		printf("tiempo compactacion: %d\n" , dato_describe->tiempo_compactacion);

	}

	list_iterate(lista_describe, _mostrar_metadata);

	printf("\n");

}



