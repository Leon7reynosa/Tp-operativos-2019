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

	memoria_siguiente = 0;

	memoria_t* memoria_principal;

	/////////////////////////////INICIALIZACIONES//////////////////////////////

	inicializar_semaforos_consistencias();

	inicializar_tabla_gossiping();

	inicializar_registro_tabla();


	logger_kernel = log_create("kernel.log" , "kernel" , 1 , LOG_LEVEL_INFO);

	creacion_del_config();
	obtener_datos_config();

	memoria_principal = crear_memoria_t(ip_memoria , puerto_memoria,  1); //si pasa pasa, modioficarlo en el config

	ingresar_a_tabla_gossiping(memoria_principal);

	conexion_memoria = memoria_principal->socket;

	inicializar_semaforo_ready();

	inicializar_consistencias();



	t_queue* colas_exec[grado_multiprocesamiento];

	inicializar_cola_exec(colas_exec , grado_multiprocesamiento);
	inicializar_cola_new(argc, argv);
	inicializar_cola_ready();
	inicializar_cola_exit();

	/////////////////////////////MAIN//////////////////////////////

	pthread_create( &hilo_gossiping , NULL , realizar_gossiping, NULL );

	pthread_detach(&hilo_gossiping);

	pthread_create(&hilo_refresh_metadata , NULL , refrescar_metadata , NULL);

	pthread_detach(&hilo_refresh_metadata);

	cola_new_to_ready();

	pthread_create(&hilo_planificador, NULL, planificador, colas_exec);

	pthread_detach(&hilo_planificador);

	pthread_create(&hilo_consola , NULL, consola, NULL);




	pthread_join(hilo_consola , NULL);

	cancelar_hilos_execute();

	pthread_cancel(hilo_gossiping);

	pthread_cancel(hilo_refresh_metadata);

	pthread_cancel(hilo_planificador);

	return EXIT_SUCCESS;
}


void inicializar_semaforos_consistencias(){

	pthread_rwlock_init(&semaforo_registro_tabla , NULL);

	pthread_rwlock_init(&semaforo_eventual_c , NULL);

	pthread_rwlock_init(&semaforo_strong_c, NULL);

	pthread_rwlock_init(&semaforo_strong_hash_c, NULL);


}









