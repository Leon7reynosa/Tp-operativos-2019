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

		usleep(tiempo_refresh_metadata * 1000);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		dato_describe = crear_dato_describe(NULL);

		printf("\n////////////////////////////ACTUALIZACION METADATA/////////////////////////////\n");

		memoria_utilizada = tomar_memoria_al_azar();;

		if(memoria_utilizada == NULL){

			printf("\n///////////////////////////FIN ACTUALIZACION METADATA//////////////////////////////\n");

			continue;

		}

		if(enviar_request(DESCRIBE, dato_describe ,memoria_utilizada->socket)){

			t_list* lista_describe = recibir_describe(memoria_utilizada->socket);

			if(!list_is_empty(lista_describe)){

				mostrar_lista_describe(lista_describe);

				actualizar_metadata(lista_describe);

			}else{

				printf("NO SE PUDO ACTUALIZAR LA METADATA, INTENTELO MAS TARDE\n");

			}



			list_destroy(lista_describe); //aca no se si libearar los datos tambien

		}else{

			printf("LA MEMORIA UTILIZADA FALLO\n");

			remover_memoria_de_consistencia(memoria_utilizada);


		}

		printf("\n///////////////////////////FIN ACTUALIZACION METADATA//////////////////////////////\n");
	}




	return NULL;
}
