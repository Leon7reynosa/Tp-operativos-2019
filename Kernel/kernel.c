/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

int main (int argc , char* argv[]){

	/////////////////////////////////VARIABLES/////////////////////////////////

	pthread_t hilo_consola;

	/////////////////////////////INICIALIZACIONES//////////////////////////////

	creacion_del_config();
	obtener_datos_config();

	logger_kernel = log_create("kernel.log" , "kernel" , 1 , LOG_LEVEL_INFO);
	log_info(logger_kernel, "LOGGER CREADO CORRECTAMENTE.");

	t_queue* colas_exec[grado_multiprocesamiento];
	inicializar_cola_exec(colas_exec , grado_multiprocesamiento);
	inicializar_cola_new(argc, argv);
	inicializar_cola_ready();
	inicializar_cola_exit();

	/////////////////////////////MAIN//////////////////////////////


	pthread_create(&hilo_consola , NULL, consola, NULL);

	pthread_join(hilo_consola , NULL);

	return 0;
}





