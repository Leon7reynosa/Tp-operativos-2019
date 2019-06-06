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

void enviar_create(int conexion, char* nombre_tabla, char* criterio, int numero_particiones, int tiempo_compactacion){
	operacion_create* bufferA_Serializar = malloc(sizeof(operacion_create));

	bufferA_Serializar->size_tabla = strlen(nombre_tabla) + 1;
	bufferA_Serializar->size_criterio = strlen(criterio) + 1;

	bufferA_Serializar->pedido = CREATE;
	bufferA_Serializar->numero_particiones = numero_particiones;
	bufferA_Serializar->tiempo_compactacion = tiempo_compactacion;

	bufferA_Serializar->criterio = malloc(bufferA_Serializar->size_criterio);
	bufferA_Serializar->nombre_tabla = malloc(bufferA_Serializar->size_tabla);

	memcpy(bufferA_Serializar->nombre_tabla, nombre_tabla, bufferA_Serializar->size_tabla);
	memcpy(bufferA_Serializar->criterio, criterio, bufferA_Serializar->size_criterio);

	void* buffer_serializado;
	//OPERACION + TAMAÑO_TABLA + TABLA + PEDIDO + TAMANIO_CRITERIO + CRITERIO + NUMERO_PART + TIEMPO_COMP
	int bytes = sizeof(int) + bufferA_Serializar->size_tabla + sizeof(int) + bufferA_Serializar->size_criterio + sizeof(int) + sizeof(int) + sizeof(int);

	buffer_serializado = serializar_mensaje_create(bufferA_Serializar, bytes);

	send(conexion, buffer_serializado, bytes, 0);

	free(buffer_serializado);

	eliminar_operacion_create(bufferA_Serializar);

}

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
