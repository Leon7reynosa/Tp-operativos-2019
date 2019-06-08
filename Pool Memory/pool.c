/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"

int main (void){

	pthread_t conexion_kernel;

	pthread_create(&conexion_kernel , NULL , conectar_kernel, NULL);




	pthread_join(conexion_kernel, NULL);


	return EXIT_SUCCESS;
}

void* conectar_kernel(){

	int puerto = 4446;
	char* ip = "127.0.0.1";
	int socket_cliente;
	int socket_sv = iniciar_servidor(ip, puerto);
	request request;

	socket_kernel = aceptar_conexion(socket_sv);

	request = recibir_request(socket_kernel);

	printf("codigo de operacion: %d\n" , request->cod_op);



	//faltaria mandar la respuesta

	close(socket_sv);


	return NULL;
}


void* conectar_lissandra(){

	socket_lissandra = conectar_servidor("127.0.0.1" , 4445);

	create create_enviar =  crear_dato_create("Tabla_A" , "SC" , 5, 20000);

	printf("Dato: %s " , (char *)(create_enviar->tabla->buffer));
	printf("bytes: %d", create_enviar->bytes);

	enviar_request(CREATE, create_enviar);

	while(1){}

	return NULL;
}

