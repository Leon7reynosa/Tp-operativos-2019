/*
 * planificador.c
 *
 *  Created on: 13 jun. 2019
 *      Author: utnso
 */

#include"planificador.h"

///////////////////////////////////////////////////////////////////////

void* planificador(t_queue* cola_exec[]){

	pthread_t* hilos_exec[grado_multiprocesamiento];

	for(int i = 0 ; i < grado_multiprocesamiento ; i ++ ){

		pthread_create(&hilos_exec[i] , NULL, ejecutar_cola_exec , cola_exec[i]);

	}

	return NULL;

}

///////////////////////////////////////////////////////////////////////

t_queue* parsear_LQL(FILE* archivo_lql){

	t_queue* cola_requests = queue_create();

	char caracter;
	char* caracter_temp;

	caracter = fgetc(archivo_lql);

	char* linea_leida = string_new();

	while(caracter != EOF){

		caracter_temp = string_from_format("%c", caracter);

		string_append(&linea_leida, caracter_temp);

		caracter = fgetc(archivo_lql);

		if(caracter == '\n'){

			//printf("Linea leida: %s\n" , linea_leida);

			queue_push(cola_requests, linea_leida );

			linea_leida = string_new();

			caracter = fgetc(archivo_lql);
		}

	}

	queue_push(cola_requests, linea_leida );

	return cola_requests;

}

////////////////////////////////////////////////////////////////////////////////

char* parsear_una_linea(FILE* archivo_lql){


	char* linea_leida = string_new();
	char caracter;
	char* caracter_temp;

	caracter = fgetc(archivo_lql);

	while(caracter != '\n' && caracter != EOF){

		caracter_temp = string_from_format("%c", caracter);

		string_append(&linea_leida, caracter_temp);

		caracter = fgetc(archivo_lql);
	}

	return linea_leida;

}

////////////////////////////////////////////////////////////////////////////////
void inicializar_cola_exec(t_queue* colas[] , int grado_multiprocesamiento){

	for(int i = 0 ; i < grado_multiprocesamiento; i++){

		colas[i] = queue_create();

	}

}

////////////////////////////////////////////////////////////////////////////////

void inicializar_cola_new(int argc , char* argv[]){
	cola_new = queue_create();

	for (int i = 1; i < argc ; i++){
		// pone los scripts en la cola de new
		queue_push(cola_new , argv[i]);
	}

}

////////////////////////////////////////////////////////////////////////////////

void inicializar_cola_ready(){
	cola_ready = queue_create();
}

////////////////////////////////////////////////////////////////////////////////

void inicializar_cola_exit(){
	cola_exit = queue_create();
}

////////////////////////////////////////////////////////////////////////////////

/*
 * vamos a poner en la cola de ready una estructura que tenga el FILE abierto y el path del archivo
 */
void cola_new_to_ready(){

	FILE* archivo;

	int tamanio_cola_new = queue_size(cola_new);

	for(int i = 0 ; i < tamanio_cola_new ; i++){

		t_scripts* nuevo_lql = malloc(sizeof(t_scripts));

		nuevo_lql->path_lql = (char*) queue_pop(cola_new);

		//nuevo_lql->path_lql = "/home/utnso/Escritorio/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/Kernel/lql.txt";

		archivo = fopen(nuevo_lql->path_lql , "r");

		if(archivo != NULL){

			nuevo_lql->cola_requests = parsear_LQL(archivo);

			queue_push(cola_ready,(void*) nuevo_lql);

			fclose(archivo);

			sem_post(&semaforo_ready);

		}else{

			log_error(logger_kernel, "NO SE PUDO ABRIR EL ARCHIVO %s.\n" , nuevo_lql->path_lql);

		}
	}

}

////////////////////////////////////////////////////////////////////////////////

//esta funcion deberia repetirse varias veces mientras la cola_ready no este vacia
void ejecutar_cola_exec(t_queue* cola_exec){

	while(1){

		sem_wait(&semaforo_ready);

		t_scripts* siguiente_script = (t_scripts*)queue_pop(cola_ready);

		cola_exec = siguiente_script->cola_requests;


		int i = 0;

		while( i < quantum && !queue_is_empty(cola_exec)){


			char* request = (char*)queue_pop(cola_exec);

			usleep(tiempo_ejecucion);

			if(!ejecutar_request(request )){

				log_error(logger_kernel , "FALLO AL EJECUTAR LA REQUEST %s.\n", request);

				queue_push(cola_exit, siguiente_script);

				break;

			}

				 //hay que inicializar las conexiones
				 //deberia ahora recibir la operacion

			free(request);
			i++;

		}

		if(queue_is_empty(cola_exec)){

			printf(">>Ya se completo las request<<\n");

			queue_push(cola_exit, siguiente_script);

			//menu();

		}else{

			printf(">>Termino el Quantum<<\n");

			queue_push(cola_ready, siguiente_script);

			sem_post(&semaforo_ready);

		}


	}

}
