/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"


void creacion_del_config(){


	g_config = config_create("pool.config");

	config_set_value(g_config, "IP", "127.0.0.1");
	config_set_value(g_config, "PUERTO", "4444");
	config_set_value(g_config, "TAM_MEM", "2048");
	config_save(g_config);
	config_destroy(g_config);

}


void obtener_puerto_ip(int* puerto,char** ip){

	g_config = config_create("pool.config");

	*puerto = config_get_int_value(g_config, "PUERTO");

	*ip = config_get_string_value(g_config,"IP");

}

int obtener_tamanio_memoria(){

	int tamanio_memoria;

	g_config = config_create("pool.config");

	tamanio_memoria = config_get_int_value(g_config, "TAM_MEM");

	return tamanio_memoria;
}

