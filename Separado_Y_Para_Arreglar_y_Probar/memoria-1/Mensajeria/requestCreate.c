/*
 * requestCreate.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#include"requestCreate.h"
/*
struct createEstructura{
	int bytes;
	t_stream* tabla;
	t_stream* consistencia;
	int numero_particiones;
	time_t compactacion;

};
*/

create decodificar_create(int conexion){

	int* size_tabla = malloc(sizeof(int));
	int* size_consistencia = malloc(sizeof(int));
	int* numero_particiones = malloc(sizeof(int));
	time_t* compactacion = malloc(sizeof(time_t));

	if(recv(conexion,size_tabla ,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamaño de la tabla.\n");
	}

	printf("SIZE TABLA: %i\n", *size_tabla);

	char* tabla = malloc(*size_tabla);

	if(recv(conexion,tabla , *size_tabla,0) == -1){
		perror("Fallo al recibir la tabla.\n");
	}

	printf("TABLA: %s\n", tabla);

	if(recv(conexion, size_consistencia ,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamaño de la consistencia.\n");
	}

	printf("SIZE CONSISTENCIA: %i\n", *size_consistencia);

	char* consistencia = malloc(*size_consistencia);

	if(recv(conexion,consistencia ,*size_consistencia ,0) == -1){
		perror("Fallo al recibir la consistencia.\n");
	}

	printf("CONSISTENCIA: %s\n", consistencia);

	if(recv(conexion,numero_particiones ,sizeof(int),0) == -1){
		perror("Fallo al recibir los numeros de particiones.\n");
	}

	printf("NUMERO PARTICIONES: %i\n", *numero_particiones);

	if(recv(conexion,compactacion ,sizeof(time_t),0) == -1){
		perror("Fallo al recibir la compactación.\n");
	}

	printf("COMPACTACION: %i\n", *compactacion);

	string_to_upper(tabla);

	create dato = crear_dato_create(tabla, consistencia, *numero_particiones, *compactacion);

	free(tabla);
	free(consistencia);
	free(numero_particiones);
	free(compactacion);
	free(size_tabla);
	free(size_consistencia);

	return dato;
}

void* serializar_create(request request_create){

	create dato_create = (create)(request_create->tipo_request);
	void* buffer_serializado = malloc(dato_create->bytes);
	int desplazamiento = 0;

	memcpy(buffer_serializado + desplazamiento, &(request_create->cod_op), sizeof(request_create->cod_op) );
	desplazamiento +=  sizeof(request_create->cod_op);

	memcpy(buffer_serializado + desplazamiento, &(dato_create->tabla->size), sizeof(dato_create->tabla->size));
	desplazamiento +=  sizeof(dato_create->tabla->size);

	memcpy(buffer_serializado + desplazamiento, dato_create->tabla->buffer, dato_create->tabla->size);
	desplazamiento += dato_create->tabla->size;

	memcpy(buffer_serializado + desplazamiento, &(dato_create->consistencia->size), sizeof(dato_create->consistencia->size));
	desplazamiento += sizeof(dato_create->consistencia->size);

	memcpy(buffer_serializado + desplazamiento, dato_create->consistencia->buffer, dato_create->consistencia->size);
	desplazamiento += dato_create->consistencia->size;

	memcpy(buffer_serializado + desplazamiento, &(dato_create->numero_particiones), sizeof(dato_create->numero_particiones));
	desplazamiento += sizeof(dato_create->numero_particiones);

	memcpy(buffer_serializado + desplazamiento, &(dato_create->compactacion), sizeof(dato_create->compactacion));
	desplazamiento += sizeof(dato_create->compactacion);

	return buffer_serializado;

}

create crear_dato_create(char* tabla, char* consistencia, int particiones, time_t compactacion){

	struct createEstructura* dato = malloc(sizeof(struct createEstructura));

	dato->tabla = malloc(sizeof(t_stream));
	dato->tabla->size = strlen(tabla) + 1;
	dato->tabla->buffer = malloc(dato->tabla->size);
	memcpy(dato->tabla->buffer, tabla, dato->tabla->size);

	dato->consistencia = malloc(sizeof(t_stream));
	dato->consistencia->size = strlen(consistencia) + 1;
	dato->consistencia->buffer = malloc(dato->consistencia->size);
	memcpy(dato->consistencia->buffer , consistencia, dato->consistencia->size);

	dato->numero_particiones = particiones;
	dato->compactacion = compactacion;

	dato->bytes = sizeof(cod_operacion) + sizeof(dato->tabla->size) + dato->tabla->size + sizeof(dato->consistencia->size) + dato->consistencia->size
				  + sizeof(dato->numero_particiones) + sizeof(dato->compactacion);

	return dato;

}

void liberar_dato_create(create dato){

	free(dato->consistencia->buffer);
	free(dato->consistencia);

	free(dato->tabla->buffer);
	free(dato->tabla);

	free(dato);

}

