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
	t_config* g_config;

	g_config = config_create("lissandra.config");
	creacion_del_config(g_config);

	ip = config_get_string_value(g_config, "IP");
	puerto = config_get_int_value(g_config, "PUERTO");

	printf("IP = %s\n",ip);
	printf("PUERTO = %d\n",puerto);

	socket_sv = iniciar_servidor( ip ,puerto );

	aceptar_conexion(socket_sv);



	printf("Bye\n");
	close(socket_sv);
	config_destroy(g_config);
	return EXIT_SUCCESS;
}

void creacion_del_config(t_config* g_config){

	g_config = config_create("lissandra.config");

	config_set_value(g_config, "IP", "127.0.0.1");
	config_set_value(g_config, "PUERTO", "4445");
	config_save(g_config);
	config_destroy(g_config);

}
