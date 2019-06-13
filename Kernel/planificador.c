/*
 * planificador.c
 *
 *  Created on: 13 jun. 2019
 *      Author: utnso
 */

#include"planificador.h"

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

	//printf("Linea leida: %s\n" , linea_leida);

	queue_push(cola_requests, linea_leida );

	return cola_requests;

}

////////////////////////////////////////////////////////////////////////////////
void inicializar_cola_exec(t_queue* colas[] , int grado_multiprocesamiento){

	for(int i = 0 ; i < grado_multiprocesamiento; i++){

		colas[i] = queue_create();

	}

}

////////////////////////////////////////////////////////////////////////////////

void inicializar_cola_new(){
	cola_new = queue_create();
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
 * Esto deberia pasar una t_list por cada script con distintas requests (char*)
 * y cada t_list se agregaria como elemento de la cola de ready;
 */
void cola_new_to_ready(){

	FILE* archivo;

	int tamanio_cola_new = queue_size(cola_new);

	for(int i = 0 ; i < tamanio_cola_new ; i++){

		t_planificador* nuevo_lql = malloc(sizeof(t_planificador));

		nuevo_lql->path_lql = (char*) queue_pop(cola_new);

		archivo = fopen(nuevo_lql->path_lql , "r");

		nuevo_lql->cola_requests = parsear_LQL(archivo);

		queue_push(cola_ready,(void*) nuevo_lql);

		fclose(archivo);

	}

}

////////////////////////////////////////////////////////////////////////////////

//esta funcion deberia repetirse varias veces mientras la cola_ready no este vacia
void cola_ready_a_exec(t_queue* cola_exec){

	 t_planificador* siguiente_script = (t_planificador*)queue_pop(cola_ready);

	 cola_exec = siguiente_script->cola_requests;

	 int i = 0;

	 while( i < quantum && !queue_is_empty(cola_exec)){


		 char* request = (char*)queue_pop(cola_exec);

		 printf("Request: %s\n" , request);

		// mandar_request(request , socket_pool); //hay que inicializar las conexiones

		 //deberia ahora recibir la operacion

		 //si falla, mando a la cola de exit

		 free(request);
		 i++;

	 }

	 if(queue_is_empty(cola_exec)){

		 printf("Ya se completo las request\n");

		 queue_push(cola_exit, siguiente_script->path_lql);

	 }else{

		 printf("Termino el quantum\n");

		 queue_push(cola_ready, siguiente_script);

	 }

	 //si no esta vacia no hago nada

}
