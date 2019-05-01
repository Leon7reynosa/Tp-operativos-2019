/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

int main (){
	int conexion;
	int puerto;
	char* ip;

	obtener_puerto_ip(&puerto,&ip);

	conexion = conectar_servidor(ip,puerto);

	config_destroy(g_config);
	return EXIT_SUCCESS;
}

