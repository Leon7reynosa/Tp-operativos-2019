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

	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>NUEVA REQUEST<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

	int cod_request;
	char* nombre_archivo;
	char* nombre_tabla = string_new();

	char* consistencia = string_new();
	int tiempo_compactacion, particiones, numero_memoria, cantidad_parametros;
	time_t tiempo_inicio_ejecucion_request;
	time_t tiempo_fin_ejecucion_request;

	memoria_t* memoria_utilizada;

	u_int16_t key;
	char* value = string_new();
	time_t timestamp;

	describe_t describe_enviar;

	log_info(logger_kernel, request_lql);

	cod_request = identificar_request(request_lql);

	switch(cod_request){
		case SELECT:

			tiempo_inicio_ejecucion_request = time(NULL);

			if(obtener_parametros_select(request_lql, nombre_tabla, &key) == 3){

				log_info(logger_kernel, "---Se realizara el SELECT---\n");

				select_t select_enviar = crear_dato_select(nombre_tabla, key);

				memoria_utilizada = seleccionar_memoria_consistencia(SELECT, select_enviar);

				if(memoria_utilizada == NULL){

					log_error( logger_kernel , "NO SE ENCUENTRAN MEMORIAS DISPONIBLES PARA ESA CONSISTENCIA\n");
					return 0;

				}

				mostrar_memoria_utilizada(memoria_utilizada);

				printf("YA MOSTRE LA MEMORIA, VOY A ENVIAR\n");

				if( enviar_request(SELECT, select_enviar, memoria_utilizada->socket) == false ) {

					printf("ALGO FALLO AL ENVIAR\n");

					log_error(logger_kernel, ">>FALLO ENVIAR EL SELECT, ELIMINAMOS LA MEMORIA %d \n" , memoria_utilizada->numero_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					pthread_rwlock_wrlock(&semaforo_tabla_gossiping);

					remover_conexion(memoria_utilizada->socket , tabla_gossiping);

					pthread_rwlock_unlock(&semaforo_tabla_gossiping);

					return 0;

				}

				sumar_contador_request(memoria_utilizada);

				t_dato* dato_recibido = recibir_dato_memoria(memoria_utilizada);

				if(dato_recibido == NULL){

					printf("NO SE PUDO REALIZAR EL SELECT\n");

					tiempo_fin_ejecucion_request = time(NULL) - tiempo_inicio_ejecucion_request;

					agregar_a_metrica(SELECT, select_enviar , tiempo_fin_ejecucion_request);

					sumar_contador_memoria(memoria_utilizada);

					return 1;

				}

				mostrar_t_dato(dato_recibido);

				liberar_t_dato(dato_recibido);

				tiempo_fin_ejecucion_request = time(NULL) - tiempo_inicio_ejecucion_request;

				agregar_a_metrica(SELECT, select_enviar , tiempo_fin_ejecucion_request);

				sumar_contador_memoria(memoria_utilizada);

				liberar_dato_select(select_enviar);

				return 1;
			}
			break;
		case INSERT:

			tiempo_inicio_ejecucion_request = time(NULL);

			if(obtener_parametros_insert(request_lql, nombre_tabla, &key, &value, &timestamp)){

				log_info(logger_kernel, "---Se realizara el INSERT---\n");

				if( !existe_en_registro_tabla(nombre_tabla)){

					log_error(logger_kernel , "-La Tabla %s no existe en el registro de tablas.-\n" , nombre_tabla);

					return 0;

				}

				printf("jajajaja\n");

				insert insert_enviar = crear_dato_insert(nombre_tabla, key, value, timestamp);


				printf("jajajaja\n");
				memoria_utilizada = seleccionar_memoria_consistencia(INSERT, insert_enviar);

				printf("jajajaja\n");
				if(memoria_utilizada == NULL){

					printf("NO SE ENCUENTRAN MEMORIAS DISPONIBLES PARA ESA CONSISTENCIA\n");
					return 0;
				}

				printf("jajajaja\n");
				mostrar_memoria_utilizada(memoria_utilizada);

				printf("jajajaja\n");
				if ( enviar_request(INSERT, insert_enviar, memoria_utilizada->socket) == false ){

					log_error(logger_kernel, ">>FALLO ENVIAR EL INSERT, ELIMINAMOS LA MEMORIA %d \n" , memoria_utilizada->numero_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					pthread_rwlock_wrlock(&semaforo_tabla_gossiping);

					remover_conexion(memoria_utilizada->socket , tabla_gossiping);

					pthread_rwlock_unlock(&semaforo_tabla_gossiping);

					return 0;

				}

				printf("jajajaja\n");

				if(recibir_estado_request(memoria_utilizada) == ERROR){

					log_error(logger_kernel,  "-Fallo la request INSERT.-\n");

					return 0;

				}

				tiempo_fin_ejecucion_request = time(NULL) - tiempo_inicio_ejecucion_request;

				agregar_a_metrica(INSERT, insert_enviar , tiempo_fin_ejecucion_request);

				sumar_contador_memoria(memoria_utilizada);

				liberar_dato_insert(insert_enviar);

				sumar_contador_request(memoria_utilizada);

				return 1;
			}
			break;

		case CREATE:
			if(obtener_parametros_create(request_lql, nombre_tabla, consistencia, &particiones, &tiempo_compactacion)){


				log_info(logger_kernel , "---Se realizara el CREATE---\n");

				create create_enviar = crear_dato_create(nombre_tabla, consistencia, particiones, tiempo_compactacion);

				memoria_utilizada = seleccionar_memoria_consistencia(CREATE, create_enviar);

				if(memoria_utilizada == NULL){

					printf("NO SE ENCUENTRAN MEMORIAS DISPONIBLES PARA ESA CONSISTENCIA\n");
					return 0;
				}

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(CREATE, create_enviar, memoria_utilizada->socket) == false ){

					log_error(logger_kernel, ">>FALLO ENVIAR EL CREATE, ELIMINAMOS LA MEMORIA %d \n" , memoria_utilizada->numero_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					remover_memoria_de_tabla_gossiping(memoria_utilizada);

					return 0;

				}

				if( recibir_estado_request(memoria_utilizada) == ERROR ){

					log_error(logger_kernel, "-Fallo el CREATE.-\n");

					return 0;


				}

				Metadata metadata_agregar =  crear_metadata(nombre_tabla, consistencia, particiones, tiempo_compactacion);

				agregar_metadata_a_registro_tabla(metadata_agregar);


				return 1;
			}
			break;

		case DESCRIBE:

			//todo tenemos que hacer en mensajeria, el requestDescribe y hace su respectiva estructura describe_t

			cantidad_parametros = obtener_parametros_describe(request_lql, nombre_tabla);

			if(cantidad_parametros == 2){

				log_info(logger_kernel, "---Se realizara el DESCRIBE para la tabla %s---\n" , nombre_tabla);

				describe_enviar = crear_dato_describe(nombre_tabla);

				memoria_utilizada = tomar_memoria_al_azar();

				if(memoria_utilizada == NULL){

					printf("NO SE ENCUENTRAN MEMORIAS DISPONIBLES PARA ESA CONSISTENCIA\n");
					return 0;
				}

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(DESCRIBE, describe_enviar , memoria_utilizada->socket) == false ){

					log_error(logger_kernel, ">>FALLO ENVIAR EL DESCRIBE, ELIMINAMOS LA MEMORIA %d \n" , memoria_utilizada->numero_memoria);

					printf("consistencia de la request: %s\n" , consistencia);

					remover_memoria_de_consistencia(memoria_utilizada);

					remover_memoria_de_tabla_gossiping(memoria_utilizada);

					return 0;

				}


			}else if(cantidad_parametros == 1){

				log_info(logger_kernel, "---SE REALIZARA UN DESCRIBE GLOBAL--");

				describe_enviar = crear_dato_describe(NULL);

				memoria_utilizada = tomar_memoria_al_azar();

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(DESCRIBE, describe_enviar , memoria_utilizada->socket) == false ){

					log_error(logger_kernel, ">>FALLO ENVIAR EL DESCRIBE, ELIMINAMOS LA MEMORIA %d \n" , memoria_utilizada->numero_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					remover_memoria_de_tabla_gossiping(memoria_utilizada);

					return 0;

				}


			}else{
				return 0;
			}


			t_list* lista_describe = recibir_describe(memoria_utilizada->socket);

			if(lista_describe == NULL){

				log_error(logger_kernel, "-Fallo la request DESCRIBE.-\n");

				return 1;

			}


			mostrar_lista_describe(lista_describe);

			actualizar_metadata(lista_describe);

			list_destroy(lista_describe);

			return 1;

		case ADD:

			if(  obtener_parametros_add(request_lql, &numero_memoria, consistencia) &&  (identificar_consistencia(consistencia) >= 0 ) ){

				log_info(logger_kernel, "\n---Se realizara la request ADD---\n");
				request_add(numero_memoria, consistencia);
				log_info(logger_kernel , "SE AGREGO LA MEMORIA CORRECTAMENTE\n");
				return 1;
			}

			break;

		case RUN:


			nombre_archivo = obtener_parametros_run(request_lql);

			log_info(logger_kernel , "--Se realizara el RUN del archivo %s--\n" , nombre_archivo);

			if(nombre_archivo != NULL){

				queue_push(cola_new , nombre_archivo);

				log_info(logger_kernel, "EL ARCHIVO YA ESTA EN LA COLA DE EJECUCION.\n");


				cola_new_to_ready();

				return 1;
			}

			break;

		case DROP:

			if(obtener_parametros_drop(request_lql , nombre_tabla)){

				log_info(logger_kernel, "\n--Se realizara la request DROP de la tabla %s---\n" , nombre_tabla);

				Drop drop_enviar = crear_drop(nombre_tabla);

				if(!existe_en_registro_tabla(nombre_tabla)){

					printf("-La Tabla %s No existe en el registro de tablas.-\n" , nombre_tabla);

					return 0;

				}


				memoria_utilizada = seleccionar_memoria_consistencia(DROP , drop_enviar);

				if(memoria_utilizada == NULL){

					printf("NO SE ENCUENTRAN MEMORIAS DISPONIBLES PARA ESA CONSISTENCIA\n");
					return 0;

				}

				mostrar_memoria_utilizada(memoria_utilizada);

				if ( enviar_request(DROP, drop_enviar , memoria_utilizada->socket ) == false ){

					log_error(logger_kernel, ">>FALLO ENVIAR EL DROP, ELIMINAMOS LA MEMORIA %d \n" , memoria_utilizada->numero_memoria);

					remover_memoria_de_consistencia(memoria_utilizada);

					remover_memoria_de_tabla_gossiping(memoria_utilizada);

					//liberar_drop(drop_enviar);

					return 0;

				}


				if(recibir_estado_request(memoria_utilizada) == ERROR){

					log_error(logger_kernel , "-Fallo la request DROP.\n");

				}else{

					printf("Tabla %s removida\n" , nombre_tabla);

				}

				remover_tabla_de_registro(nombre_tabla);

				//liberar_drop(drop_enviar);

				//sacar del registro de tablas

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
			log_error( logger_kernel ,"-LA REQUEST NO ES VALIDA-\n");
			return 0;
			break;
	}


	free(nombre_tabla);
	free(consistencia);
	free(value);

	printf("\n>>>>>>>>>>>>>>>>>>>>>>>><FIN DE LA REQUEST<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

	return 0;

}


void request_journal(){

	log_info(logger_kernel, "\n---Se realizara la request JOURNAL---\n");


	t_list* lista_memorias_a_enviar = lista_memorias_de_consistencia();

	memoria_t* memoria_a_enviar;

	void _enviar_journal(void* memoria){

		memoria_a_enviar = (memoria_t* ) memoria;

		if ( enviar_request(JOURNAL, NULL , memoria_a_enviar->socket ) == false ){

			log_error(logger_kernel, ">>FALLO ENVIAR EL JOURNAL ELIMINAMOS LA MEMORIA %d \n" , memoria_a_enviar->numero_memoria);

			remover_memoria_de_consistencia(memoria_a_enviar);

			remover_memoria_de_tabla_gossiping(memoria_a_enviar);

		}


	}

	list_iterate(lista_memorias_a_enviar, _enviar_journal);

	list_destroy(lista_memorias_a_enviar);

}


int obtener_parametros_select(char* linea_request, char* nombre_tabla, u_int16_t* key){

	char* funcion = string_new();

	int cantidad_parametros;

	cantidad_parametros = sscanf(linea_request, "%s %s %d", funcion, nombre_tabla, key);

	if( cantidad_parametros != 3){

		log_error(logger_kernel, "-LA REQUEST SELECT RECIBIO PARAMETROS INCORRECTOS.-\n");
		return 0;

	}

	string_to_upper(nombre_tabla);

	free(funcion);

	return cantidad_parametros;
}

int obtener_parametros_insert(char* linea_request, char* nombre_tabla, u_int16_t* key, char** value, time_t* timestamp){

	char** auxiliar;
	char* funcion = string_new();

	char* comillas = "\"";

	auxiliar = string_n_split(linea_request, 3, comillas  );

	printf("primer valor : %s\n", auxiliar[0]);
	printf("segundo valor : %s\n", auxiliar[1]);

	if(sscanf(auxiliar[0] , "%s %s %i" , funcion, nombre_tabla, key) != 3){

		log_error(logger_kernel, "-LA REQUEST INSERT RECIBIO PARAMETROS INCORRECTOS DE NOMBRE Y KEY.-\n");
		return 0;
	}



	string_append(value , auxiliar[1]);

	printf("timestamp: %s\n", auxiliar[2]);

	if(auxiliar[2] == NULL){

		printf("si es null papa\n");

		*timestamp = -1;

		return 1;

	}else if( (sscanf(auxiliar[2] , " %d" , timestamp)) != 1 ){

		log_error(logger_kernel, "-LA REQUEST INSERT RECIBIO PARAMETROS INCORRECTOS DE TIMESTAMP.-\n");
		return 0;

	}

	string_to_upper(nombre_tabla);

	free(auxiliar);
	//free(comillas);
	free(funcion);
	return 1;

}

int obtener_parametros_add(char* linea_request, int* numero_memoria, char* consistencia){
	char* funcion = string_new();
	char* memoria = string_new();
	char* to = string_new();



	if((sscanf(linea_request, "%s %s %i %s %s", funcion, memoria, numero_memoria, to, consistencia)) != 5){

		//log
		log_error(logger_kernel , "-LA REQUEST ADD RECIBIO PARAMETROS INCORRECTOS.-\n");
		return 0;

	}

	if( !string_equals_ignore_case(consistencia, "SC") && !string_equals_ignore_case(consistencia, "EC") && !string_equals_ignore_case(consistencia, "SHC") ){

		log_error(logger_kernel , "-LACONSISTENCIA NO ES VALIDA.-\n");

		return 0;

	}

	free(funcion);
	free(memoria);
	free(to);

	return 1;
}


int obtener_parametros_insert_sin_timestamp(char* linea_request, char* nombre_tabla, u_int16_t* key, char* value){
	char* funcion = string_new();

	if( (sscanf(linea_request, "%s %s %i %s %i", funcion, nombre_tabla, key, value)) != 5 ){

		log_error(logger_kernel, "-LA REQUEST INSERT RECIBIO PARAMETROS INCORRECTOS.-\n");
		return 0;

	}

	string_to_upper(nombre_tabla);

	free(funcion);
	return 1;
}

int obtener_parametros_create(char* linea_request, char* nombre_tabla, char* criterio, int* numero_particiones, int* tiempo_compactacion){
	char* funcion = string_new();

	if( (sscanf(linea_request, "%s %s %s %i %i", funcion, nombre_tabla, criterio, numero_particiones, tiempo_compactacion)) != 5 ){

		//log
		log_error( logger_kernel , "-LA REQUEST CREATE RECIBIO PARAMETROS INCORRECTOS.-\n");
		return 0;
	}

	string_to_upper(nombre_tabla);

	free(funcion);
	return 1;

}

int obtener_parametros_describe(char* linea_request, char* nombre_tabla){
	char* funcion = string_new();

	int cantidad = sscanf(linea_request, "%s %s", funcion, nombre_tabla);

	if(cantidad == -1){
		log_error( logger_kernel , "-LA REQUEST DESCRIBE RECIBIO PARAMETROS INCORRECTOS.-\n");
		return 0;
	}

	string_to_upper(nombre_tabla);

	free(funcion);

	return cantidad;
}

int obtener_parametros_drop(char* linea_request, char* nombre_tabla){
	char* funcion = string_new();

	int cantidad_parametros;

	cantidad_parametros = sscanf(linea_request, "%s %s", funcion, nombre_tabla);

	string_to_upper(nombre_tabla);


	return cantidad_parametros;

}


char* obtener_parametros_run(char* linea_request){

	char* funcion = string_new();

	char* archivo = malloc( strlen(linea_request) - 3 );

	sscanf(linea_request, "%s %s", funcion, archivo);

	//free(funcion);

	printf("archivo: %s\n" , archivo);

//	char* archivo_devuelto = string_duplicate(archivo);

	return archivo;
}

