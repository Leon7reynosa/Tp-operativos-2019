/*
 * API_Kernel.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Kernel.h"


////////////////////// FUNCIONES NUEVAS///////////////////////////////
//no es el mismo que enviar_request
int ejecutar_request(char* request_lql){

	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>NUEVA REQUEST<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

	log_info(logger_kernel, "  >>>NUEVA REQUEST: %s<<< " , request_lql);

	printf("\n>REQUEST: %s\n" , request_lql);

	int cod_request;
	char* nombre_archivo;
	char* nombre_tabla ;

	char* consistencia ;
	int tiempo_compactacion, particiones, numero_memoria, cantidad_parametros;
	time_t tiempo_inicio_ejecucion_request;
	time_t tiempo_fin_ejecucion_request;

	memoria_t* memoria_utilizada;

	u_int16_t key;
	char* value ;
	time_t timestamp;

	describe_t describe_enviar;

	cod_request = identificar_request(request_lql);

	switch(cod_request){
		case SELECT:

			tiempo_inicio_ejecucion_request = time(NULL);

			printf("\n   --> REQUEST SELECT<-- \n");

			if(obtener_parametros_select(request_lql, &nombre_tabla, &key)){

				select_t select_enviar = crear_dato_select(nombre_tabla, key);

				memoria_utilizada = seleccionar_memoria_consistencia(SELECT, select_enviar);

				if(memoria_utilizada == NULL){

					free(nombre_tabla);

					return 0;

				}

				pthread_rwlock_wrlock(&memoria_utilizada->semaforo_memoria);

			/*
			 * tendrria que agregar esto?
			 *
			 * if( !memoria_utilizada->conectada){
			 *
			 * 	 	pthread_rwlock_unlock(memoria_utilizda->csemaforo);
			 *
			 * 		memoria_utilizada = seleccionar_memoria_consistencia;
			 *
			 * 		pthread_rwlock_wlock(memoria_utilizada->semaforo);
			 *
			 * }
			 *
			 */

				mostrar_memoria_utilizada(memoria_utilizada);

				if( enviar_request(SELECT, select_enviar, memoria_utilizada->socket) == false ) {

					printf("\n>Algo FALLO al enviar\n");

					log_error(logger_kernel, "-Fallo al enviar, removemos la memoria %d.- " , memoria_utilizada->numero_memoria);

					memoria_utilizada->conectado = false;

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					free(nombre_tabla);

					return 0;

				}

				t_dato* dato_recibido = recibir_dato_memoria(memoria_utilizada);

				sumar_contador_memoria(memoria_utilizada);

				pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

				if(dato_recibido == NULL){

					log_error(logger_kernel , "-NO se pudo realizar el SELECT.-");

					printf("\n>NO se pudo realizar el SELECT\n");

					tiempo_fin_ejecucion_request = time(NULL) - tiempo_inicio_ejecucion_request;

					agregar_a_metrica(SELECT, select_enviar , tiempo_fin_ejecucion_request);

					sumar_contador_memoria(memoria_utilizada);

					free(nombre_tabla);

					return 1;

				}

				mostrar_t_dato(dato_recibido);

				liberar_t_dato(dato_recibido);

				tiempo_fin_ejecucion_request = time(NULL) - tiempo_inicio_ejecucion_request;

				agregar_a_metrica(SELECT, select_enviar , tiempo_fin_ejecucion_request);

				liberar_dato_select(select_enviar);

				free(nombre_tabla);

				return 1;
			}
			break;
		case INSERT:

			tiempo_inicio_ejecucion_request = time(NULL);

			printf("\n   --> REQUEST INSERT<-- \n");

			if(obtener_parametros_insert(request_lql, &nombre_tabla, &key, &value, &timestamp)){

				if( !existe_en_registro_tabla(nombre_tabla)){

					printf("\n>No existe la TABLA %s en el registro de tabla\n", nombre_tabla);

					log_error(logger_kernel , "-La Tabla %s no existe en el registro de tablas.-" , nombre_tabla);

					free(nombre_tabla);

					free(value);

					return 0;

				}

				insert insert_enviar = crear_dato_insert(nombre_tabla, key, value, timestamp);

				memoria_utilizada = seleccionar_memoria_consistencia(INSERT, insert_enviar);

				if(memoria_utilizada == NULL){

					printf("\n>NO existen memorias para esa consistencia\n");

					free(nombre_tabla);

					free(value);

					liberar_dato_insert(insert_enviar);

					return 0;
				}

				pthread_rwlock_wrlock(&memoria_utilizada->semaforo_memoria);

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(INSERT, insert_enviar, memoria_utilizada->socket) == false ){

					log_error(logger_kernel, "-FALLO al enviar el INSERT con la MEMORIA %d" , memoria_utilizada->numero_memoria);

					printf("\n>Fallo al enviar el insert a la memoria\n");

					memoria_utilizada->conectado = false;

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					free(nombre_tabla);

					free(value);

					liberar_dato_insert(insert_enviar);

					return 0;

				}


				if(recibir_estado_request(memoria_utilizada) == ERROR){

					log_error(logger_kernel,  "-Se RECIBIO un ERROR.-");

					printf("\n>Se RECIBIO un estado de ERROR\n");

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					free(nombre_tabla);

					free(value);

					liberar_dato_insert(insert_enviar);

					return 0;

				}

				sumar_contador_memoria(memoria_utilizada);

				pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

				tiempo_fin_ejecucion_request = time(NULL) - tiempo_inicio_ejecucion_request; //ESTA PARA EL ORTO EL TIEMPO NOO SE QUE ONDA

				agregar_a_metrica(INSERT, insert_enviar , tiempo_fin_ejecucion_request);


				free(nombre_tabla); //NO SE QUE MIERDA PASA CON LA TABLA

				free(value);

				liberar_dato_insert(insert_enviar); // LO SACO POR UQE ROMPE SINO Y NO ENCUENTRO DONDE, ES EN AGREGAR METRICAS PERO NO SE POR QUE

				return 1;
			}
			break;

		case CREATE:

			printf("\n   --> REQUEST CREATE<-- \n");

			if(obtener_parametros_create(request_lql, &nombre_tabla, &consistencia, &particiones, &tiempo_compactacion)){

				create create_enviar = crear_dato_create(nombre_tabla, consistencia, particiones, tiempo_compactacion);

				memoria_utilizada = seleccionar_memoria_consistencia(CREATE, create_enviar);

				if(memoria_utilizada == NULL){

					free(consistencia);

					free(nombre_tabla);

					liberar_dato_create(create_enviar);

					return 0;
				}

				pthread_rwlock_wrlock(&memoria_utilizada->semaforo_memoria);

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(CREATE, create_enviar, memoria_utilizada->socket) == false ){

					log_error(logger_kernel, "-Fallo al enviar el CREATE con la memoria %d.-" , memoria_utilizada->numero_memoria);

					memoria_utilizada->conectado = false;

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					free(consistencia);

					free(nombre_tabla);

					liberar_dato_create(create_enviar);

					return 0;

				}

				if( recibir_estado_request(memoria_utilizada) == ERROR ){

					log_error(logger_kernel, "-Fallo el CREATE.-");

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					free(consistencia);

					free(nombre_tabla);

					liberar_dato_create(create_enviar);

					return 0;


				}

				pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

				Metadata metadata_agregar =  crear_metadata(nombre_tabla, consistencia, particiones, tiempo_compactacion);

				agregar_metadata_a_registro_tabla(metadata_agregar);

				free(consistencia);

				free(nombre_tabla);

				liberar_dato_create(create_enviar);

				return 1;
			}
			break;

		case DESCRIBE:

			//todo tenemos que hacer en mensajeria, el requestDescribe y hace su respectiva estructura describe_t

			cantidad_parametros = obtener_parametros_describe(request_lql, &nombre_tabla);

			if(cantidad_parametros == 2){

				log_info(logger_kernel, "---Se realizara el DESCRIBE para la tabla %s---\n" , nombre_tabla);

				describe_enviar = crear_dato_describe(nombre_tabla);

				memoria_utilizada = tomar_memoria_al_azar();

				if(memoria_utilizada == NULL){

					liberar_dato_describe(describe_enviar);

					free(nombre_tabla);

					return 0;

				}

				pthread_rwlock_wrlock(&memoria_utilizada->semaforo_memoria);

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(DESCRIBE, describe_enviar , memoria_utilizada->socket) == false ){

					log_error(logger_kernel, "-FALLO al enviar el DESCRIBE, eliminamos la MEMORIA %d.-" , memoria_utilizada->numero_memoria);

					printf("\nconsistencia de la request: %s\n" , consistencia);

					memoria_utilizada->conectado = false;

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					liberar_dato_describe(describe_enviar);

					free(nombre_tabla);

					return 0;

				}


			}else if(cantidad_parametros == 1){

				log_info(logger_kernel, "---SE REALIZARA UN DESCRIBE GLOBAL--");

				describe_enviar = crear_dato_describe(NULL);  //creo que hay que lierar el dato este

				memoria_utilizada = tomar_memoria_al_azar();

				if(memoria_utilizada == NULL){

					liberar_dato_describe(describe_enviar);

					return 0;

				}

				pthread_rwlock_wrlock(&memoria_utilizada->semaforo_memoria);

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(DESCRIBE, describe_enviar , memoria_utilizada->socket) == false ){

					log_error(logger_kernel, "-Fallo al enviar el DESCRIBE con la memoria %d.- " , memoria_utilizada->numero_memoria);

					memoria_utilizada->conectado = false;

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					liberar_dato_describe(describe_enviar);

					return 0;

				}


			}else{

				liberar_dato_describe(describe_enviar);

				return 0;
			}

			printf("[DESCRIBE] Voy a recibir el describe\n");

			t_list* lista_describe = recibir_describe(memoria_utilizada->socket);

			pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

			if(list_is_empty(lista_describe)){

				log_error(logger_kernel, "-Fallo la request DESCRIBE.-");

				list_destroy(lista_describe);

				liberar_dato_describe(describe_enviar);

				return 1;

			}


			mostrar_lista_describe(lista_describe);

			if(cantidad_parametros == 2){

				agregar_a_metadata(lista_describe);

			}else{
				actualizar_metadata(lista_describe);
			}
			list_destroy(lista_describe);

			liberar_dato_describe(describe_enviar);

			return 1;

		case ADD:

			printf("\n   --> REQUEST ADD<-- \n");

			if(  obtener_parametros_add(request_lql, &numero_memoria, &consistencia) &&  (identificar_consistencia(consistencia) >= 0 ) ){

				request_add(numero_memoria, consistencia);

				free(consistencia);

				return 1;
			}

			printf("\n>La REQUEST ADD FALLO.-");

			break;

		case RUN:

			printf("\n   --> REQUEST RUN<-- \n");

			nombre_archivo = obtener_parametros_run(request_lql);

			if(nombre_archivo != NULL){

				printf("\n>Se correrá el ARCHIVO: %s\n" , nombre_archivo);

				queue_push(cola_new , nombre_archivo);

				log_info(logger_kernel, "El ARCHIVO ya está en la cola de EJECUCIÓN.");


				cola_new_to_ready();

				return 1;
			}

			break;

		case DROP:

			if(obtener_parametros_drop(request_lql , &nombre_tabla)){

				log_info(logger_kernel, "-Se realizara la request DROP de la tabla %s.-" , nombre_tabla);

				Drop drop_enviar = crear_drop(nombre_tabla);

				if(!existe_en_registro_tabla(nombre_tabla)){

					printf("\n>La Tabla %s NO existe en el registro de tablas\n");

					log_error( logger_kernel, "-La Tabla %s No existe en el registro de tablas.-" , nombre_tabla);

					liberar_drop(drop_enviar);

					free(nombre_tabla);

					return 0;

				}


				memoria_utilizada = seleccionar_memoria_consistencia(DROP , drop_enviar);

				if(memoria_utilizada == NULL){

					liberar_drop(drop_enviar);

					free(nombre_tabla);


					return 0;

				}

				pthread_rwlock_wrlock(&memoria_utilizada->semaforo_memoria);

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(DROP, drop_enviar , memoria_utilizada->socket ) == false ){

					log_error(logger_kernel, "-FALLO al enviar el DROP, eliminamos la MEMORIA %d.-" , memoria_utilizada->numero_memoria);

					memoria_utilizada->conectado = false;

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					liberar_drop(drop_enviar);

					free(nombre_tabla);


					return 0;

				}


				if(recibir_estado_request(memoria_utilizada) == ERROR){

					log_error(logger_kernel , "-Fallo la request DROP.-");

					pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);

					liberar_drop(drop_enviar);

					free(nombre_tabla);


					return 0; //esto no se si esta bien

				}else{

					printf("\n>Tabla %s removida\n" , nombre_tabla);

				}

				pthread_rwlock_unlock(&memoria_utilizada->semaforo_memoria);
				//liberar_drop(drop_enviar);

				sacar_de_registro_tabla(nombre_tabla);

				liberar_drop(drop_enviar);

				free(nombre_tabla);


				return 1;
			}

			break;

		case JOURNAL:

			request_journal();

			return 1;

			break;

		case METRICS:

			request_metrics();

			return 1;

		default:
			log_error( logger_kernel ,"-LA REQUEST NO ES VALIDA-");
			return 0;
			break;
	}


	return 0;

}


void request_journal(){

	log_info(logger_kernel, "-Se realizara la request JOURNAL.-");


	t_list* lista_memorias_a_enviar = lista_memorias_de_consistencia();

	memoria_t* memoria_a_enviar;

	void _enviar_journal(void* memoria){

		memoria_a_enviar = (memoria_t* ) memoria;

		if ( enviar_request(JOURNAL, NULL , memoria_a_enviar->socket ) == false ){

			log_error(logger_kernel, "-FALLO enviar el JOURNAL, removemos la MEMORIA %d.-" , memoria_a_enviar->numero_memoria);

			memoria_a_enviar->conectado = false;

			remover_memoria_de_consistencia(memoria_a_enviar);

		}


	}

	list_iterate(lista_memorias_a_enviar, _enviar_journal);

	list_destroy(lista_memorias_a_enviar);

}


int obtener_parametros_select(char* linea_request, char** nombre_tabla , u_int16_t* key){

	char** parametros = string_split(linea_request , " ");


	for(int i = 0; i < 3 ; i++){

		if(parametros[i] == NULL){

			log_error(logger_kernel, "-la REQUEST SELECT recibio parametros incorrectos.-");

			printf("\n>La REQUEST SELECT recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;
		}

	}

	if( parametros[3] != NULL){

		log_error(logger_kernel, "-la REQUEST ADD recibio parametros de mas.-");

		printf("\n>La REQUEST ADD recibio parametros de mas\n");

		liberar_puntero_doble(parametros);

		return 0;

	}

	*nombre_tabla = malloc(strlen(parametros[1]) + 1);
	memcpy(*nombre_tabla , parametros[1] , strlen(parametros[1]) + 1);

	*key = atoi(parametros[2]);

	liberar_puntero_doble(parametros);

	return 1;

}

int obtener_parametros_insert(char* linea_request, char** nombre_tabla, u_int16_t* key, char** value, time_t* timestamp ){

	char** auxiliar;
	char** parametros;
	char* comillas = "\"";
	//int numero;

	auxiliar = string_split(linea_request, comillas  );

	for(int i = 0; i < 2 ; i++){

		if( auxiliar[i] == NULL){

			log_error(logger_kernel, "-la REQUEST INSERT recibio parametros incorrectos.-");

			printf("\n>La REQUEST INSERT recibio parametros incorrectos\n");

			liberar_puntero_doble(auxiliar);

			return 0 ;

		}

	}

	*value = malloc(strlen(auxiliar[1]) + 1);
	memcpy(*value , auxiliar[1] , strlen(auxiliar[1]) + 1);

	parametros = string_split(auxiliar[0] , " " );

	for(int j = 0 ; j < 3 ; j++){

		if(parametros[j] == NULL){

			log_error(logger_kernel, "-la REQUEST INSERT recibio parametros incorrectos.-");

			printf("\n>La REQUEST INSERT recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			liberar_puntero_doble(auxiliar);

			free(*value);

			return 0;

		}

	}

	if(parametros[3] != NULL){

		log_error(logger_kernel, "-la REQUEST INSERT recibio parametros incorrectos.-");

		printf("\n>La REQUEST INSERT recibio parametros incorrectos\n");

		liberar_puntero_doble(parametros);

		liberar_puntero_doble(auxiliar);

		free(value);

		return 0 ;

	}

	*nombre_tabla = malloc(strlen(parametros[1] ) + 1);
	memcpy(*nombre_tabla , parametros[1] , strlen(parametros[1]) + 1);

	*key = atoi(parametros[2]);



//	char* auxilio = malloc(strlen(auxiliar[2]) + 1);
//	memcpy(auxilio, auxiliar[2] , strlen(auxiliar[2] ) +1);

	if( !es_un_numero( auxiliar[2] ) ){

		*timestamp = -1;

	}else{

		string_trim(&auxiliar[2]);
		*timestamp = atoi(auxiliar[2]);


	}

	liberar_puntero_doble(parametros);

	liberar_puntero_doble(auxiliar);

	return 1;

}

int obtener_parametros_add(char* linea_request, int* numero_memoria, char** consistencia){

	char** parametros = string_split(linea_request , " ");


	for(int i = 0; i < 5 ; i++){

		if(parametros[i] == NULL){

			log_error(logger_kernel, "-la REQUEST ADD recibio parametros incorrectos.-");

			printf("\n>La REQUEST ADD recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;
		}

	}

	if ( parametros[5] != NULL ){

		log_error(logger_kernel, "-la REQUEST ADD recibio parametros de mas.-");

		printf("\n>La REQUEST ADD recibio parametros de mas\n");

		liberar_puntero_doble(parametros);

		return 0;
	}

	*consistencia = malloc(strlen(parametros[4]) + 1);
	memcpy(*consistencia, parametros[4] , strlen(parametros[4] ) + 1);

	*numero_memoria = atoi(parametros[2]);

	liberar_puntero_doble(parametros);

	return 1;

}

int obtener_parametros_create(char* linea_request, char** nombre_tabla, char** criterio, int* numero_particiones, int* tiempo_compactacion){

	char** parametros = string_split(linea_request, " ");

	for(int  i =  0 ; i < 5 ; i++){

		if(parametros[i] == NULL){

			log_error(logger_kernel, "-la REQUEST CREATE recibio parametros incorrectos.-");

			printf("\n>La REQUEST CREATE recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;


		}

	}

	if ( parametros[5] != NULL ){

		log_error(logger_kernel, "-la REQUEST CREATE recibio parametros de mas.-");

		printf("\n>La REQUEST CREATE recibio parametros de mas\n");

		liberar_puntero_doble(parametros);

		return 0;
	}

	*criterio = malloc(strlen(parametros[2]) + 1);
	memcpy(*criterio, parametros[2] , strlen(parametros[2] ) + 1);

	*nombre_tabla = malloc(strlen(parametros[1]) + 1);
	memcpy(*nombre_tabla, parametros[1] , strlen(parametros[1] ) + 1);

	*numero_particiones = atoi(parametros[3]);

	*tiempo_compactacion = atoi(parametros[4]);

	liberar_puntero_doble(parametros);

	return 1;

}


int obtener_parametros_describe(char* linea_request , char** nombre_tabla){

	char** parametros = string_split(linea_request, " ");

	if(parametros[0] == NULL){

		log_error(logger_kernel, "-la REQUEST DESCRIBE recibio parametros incorrectos.-");

		printf("\n>La REQUEST DESCRIBE recibio parametros incorrectos\n");

		return 0;

	}

	if(parametros[1] == NULL ){


		liberar_puntero_doble(parametros);

		return 1;
	}else{

		if(parametros[2] != NULL ){

			log_error(logger_kernel, "-la REQUEST DESCRIBE recibio parametros incorrectos.-");

			printf("\n>La REQUEST DESCRIBE recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;
		}else{

			*nombre_tabla = malloc(strlen(parametros[1] ) + 1);
			memcpy(*nombre_tabla , parametros[1] , strlen(parametros[1]) + 1);

			liberar_puntero_doble(parametros);

			return 2;

		}

	}


}

int obtener_parametros_drop(char* linea_request, char** nombre_tabla){

	char** parametros = string_split(linea_request, " ");

	for(int i  = 0 ; i < 2 ; i++){

		if(parametros[i] == NULL){

			log_error(logger_kernel, "-la REQUEST DROP recibio parametros incorrectos.-");

			printf("\n>La REQUEST DROP recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;

		}

	}

	if(parametros[2] != NULL){

		log_error(logger_kernel, "-la REQUEST DROP recibio parametros de mas.-");

		printf("\n>La REQUEST DROP recibio parametros de mas\n");

		liberar_puntero_doble(parametros);

		return 0;

	}

	*nombre_tabla = malloc(strlen(parametros[1] ) +1 );
	memcpy(*nombre_tabla , parametros[1] , strlen(parametros[1]) + 1);

	string_to_upper(*nombre_tabla);

	liberar_puntero_doble(parametros);

	return 1;
}

char* obtener_parametros_run(char* linea_request){

	char** parametros = string_split(linea_request , " ");

	char* archivo;

	for(int i  = 0 ; i < 2 ; i++){

		if(parametros[i] == NULL){

			log_error(logger_kernel, "-la REQUEST RUN recibio parametros incorrectos.-");

			printf("\n>La REQUEST RUN recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return NULL;

		}

		printf("ESTO TIENE EL %d : %s\n" , i , parametros[i] );

	}

	if(parametros[2] != NULL){

		printf("esto esta mal: %s\n" , parametros[2]);

		log_error(logger_kernel, "-la REQUEST RUN recibio parametros de mas.-");

		printf("\n>La REQUEST RUN recibio parametros de mas\n");

		liberar_puntero_doble(parametros);

		return NULL;

	}

	archivo = malloc(strlen(parametros[1]) + 1);
	memcpy(archivo , parametros[1] , strlen(parametros[1]) + 1);

	printf("\n>Archivo: %s\n" , archivo);
	log_info(logger_kernel, "-ARCHIVO: %s.-" , archivo) ;

	liberar_puntero_doble(parametros);

	return archivo;

}



