/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"


int main (void){

	/*
	pthread_t conexion_kernel;
	pthread_t conexion_lissandra;


	pthread_create(&conexion_kernel , NULL , conectar_kernel , NULL);
	pthread_create(&conexion_lissandra, NULL, conectar_lissandra, NULL);


	pthread_join(conexion_kernel , NULL);
	pthread_join(conexion_lissandra , NULL);
	*/

	// esto es del handshake, tabla_A y tabla_B
	int cantidad_tablas = 2;

	memoria_principal = malloc(sizeof(segmento) * cantidad_tablas);



	return EXIT_SUCCESS;
}

void* conectar_kernel(){

	int puerto = 4444;
	char* ip = "127.0.0.1";
	int socket_cliente;

	socket_sv = iniciar_servidor(ip, puerto);


	socket_cliente = aceptar_conexion(socket_sv);

	while(1){
		recibir_mensaje(socket_cliente);
	}

	close(socket_sv);
	return NULL;
}

void* conectar_lissandra(){

	int puerto = 4445;
	char* ip = "127.0.0.1";

	conexion_lissandra = conectar_servidor(ip,puerto);

	mandar_mensaje(conexion_lissandra);

	close(conexion_lissandra);
	return  NULL;
}
