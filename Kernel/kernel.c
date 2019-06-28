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
	pthread_t hilo_planificador;

	/////////////////////////////INICIALIZACIONES//////////////////////////////

	creacion_del_config();
	printf("holas\n");
	obtener_datos_config();
	printf("%s\n" , ip_memoria);

	conexion_memoria = conectar_servidor(ip_memoria, puerto_memoria);

	inicializar_semaforo_ready();

	logger_kernel = log_create("kernel.log" , "kernel" , 1 , LOG_LEVEL_INFO);

	t_queue* colas_exec[grado_multiprocesamiento];

	inicializar_cola_exec(colas_exec , grado_multiprocesamiento);
	inicializar_cola_new(argc, argv);
	inicializar_cola_ready();
	inicializar_cola_exit();

	/////////////////////////////MAIN//////////////////////////////


	cola_new_to_ready();

	pthread_create(&hilo_planificador, NULL, planificador, colas_exec);

	pthread_create(&hilo_consola , NULL, consola, NULL);



	pthread_join(hilo_planificador, NULL);

	pthread_join(hilo_consola , NULL);




	return 0;
}






