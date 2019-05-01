/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"


int main (void){


	int socket_sv;
	int puerto;
	char* ip;

	obtener_puerto_ip(&puerto,&ip);

	printf("puerto = %d\n", puerto);
	printf("ip = %s\n", ip);

	socket_sv = iniciar_servidor(ip, puerto);

	aceptar_conexion(socket_sv);


	config_destroy(g_config);
	close(socket_sv);
	return EXIT_SUCCESS;
}
