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

	int cod_request;
	char* nombre_archivo;
	char* nombre_tabla = string_new();

	char* consistencia = string_new();
	int tiempo_compactacion, particiones, numero_memoria, cantidad_parametros;

	u_int16_t key;
	char* value = string_new();
	time_t timestamp;

	describe_t describe_enviar;

	log_info(logger_kernel, request_lql);

	cod_request = identificar_request(request_lql);

	switch(cod_request){
		case SELECT:

			if(obtener_parametros_select(request_lql, nombre_tabla, &key) == 3){

				log_info(logger_kernel, "---Se realizara el SELECT---\n");
				select_t select_enviar = crear_dato_select(nombre_tabla, key);
				enviar_request(SELECT, select_enviar);

				return 1;
			}
			break;
		case INSERT:

			if(obtener_parametros_insert(request_lql, nombre_tabla, &key, &value, &timestamp)){

				log_info(logger_kernel, "---Se realizara el INSERT---\n");

				insert insert_enviar = crear_dato_insert(nombre_tabla, key, value, timestamp);

				enviar_request(INSERT, insert_enviar);
				return 1;
			}
			break;

		case CREATE:
			if(obtener_parametros_create(request_lql, nombre_tabla, consistencia, &particiones, &tiempo_compactacion)){

				log_info(logger_kernel , "---Se realizara el CREATE---\n");
				create create_enviar = crear_dato_create(nombre_tabla, consistencia, particiones, tiempo_compactacion);

				printf("se creo el dato create, tabla: %s \n" , create_enviar->tabla->buffer);
				enviar_request(CREATE, create_enviar);
				printf("no pasaste el enviar bro\n");

				return 1;
			}
			break;

		case DESCRIBE:

			//todo tenemos que hacer en mensajeria, el requestDescribe y hace su respectiva estructura describe_t

			cantidad_parametros = obtener_parametros_describe(request_lql, nombre_tabla);

			if(cantidad_parametros == 2){

				log_info(logger_kernel, "---Se realizara el DESCRIBE para la tabla %s---\n" , nombre_tabla);

				describe_enviar = crear_dato_describe(nombre_tabla);

				printf("tabla que enviamos %s\n" , (char*)describe_enviar->tabla->buffer);

				enviar_request(DESCRIBE, describe_enviar);

				printf("se envio bien el describe\n");

			}else if(cantidad_parametros == 1){

				log_info(logger_kernel, "---SE REALIZARA UN DESCRIBE GLOBAL--");

				describe_enviar = crear_dato_describe(NULL);
				enviar_request(DESCRIBE, describe_enviar);


			}else{
				return 0;
			}

			printf("Voy a esperar la respuesta de la memoria\n");

			t_list* lista_describe = recibir_describe(conexion_memoria);

			printf("Meti todas las metadatas en una lista\n");

			printf("Muestro la lista\n");
			mostrar_lista_describe(lista_describe);
			printf("Termine de mostrar\n");

			printf("Elimino la lista de metadatas\n");
			list_destroy_and_destroy_elements(lista_describe, liberar_metadata);

			printf("Termino el describe!\n");
			return 1;

		case ADD:

			if(obtener_parametros_add(request_lql, &numero_memoria, consistencia)){

				log_info(logger_kernel, "---Se realizara la request ADD---\n");
				request_add(numero_memoria, consistencia);
				log_info(logger_kernel , "SE AGREGO LA MEMORIA CORRECTAMENTE\n");
				return 1;
			}

			break;

		case RUN:
			nombre_archivo = obtener_parametros_run(request_lql);

			if(nombre_archivo != NULL){

				queue_push(cola_new , nombre_archivo);

				log_info(logger_kernel, "EL ARCHIVO YA ESTA EN LA COLA DE EJECUCION.\n");

				cola_new_to_ready();

				return 1;
			}

			break;

		case DROP:

			printf("entre al drop\n");

			if(obtener_parametros_drop(request_lql , nombre_tabla)){
				printf("entre al if, nombre : %s\n" , nombre_tabla);

				Drop drop_enviar = crear_drop(nombre_tabla);

				printf("ya lo cree bro\n");

				enviar_request(DROP, drop_enviar );

				printf("ya lo envie bro\n");

				return 1;
			}

			break;

		case JOURNAL:

			request_journal();

			return 1;

			break;

		default:
			log_error( logger_kernel ,"-LA REQUEST NO ES VALIDA-\n");
			return 0;
			break;
	}


	free(nombre_tabla);
	free(consistencia);
	free(value);
	return 0;

}


void request_journal(){

	t_list* lista_memorias_a_enviar = lista_memorias_de_consistencia();

	memoria_t* memoria_a_enviar;

	void _enviar_journal(void* memoria){

		memoria_a_enviar = (memoria_t* ) memoria;

		enviar_request(JOURNAL, NULL /*, memoria->socket*/ ); //esto deberiamos agregar cuando modifiquemos

	}

	list_iterate(lista_memorias_a_enviar, _enviar_journal);

	list_destroy(lista_memorias_a_enviar);

}

t_list* lista_memorias_de_consistencia(){

	t_list* lista_de_memorias = list_create();

	if(Strong_C != NULL){

		list_add(lista_de_memorias, Strong_C);

	}

	void _agregar_si_cumple(void* _memoria){

		memoria_t* memoria = (memoria_t *)_memoria;

		bool _esta_agregado(void* _memoria_2){

				memoria_t* memoria_2 = (memoria_t *)_memoria_2 ;

				return memoria == memoria_2;

		}

		if(list_any_satisfy(lista_de_memorias, _esta_agregado)){
			//no lo agrego

		}else{

			list_add(lista_de_memorias, memoria);

		}

	}

	list_iterate(Strong_Hash_C, _agregar_si_cumple);

	list_iterate(Eventual_C, _agregar_si_cumple);

	return lista_de_memorias;

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

	if(auxiliar[2] == NULL){

		timestamp = -1;

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

	char* archivo = string_new();

	sscanf(linea_request, "%s %s", funcion, archivo);

	printf("El archivo se llama: %s\n", archivo);

	free(funcion);

	return archivo;
}

