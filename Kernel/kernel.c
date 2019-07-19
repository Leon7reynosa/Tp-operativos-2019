/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

void pruebas(){

	char* request = "DESCrIBEs HOLA 43";

	int auxiliar = identificar_request(request);

	printf(	"el numero es: %d\n" , auxiliar);

}

int main (int argc , char* argv[]){


	/////////////////////////////////VARIABLES/////////////////////////////////

	pthread_t hilo_gossiping;
	pthread_t hilo_consola;
	pthread_t hilo_planificador;

	memoria_t* memoria_principal;

	/////////////////////////////INICIALIZACIONES//////////////////////////////

	inicializar_tabla_gossiping();

	logger_kernel = log_create("kernel.log" , "kernel" , 1 , LOG_LEVEL_INFO);

	creacion_del_config();
	obtener_datos_config();

	memoria_principal = crear_memoria_t(ip_memoria , puerto_memoria,  1); //si pasa pasa, modioficarlo en el config

	printf("llegueeep\n");

	ingresar_a_tabla_gossiping(memoria_principal);

	printf("llegueeep\n");

	conexion_memoria = memoria_principal->socket;

	printf("llegueeep\n");

	inicializar_semaforo_ready();

	printf("paso 1\n");

	//tabla_gossiping = list_create(); // despues poner en una funcion inicializar gossiping y probar el ADD

	inicializar_consistencias();



	t_queue* colas_exec[grado_multiprocesamiento];

	printf("paso 3\n");

	inicializar_cola_exec(colas_exec , grado_multiprocesamiento);
	inicializar_cola_new(argc, argv);
	inicializar_cola_ready();
	inicializar_cola_exit();

	/////////////////////////////MAIN//////////////////////////////

	pthread_create( &hilo_gossiping , NULL , realizar_gossiping, NULL );

	pthread_detach(&hilo_gossiping);

	printf("paso 4\n");

	cola_new_to_ready();

	printf("pasaste bro\n");

	pthread_create(&hilo_planificador, NULL, planificador, colas_exec);

	pthread_create(&hilo_consola , NULL, consola, NULL);


	pthread_join(hilo_planificador, NULL);

	pthread_join(hilo_consola , NULL);

	return 0;
}












