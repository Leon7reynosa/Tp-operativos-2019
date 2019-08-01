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

	system("clear");

	/////////////////////////////////VARIABLES/////////////////////////////////

	memoria_siguiente = 0;

	memoria_t* memoria_principal;

	inotify_config argumento_inotify;

	/////////////////////////////INICIALIZACIONES//////////////////////////////

	inicializar_semaforos_consistencias();

	inicializar_semaforos_metricas();

	inicializar_semaforos_config();

	inicializar_tabla_gossiping();

	inicializar_registro_tabla();

	inicializar_metricas();

	printf("Voy a crear los loggers\n");

	inicializar_loggers();

	//creacion_del_config();

	printf("Voy a agarrar el config \n");

	obtener_datos_config();

	printf("ya agarre el config-> multiprocesamiento:  %d\n" , grado_multiprocesamiento);

	memoria_principal = crear_memoria_t(ip_memoria , puerto_memoria,  numero_memoria_seed); //si pasa pasa, modioficarlo en el config

	ingresar_a_tabla_gossiping(memoria_principal);

	conexion_memoria = memoria_principal->socket;

	inicializar_semaforo_ready();

	inicializar_consistencias();

	argumento_inotify = crear_inotify();

	t_queue* colas_exec[grado_multiprocesamiento];

	inicializar_cola_exec(colas_exec , grado_multiprocesamiento);
	inicializar_cola_new(argc, argv);
	inicializar_cola_ready();
	inicializar_cola_exit();

	/////////////////////////////MAIN//////////////////////////////

	pthread_create(&hilo_inotify, NULL, realizar_inotify, argumento_inotify);

	pthread_detach(hilo_inotify);

	pthread_create( &hilo_gossiping , NULL , realizar_gossiping, NULL );

	pthread_detach(hilo_gossiping);

	pthread_create(&hilo_refresh_metadata , NULL , refrescar_metadata , NULL);

	pthread_detach(hilo_refresh_metadata);

	pthread_create(&hilo_metrics, NULL, realizar_metrics, NULL);

	pthread_detach(hilo_metrics);

	cola_new_to_ready();

	pthread_create(&hilo_planificador, NULL, planificador, colas_exec);

	pthread_detach(hilo_planificador);

	pthread_create(&hilo_consola , NULL, consola, NULL);




	pthread_join(hilo_consola , NULL);

	cancelar_hilos_execute();

	pthread_cancel(hilo_inotify);

	pthread_cancel(hilo_metrics);

	pthread_cancel(hilo_gossiping);

	pthread_cancel(hilo_refresh_metadata);

	pthread_cancel(hilo_planificador);

	liberar_inotify(argumento_inotify);

	destruir_semaforos();

	return EXIT_SUCCESS;
}

void destruir_semaforos(){

	pthread_rwlock_destroy(&semaforo_tabla_gossiping);
	pthread_rwlock_destroy(&semaforo_metrica_ec);
	pthread_rwlock_destroy(&semaforo_metrica_sc);
	pthread_rwlock_destroy(&semaforo_metrica_shc);
	pthread_rwlock_destroy(&semaforo_registro_tabla);
	pthread_rwlock_destroy(&semaforo_eventual_c);
	pthread_rwlock_destroy(&semaforo_strong_c);
	pthread_rwlock_destroy(&semaforo_strong_hash_c);
	pthread_rwlock_destroy(&semaforo_ready);

	pthread_rwlock_destroy(&semaforo_quantum);
	pthread_rwlock_destroy(&semaforo_refresh_metadata);
	pthread_rwlock_destroy(&semaforo_tiempo_ejecucion);

}


void inicializar_semaforos_consistencias(){

	pthread_rwlock_init(&semaforo_registro_tabla , NULL);

	pthread_rwlock_init(&semaforo_eventual_c , NULL);

	pthread_rwlock_init(&semaforo_strong_c, NULL);

	pthread_rwlock_init(&semaforo_strong_hash_c, NULL);


}

void inicializar_semaforos_metricas(){

	pthread_rwlock_init(&semaforo_metrica_sc, NULL);
	pthread_rwlock_init(&semaforo_metrica_ec, NULL);
	pthread_rwlock_init(&semaforo_metrica_shc, NULL);
}

void inicializar_loggers(){

	logger_kernel = log_create("../kernel.log" , "kernel" , 0 , LOG_LEVEL_INFO);
	logger_metricas = log_create("../metricas.log" , "Metricas" , 0, LOG_LEVEL_INFO);

}









