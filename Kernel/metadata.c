/*
 * metadata.c
 *
 *  Created on: 22 jul. 2019
 *      Author: utnso
 */


#include"metadata.h"

void* refrescar_metadata(){

	memoria_t* memoria_utilizada;

	describe_t dato_describe;

	while(1){

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		pthread_rwlock_rdlock(&semaforo_refresh_metadata);

		usleep(tiempo_refresh_metadata * 1000);

		pthread_rwlock_unlock(&semaforo_refresh_metadata);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		dato_describe = crear_dato_describe(NULL);

		printf("\n////////////////////////////ACTUALIZACION METADATA/////////////////////////////\n");

		memoria_utilizada = tomar_memoria_al_azar();

		if(memoria_utilizada == NULL){

			printf("\n///////////////////////////FIN ACTUALIZACION METADATA//////////////////////////////\n");

			continue;

		}

		printf("Ya agarre una memoria: %d\n", memoria_utilizada->numero_memoria);

		pthread_rwlock_wrlock(&memoria_utilizada->semaforo_memoria);

		printf("Ya agarre el semaforo de la memoria\n");


		if(enviar_request(DESCRIBE, dato_describe ,memoria_utilizada->socket)){

			printf("Se envio bien\n");

			t_list* lista_describe = recibir_describe(memoria_utilizada->socket);

			if(!list_is_empty(lista_describe)){

				printf("la lista no esta vacia\n");

				mostrar_lista_describe(lista_describe);

				actualizar_metadata(lista_describe);

				printf("ya actualice\n");

			}else{


				printf("NO SE PUDO ACTUALIZAR LA METADATA, INTENTELO MAS TARDE\n");

			}



			list_destroy(lista_describe); //aca no se si libearar los datos tambien

		}else{

			printf("LA MEMORIA UTILIZADA FALLO\n");

			memoria_utilizada->conectado = false;

			remover_memoria_de_consistencia(memoria_utilizada);


		}

		pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

		printf("\n///////////////////////////FIN ACTUALIZACION METADATA//////////////////////////////\n");
	}




	return NULL;
}

char* obtener_path_script(char* archivo){

	char* path = string_new();

	string_append(&path , punto_montaje);

	string_append(&path, "Scripts");

	string_append(&path , "/");

	string_append(&path , archivo);

	return path;

}
