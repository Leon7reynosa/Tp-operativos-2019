/*
 * API_Kernel.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Kernel.h"


void enviar_describe(int conexion, char* nombre_tabla){
	operacion_describe* bufferA_Serializar = malloc(sizeof(operacion_describe));

	bufferA_Serializar->size_tabla = strlen(nombre_tabla) + 1;

	bufferA_Serializar->pedido = DESCRIBE;

	bufferA_Serializar->nombre_tabla = malloc(bufferA_Serializar->size_tabla);

	memcpy(bufferA_Serializar->nombre_tabla, nombre_tabla, bufferA_Serializar->size_tabla);

	void* buffer_serializado;
	//OPERACION + TAMAÑO_TABLA + TABLA
	int bytes = sizeof(int) + bufferA_Serializar->size_tabla + sizeof(int);

	buffer_serializado = serializar_mensaje_describe(bufferA_Serializar, bytes);

	send(conexion, buffer_serializado, bytes, 0);

	free(buffer_serializado);

	eliminar_operacion_describe(bufferA_Serializar);
}

void enviar_drop(int conexion, char* nombre_tabla){
	operacion_drop* bufferA_Serializar = malloc(sizeof(operacion_drop));

	bufferA_Serializar->size_tabla = strlen(nombre_tabla) + 1;

	bufferA_Serializar->pedido = DROP;
	bufferA_Serializar->nombre_tabla = malloc(bufferA_Serializar->size_tabla);

	memcpy(bufferA_Serializar->nombre_tabla, nombre_tabla, bufferA_Serializar->size_tabla);

	void* buffer_serializado;
	//OPERACION + TAMAÑO_TABLA + TABLA
	int bytes = sizeof(int) + bufferA_Serializar->size_tabla + sizeof(int);

	buffer_serializado = serializar_mensaje_describe(bufferA_Serializar, bytes);

	send(conexion, buffer_serializado, bytes, 0);

	free(buffer_serializado);

	eliminar_operacion_drop(bufferA_Serializar);
}

void enviar_journal(int conexion){
	void* buffer_serializado;

	buffer_serializado = serializar_mensaje_journal();

	send(conexion, buffer_serializado, sizeof(int), 0);
}
