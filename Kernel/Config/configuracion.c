/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"


void creacion_del_config(){


	g_config = config_create("kernel.config");

	config_set_value(g_config, "IP_Memoria", "127.0.0.1");
	config_set_value(g_config, "Puerto_Memoria", "4444");
	config_set_value(g_config, "Quantum", "4");
	config_set_value(g_config, "Multiprocesamiento", "3");
	config_set_value(g_config, "Refresh_Metadata", "10000");
	config_set_value(g_config, "Retardo_Ciclo_Ejecucion", "5000");
	config_save(g_config);
	config_destroy(g_config);

}


void obtener_puerto_ip(int* puerto,char** ip){

	g_config = config_create("kernel.config");

	*puerto = config_get_int_value(g_config, "PUERTO");

	*ip = config_get_string_value(g_config,"IP");

}

