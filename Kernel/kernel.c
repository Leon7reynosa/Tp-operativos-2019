/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

int main (){
	char* ip = obtenerIp();
	char* puerto = obtenerPuerto();
	char* mensaje = "hola chino";

	int socket_kernel = crear_conexion( ip , puerto );

	send(socket_kernel , mensaje, strlen(mensaje) + 1 , 0);

	liberar_conexion(socket_kernel);
	return EXIT_SUCCESS;
}

char* obtenerIp (){

	char* ip;

	t_config* kernel_config;

	kernel_config = config_create("kernel.config");

	ip = config_get_string_value(kernel_config , "IP");

	config_destroy(kernel_config);

	return ip;
}

char* obtenerPuerto (){

	char* puerto;

	t_config* kernel_config;

	kernel_config = config_create("kernel.config");

	puerto = config_get_string_value(kernel_config , "PUERTO");

	config_destroy(kernel_config);

	return puerto;

}
