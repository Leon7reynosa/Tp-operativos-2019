/*
 * configCreate.c
 *
 *  Created on: 21 abr. 2019
 *      Author: utnso
 */

#include "configCreate.h"

void crear_config(){

	g_config = config_create("kernel.config");

	config_set_value(g_config, "IP" , "127.0.0.1" );

	config_set_value(g_config, "PUERTO" , "8888" );

	config_save(g_config);

	config_destroy(g_config);

}

