/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"

void realizar_handshake(void){

	int bytes_recibidos;

	socket_lissandra = conectar_servidor(ip_lfs, puerto_lfs);

	if(socket_lissandra < 0){
		printf("Error al conectar con LissandraFileSystem, intentarlo mas tarde.\n");
		exit(1);
	}

	bytes_recibidos = recv(socket_lissandra, &tamanio_value, sizeof(int), 0);

	if(bytes_recibidos < 0){
		printf("No pude recibir el tamanio del value\n No se realizo correctamente el handshake\n");
		exit(1);
	}

}

void creacion_del_config(){

	g_config = config_create("pool.config");

	config_set_value(g_config, "PUERTO_ESCUCHA", "8000");
	config_set_value(g_config, "IP_LFS", "127.0.0.1");
	config_set_value(g_config, "PUERTO_LFS", "4445");
	config_set_value(g_config, "RETARDO_MEMORIA", "600");
	config_set_value(g_config, "RETARDO_LFS", "600");
	config_set_value(g_config, "TAMANIO_MEMORIA", "2048");
	config_set_value(g_config, "TIEMPO_JOURNAL","60000");
	config_set_value(g_config, "TIEMPO_GOSSIPING", "30000");
	config_set_value(g_config, "NUMERO_MEMORIA", "1");
	config_save(g_config);
	config_destroy(g_config);

}

void obtener_datos_config(){

	char* ip_aux;

	g_config = config_create("pool.config");

	puerto_escucha   = config_get_int_value(g_config, "PUERTO_ESCUCHA");
	ip_aux		     = config_get_string_value(g_config, "IP_LFS");
	puerto_lfs	     = config_get_int_value(g_config, "PUERTO_LFS");
	retardo_memoria  = config_get_int_value(g_config, "RETARDO_MEMORIA");
	retardo_lfs      = config_get_int_value(g_config, "RETARDO_LFS");
	tamanio          = config_get_int_value(g_config, "TAMANIO_MEMORIA");
	tiempo_journal   = config_get_int_value(g_config, "TIEMPO_JOURNAL");
	tiempo_gossiping = config_get_int_value(g_config, "TIEMPO_GOSSIPING");
	numero_memoria   = config_get_int_value(g_config, "NUMERO_MEMORIA");

	ip_lfs = malloc(strlen(ip_aux) + 1);
	memcpy(ip_lfs, ip_aux, strlen(ip_aux) + 1);


	//SI HAY ALGO LOCO, ES ESTO DE ABAJO XD
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

