/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"

char* obtener_path_config(void){

	char* path_config = string_new();

	string_append(&path_config, "/home/utnso/memoria-3/pool.config");

	return path_config;
}

void realizar_handshake(void){

	int bytes_recibidos;

	socket_lissandra = conectar_servidor(ip_lfs, puerto_lfs);

	if(socket_lissandra < 0){
		printf("Error al conectar con LissandraFileSystem, intentarlo mas tarde.\n");
		exit(1);
	}

	desconexion_pool = false;

	bytes_recibidos = recv(socket_lissandra, &tamanio_value, sizeof(int), 0);

	if(bytes_recibidos < 0){
		printf("No pude recibir el tamanio del value\n No se realizo correctamente el handshake\n");
		exit(1);
	}

	log_info(logger, "Se establecion conexion con el File System.\n>> Tamanio value = %i\n", tamanio_value);

}

void creacion_del_config(){

	char* array_ips = "[192.168.0.168]";

	char* array_puertos = "[8001]";

	char* path_config = obtener_path_config();

	g_config = config_create(path_config);

	config_set_value(g_config, "PUERTO_ESCUCHA", "8001");
	config_set_value(g_config, "IP_LFS", "192.168.1.50");
	config_set_value(g_config, "PUERTO_LFS", "5003");

	config_set_value(g_config, "IP_SEEDS", array_ips);
	config_set_value(g_config, "ARRAY_PUERTOS", array_puertos);

	config_set_value(g_config, "RETARDO_MEMORIA", "0");
	config_set_value(g_config, "RETARDO_LFS", "0");
	config_set_value(g_config, "TAMANIO_MEMORIA", "320");
	config_set_value(g_config, "TIEMPO_JOURNAL","60000");
	config_set_value(g_config, "TIEMPO_GOSSIPING", "10000");
	config_set_value(g_config, "NUMERO_MEMORIA", "1");

	config_save(g_config);
	config_destroy(g_config);

}

void obtener_datos_config(){

	char* ip_aux;

	char* path_config = obtener_path_config();

	printf(">>\n");

	g_config = config_create(path_config);
	printf(">>\n");
	puerto_escucha   = config_get_int_value(g_config, "PUERTO_ESCUCHA");
	ip_aux		     = config_get_string_value(g_config, "IP_LFS");
	puerto_lfs	     = config_get_int_value(g_config, "PUERTO_LFS");
	printf(">>\n");
	char** ips_seeds = config_get_array_value(g_config, "IP_SEEDS");
	char** puertos_seeds = config_get_array_value(g_config, "ARRAY_PUERTOS");
	printf(">>\n");
	retardo_memoria  = config_get_int_value(g_config, "RETARDO_MEMORIA");
	retardo_lfs      = config_get_int_value(g_config, "RETARDO_LFS");
	tamanio          = config_get_int_value(g_config, "TAMANIO_MEMORIA");
	tiempo_journal   = config_get_int_value(g_config, "TIEMPO_JOURNAL");
	tiempo_gossiping = config_get_int_value(g_config, "TIEMPO_GOSSIPING");
	numero_memoria   = config_get_int_value(g_config, "NUMERO_MEMORIA");

	ip_lfs = malloc(string_length(ip_aux) + 1);
	memcpy(ip_lfs, ip_aux, string_length(ip_aux) + 1);

	ip_seeds = list_create();
	puerto_seeds = list_create();

	char* aux_seed;

	int i = 0;
	while(*(ips_seeds + i) != NULL){

		aux_seed = malloc(string_length(*(ips_seeds + i)) + 1);
		memcpy(aux_seed, *(ips_seeds + i), string_length(*(ips_seeds + i)) + 1);

		list_add(ip_seeds, aux_seed);

		i++;
	}

	int* aux_puerto;

	i = 0;
	while(*(puertos_seeds + i) != NULL){

		aux_puerto = malloc(sizeof(int));

		*aux_puerto = atoi(*(puertos_seeds + i));

		list_add(puerto_seeds, aux_puerto);

		i++;
	}


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

