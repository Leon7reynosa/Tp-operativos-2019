/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"


void creacion_del_config(){

	FILE* config_kernel = fopen("kernel.config" , "w");
	fclose(config_kernel);

	g_config = config_create("kernel.config");

	config_set_value(g_config, "IP_MEMORIA", "192.168.1.50");
	config_set_value(g_config, "PUERTO_MEMORIA", "8001");
	config_set_value(g_config, "NUMERO_MEMORIA" , "1");
	config_set_value(g_config, "QUANTUM", "3");
	config_set_value(g_config, "GRADO_MULTIPROCESAMIENTO", "1");
	config_set_value(g_config, "REFRESH_METADATA", "15000");
	config_set_value(g_config, "RETARDO_CICLO_EJECUCION", "0");
	config_set_value(g_config, "TIEMPO_GOSSIPING_KERNEL", "30000");
	config_set_value(g_config, "PUNTO_MONTAJE", "/home/utnso/Escritorio/TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/Kernel");
	//capaz necesitemos un punto de montaje
	config_save(g_config);
	config_destroy(g_config);

}

void obtener_datos_config(){

	g_config = config_create("kernel.config");
	ip_memoria = string_new();
	char* ip_auxiliar;
	char* punto_montaje_aux;

	puerto_memoria = config_get_int_value(g_config, "PUERTO_MEMORIA");
	ip_auxiliar = config_get_string_value(g_config, "IP_MEMORIA");
	numero_memoria_seed = config_get_int_value(g_config, "NUMERO_MEMORIA");
	quantum = config_get_int_value(g_config , "QUANTUM");
	grado_multiprocesamiento = config_get_int_value(g_config , "GRADO_MULTIPROCESAMIENTO");
	tiempo_refresh_metadata = config_get_int_value(g_config, "REFRESH_METADATA");
	tiempo_ejecucion = config_get_int_value(g_config, "RETARDO_CICLO_EJECUCION");
	tiempo_gossiping_kernel = config_get_int_value(g_config , "TIEMPO_GOSSIPING_KERNEL");
	punto_montaje_aux = config_get_string_value(g_config, "PUNTO_MONTAJE");

	string_append(&ip_memoria, ip_auxiliar);

	punto_montaje = malloc(sizeof(punto_montaje_aux) + 1);
	memcpy(punto_montaje , punto_montaje_aux , sizeof(punto_montaje_aux) + 1);

//	free(ip_auxiliar);

	config_destroy(g_config);

}

void inicializar_semaforos_config(){

	pthread_rwlock_init(&semaforo_quantum);

	pthread_rwlock_init(&semaforo_tiempo_ejecucion);

	pthread_rwlock_init(&semaforo_refresh_metadata);

}

char* obtener_path_config(){


	char* path = string_new();

	string_append(&path, punto_montaje );

	string_append(&path, "/");

	string_append(&path , "kernel.config");

	return path;

}

