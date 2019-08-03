/*
 * planificador.c
 *
 *  Created on: 13 jun. 2019
 *      Author: utnso
 */

#include"planificador.h"

///////////////////////////////////////////////////////////////////////

void* planificador(estructura_ejecucion* cola_exec[]){

	lista_hilos_exec = list_create();

	for(int i = 0 ; i < grado_multiprocesamiento ; i ++ ){

		pthread_t hilo_exec;

		pthread_create(&hilo_exec , NULL, ejecutar_cola_exec , cola_exec[i]);

		list_add(lista_hilos_exec, hilo_exec);

	}

	return NULL;

}
///////////////////////////////////////////////////////////////////////

void mostrar_cola_final_exit(){

	printf("\n\nCOLA EXIT AL FINAL DEL PROGRAMA: \ņ");

	int contador_consola = 0;

	printf("size: %d\n" , queue_size(cola_exit));


	for(int i = 0; i < queue_size(cola_exit) ; i++){

		t_scripts* script = (t_scripts*) queue_pop(cola_exit);

		if(string_equals_ignore_case(script->path_lql , "consola")){

			contador_consola++;

		}else{

			printf("\nNombre Script: %s\n", script->path_lql);
			printf("Lineas restantes: %d\n" , list_size(script->cola_requests) );

		}

		liberar_script(script);
	}

	printf("\nLINEAS POR CONSOLA: %d\n" , contador_consola);


}

//////////////////////////////////////////////////////////////////////////

void liberar_script(t_scripts* script){

	free(script->path_lql);
	queue_destroy(script->cola_requests);
	free(script);

}

///////////////////////////////////////////////////////////////////////

void cancelar_hilos_execute(){

	for(int i = 0; i < grado_multiprocesamiento ; i++){

		pthread_t hilo_exec = (pthread_t) list_get(lista_hilos_exec , i);

		pthread_cancel(hilo_exec);

	}

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

		free(caracter_temp);

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
void inicializar_cola_exec(estructura_ejecucion* colas[] , int grado_multiprocesamiento){



	for(int i = 0 ; i < grado_multiprocesamiento; i++){

		colas[i] = malloc(sizeof(estructura_ejecucion));

		colas[i] = queue_create();

		colas[i]->numero_cola_ejecucion = i+1;

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

		char* path_archivo_lql = obtener_path_script(nuevo_lql->path_lql);

		printf("\n>Path: %s\n" , path_archivo_lql);

		archivo = fopen(path_archivo_lql , "r");

		free(path_archivo_lql);

		if(archivo != NULL){

			nuevo_lql->cola_requests = parsear_LQL(archivo);

			queue_push(cola_ready,(void*) nuevo_lql);

			fclose(archivo);

			sem_post(&semaforo_ready);

		}else{

			printf("\nNO se pudo abrir el archivo %s\n" , nuevo_lql->path_lql);

			log_error(logger_kernel, "NO se pudo abrir el ARCHIVO %s.\n" , nuevo_lql->path_lql);

		}
	}

}

////////////////////////////////////////////////////////////////////////////////

//esta funcion deberia repetirse varias veces mientras la cola_ready no este vacia
void ejecutar_cola_exec(estructura_ejecucion* cola_exec){

	while(1){

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		sem_wait(&semaforo_ready);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		pthread_rwlock_wrlock(&semaforo_cola_ready);

		t_scripts* siguiente_script = (t_scripts*)queue_pop(cola_ready);

		pthread_rwlock_unlock(&semaforo_cola_ready);

		cola_exec->cola_exec = siguiente_script->cola_requests;


		int i = 0;

		pthread_rwlock_rdlock(&semaforo_quantum);

		printf("\n========Se Ejecutara el ARCHIVO %s en la COLA EXEC %d==========\n" , siguiente_script->path_lql , cola_exec->numero_cola_ejecucion);
		log_info(logger_kernel, "===SE EJECTURA EL ARCHIVO %s En la cola de ejecucion %d===" , siguiente_script->path_lql, cola_exec->numero_cola_ejecucion);

		while( i < quantum && !queue_is_empty(cola_exec->cola_exec)){


			char* request = (char*)queue_pop(cola_exec->cola_exec);

			if(!ejecutar_request(request )){



				log_error(logger_kernel , "FALLO al ejecutar la REQUEST:  %s.", request);

				log_error(logger_kernel, "SCRIPT %s movida a COLA DE EXIT" ,siguiente_script->path_lql);

				printf("\n>NO se seguira EJECTUANDO el SCRIPT\n");

				queue_push(cola_exit, siguiente_script);

				printf("\n>Script movido a COLA DE EXIT\n");

				queue_clean(cola_exec->cola_exec);  //agrego esto no se si esta bien

				log_info(logger_kernel , ">>FIN de la REQUEST<<\n");

				printf("\n>>>>>>>>>>FIN DE LA REQUEST<<<<<<<<<<<\n\n");

				continue;

			}


			//log_info(logger_kernel , ">>FIN de la REQUEST<<\n"); // ME ROMPE EN ESTE LOG , QUE SE VAYA A CAGAR

			log_info(logger_kernel , ">>FIN de la REQUEST<<\n");
			printf("\n>>>>>>>>>>FIN DE LA REQUEST<<<<<<<<<<<<\n\n");

				 //hay que inicializar las conexiones
				 //deberia ahora recibir la operacion

			free(request);  //HAY QUE SACARLO
			i++;

		}

		pthread_rwlock_unlock(&semaforo_quantum);

		if(queue_is_empty(cola_exec->cola_exec)){

			queue_push(cola_exit, siguiente_script);

			log_info(logger_kernel, "SCRIPT  %s movida a COLA DE EXIT" , siguiente_script->path_lql);

			printf("\n>Script movido a COLA DE EXIT\n");

			log_info(logger_kernel , "===FIN DE LA EJECUCION DEL ARCHIVO %s===\n" , siguiente_script->path_lql);

			printf("\n==================Fin de Ejecucion del ARCHIVO %s===================\n" , siguiente_script->path_lql);



			//menu();

		}else{

			log_info(logger_kernel , ">>FIN de la REQUEST -- TERMINO EL QUANTUM<<\n");

			printf("\n>>>>>>>>>>>>Termino el Quantum<<<<<<<<<<<<<<<<\n\n");

			pthread_rwlock_wrlock(&semaforo_cola_ready);

			queue_push(cola_ready, siguiente_script);

			pthread_rwlock_unlock(&semaforo_cola_ready);

			sem_post(&semaforo_ready);

		}

		pthread_rwlock_rdlock(&semaforo_tiempo_ejecucion);

		usleep(tiempo_ejecucion*1000);

		pthread_rwlock_unlock(&semaforo_tiempo_ejecucion);

	}

}
