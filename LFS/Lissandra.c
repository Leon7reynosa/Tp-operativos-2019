/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"

int main(void){

	int socket_sv;
	char* ip;
	int puerto;

	obtener_puerto_ip(&puerto,&ip);

	printf("IP = %s\n",ip);
	printf("PUERTO = %d\n",puerto);

	socket_sv = iniciar_servidor( ip ,puerto );

	aceptar_conexion(socket_sv);

	close(socket_sv);
	config_destroy(g_config);
	return EXIT_SUCCESS;
}

