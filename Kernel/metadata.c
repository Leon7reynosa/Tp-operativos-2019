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

		log_info(logger_metadata , "INICIO ACTUALIZACION DE METADATA");

		memoria_utilizada = tomar_memoria_al_azar();

		if(memoria_utilizada == NULL){

			log_error(logger_metadata , "NO hay memorias disponibles");

			log_info(logger_metadata , "FIN ACTUALIZACION METADATA\n");

			liberar_dato_describe(dato_describe);

			continue;

		}

		log_info(logger_metadata, "Se utilizará la memoria: %d" , memoria_utilizada->numero_memoria);

		pthread_rwlock_wrlock(&memoria_utilizada->semaforo_memoria);


		if(enviar_request(DESCRIBE, dato_describe ,memoria_utilizada->socket)){


			t_list* lista_describe = recibir_describe(memoria_utilizada->socket);

			if(!list_is_empty(lista_describe)){

				log_info(logger_metadata , "Se recibieron %d TABLAS" , list_size(lista_describe));

				mostrar_lista_describe_log(lista_describe);

				actualizar_metadata(lista_describe);



			}else{


				log_info(logger_metadata , "NO se pudo actualizar la METADATA. intentelo mas tarde");

			}


			list_destroy(lista_describe); //aca no se si libearar los datos tambien

		}else{

			log_error(logger_metadata, "La MEMORIA utilizada FALLO");

			memoria_utilizada->conectado = false;

			remover_memoria_de_consistencia(memoria_utilizada);


		}

		pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

		liberar_dato_describe(dato_describe);

		log_info(logger_metadata , "FIN ACTUALIZACION METADATA\n");
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
