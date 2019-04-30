/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

int main (){
	t_config* g_config;
	int conexion;
	int puerto;
	char* ip;

	creacion_del_config(g_config);
	g_config = config_create("kernel.config");

	ip = config_get_string_value(g_config, "IP");
	puerto = config_get_int_value(g_config, "PUERTO");

	conexion = conectar_servidor(ip,puerto);

	printf("Bye\n");
	config_destroy(g_config);
	return EXIT_SUCCESS;
}

void creacion_del_config(t_config* g_config){

	g_config = config_create("kernel.config");

	config_set_value(g_config, "IP", "127.0.0.1");
	config_set_value(g_config, "PUERTO", "4444");
	config_save(g_config);
	config_destroy(g_config);
}
