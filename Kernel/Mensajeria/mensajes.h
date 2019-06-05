/*
 * mensajes.h
 *
 *  Created on: 24 abr. 2019
 *      Author: utnso
 */

#ifndef MENSAJES_H_
#define MENSAJES_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netdb.h>
#include<readline/readline.h>
#include<readline/history.h>

typedef enum criterio{
	SC, //Strong Consistency
	SHC, // Strong Hash Consistency
	EC //Eventual Consistency
}criterio_t;


typedef struct{

	int size;
	void* buffer;

}t_stream;

typedef enum{
	SELECT,
	INSERT,
	CREATE,
	DESCRIBE,
	DROP,
	JOURNAL
}request;


typedef struct{
	request pedido;
	int size_tabla;
	void* nombre_tabla;
	u_int16_t key;

}operacion_select;

typedef struct{
	request pedido;
	int size_tabla;
	void* nombre_tabla;
	u_int16_t key;
	int size_value;
	void* value;
	time_t timestamp;

}operacion_insert;

typedef struct{
	request pedido;
	int size_tabla;
	void* nombre_tabla;
	criterio_t criterio;
	int numero_particiones;
	int tiempo_compactacion;
}operacion_create;

typedef struct{
	request pedido;
	int size_tabla;
	void* nombre_tabla;
}operacion_describe;

typedef struct{
	request pedido;
	int size_tabla;
	void* nombre_tabla;
}operacion_drop;


//void* serializar_mensaje(t_stream* bufferA_serializar, int bytes);
void* serializar_mensaje_select(operacion_select* bufferA_serializar, int bytes);
void* serializar_mensaje_insert(operacion_insert* bufferA_serializar, int bytes);
void* serializar_mensaje_create(operacion_create* bufferA_serializar, int bytes);
void* serializar_mensaje_describe(operacion_create* bufferA_serializar, int bytes);
void* serializar_mensaje_drop(operacion_drop* bufferA_serializar, int bytes);
void* serializar_mensaje_journal();
void mandar_mensaje(int conexion);
void eliminar_tStream(t_stream* tStream);
void eliminar_operacion_select(operacion_select* buffer);
void eliminar_operacion_insert(operacion_insert* buffer);
void eliminar_operacion_create(operacion_create* buffer);
void eliminar_operacion_describe(operacion_describe* buffer);
void eliminar_operacion_drop(operacion_describe* buffer);


#endif /* MENSAJES_H_ */
