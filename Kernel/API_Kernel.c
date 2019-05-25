/*
 * API_Kernel.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Kernel.h"

void enviar_select(int conexion, char* nombre_tabla, u_int16_t key){
	operacion_select* bufferA_Serializar = malloc(sizeof(operacion_select));

	bufferA_Serializar->size_tabla = strlen(nombre_tabla) + 1;

	bufferA_Serializar->pedido = SELECT;
	bufferA_Serializar->key = key;
	bufferA_Serializar->nombre_tabla = malloc(bufferA_Serializar->size_tabla);

	memcpy(bufferA_Serializar->nombre_tabla, nombre_tabla, bufferA_Serializar->size_tabla);

	void* buffer_serializado;

	//OPERACION + TAMANO_TABLA + TABLA + KEY
	int bytes = sizeof(int) + bufferA_Serializar->size_tabla + sizeof(int) + sizeof(u_int16_t);

	buffer_serializado = serializar_mensaje_select(bufferA_Serializar, bytes);

	send(conexion, buffer_serializado, bytes, 0);

	free(buffer_serializado);

	eliminar_operacion_select(bufferA_Serializar);
}

void enviar_insert(int conexion, char* nombre_tabla, u_int16_t key, char* value, time_t timestamp){
	operacion_insert* bufferA_Serializar = malloc(sizeof(operacion_insert));

	bufferA_Serializar->size_tabla = strlen(nombre_tabla) + 1;
	bufferA_Serializar->size_value = strlen(value) + 1;

	bufferA_Serializar->pedido = INSERT;
	bufferA_Serializar->key = key;
	bufferA_Serializar->nombre_tabla = malloc(bufferA_Serializar->size_tabla);
	bufferA_Serializar->value = malloc(bufferA_Serializar->size_value);
	bufferA_Serializar->timestamp = timestamp;

	memcpy(bufferA_Serializar->nombre_tabla, nombre_tabla, bufferA_Serializar->size_tabla);
	memcpy(bufferA_Serializar->value, value, bufferA_Serializar->size_value);

	void* buffer_serializado;

	//OPERACION + TAMAÑO TABLA + TABLA + KEY + TAMAÑO VALUE + VALUE + TIMESTAMP
	int bytes = sizeof(int) + bufferA_Serializar->size_tabla + sizeof(int) + sizeof(u_int16_t) + bufferA_Serializar->size_value + sizeof(int) + sizeof(time_t);

	buffer_serializado = serializar_mensaje_insert(bufferA_Serializar, bytes);

	send(conexion, buffer_serializado, bytes, 0);

	free(buffer_serializado);

	eliminar_operacion_insert(bufferA_Serializar);

}
