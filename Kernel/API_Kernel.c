/*
 * API_Kernel.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Kernel.h"


////////////////////// FUNCIONES NUEVAS///////////////////////////////
//no es el mismo que enviar_request
void mandar_request(char* request_lql){

	int cod_request;
	char* nombre_tabla = string_new();

	char* consistencia = string_new();
	int tiempo_compactacion, particiones, numero_memoria;

	u_int16_t key;
	char* value = string_new();
	time_t timestamp;

	log_info(logger_kernel, request_lql);

	cod_request = identificar_request(request_lql);

	switch(cod_request){
		case SELECT:
			if(obtener_parametros_select(request_lql, nombre_tabla, &key)){

				log_info(logger_kernel, "---Se realizara el SELECT---");
				select_t select_enviar = crear_dato_select(nombre_tabla, key);
				//enviar_request(SELECT, select_enviar);

			}
			break;
		case INSERT:

			if(obtener_parametros_insert(request_lql, nombre_tabla, &key, value, &timestamp)){

				log_info(logger_kernel, "---Se realizara el INSERT---");
				insert insert_enviar = crear_dato_insert(nombre_tabla, key, value, timestamp);
				//enviar_request(INSERT, insert_enviar);

			}
			break;

		case CREATE:
			if(obtener_parametros_create(request_lql, nombre_tabla, consistencia, &particiones, &tiempo_compactacion)){

				log_info(logger_kernel , "---Se realizara el CREATE---");
				obtener_parametros_create(request_lql, nombre_tabla, consistencia, &particiones, &tiempo_compactacion);
				//enviar_request(CREATE, create_enviar);

			}
			break;

		case ADD:
			if(obtener_parametros_add(request_lql, &numero_memoria, consistencia)){

				log_info(logger_kernel, "---Se realizara la request ADD---");
				//request_add(numero_memoria, consistencia);
				log_info(logger_kernel , "SE AGREGO LA MEMORIA CORRECTAMENTE");

			}
			break;

		default:
			log_error( logger_kernel ,"-LA REQUEST NO ES VALIDA-");
			//exit(-1);
			break;
	}

}



int identificar_request(char* request_lql){

	string_to_upper(request_lql);

	if(string_starts_with(request_lql , "SELECT")){

		return SELECT;

	}else if(string_starts_with(request_lql , "INSERT")){

		return INSERT;

	}else if(string_starts_with(request_lql , "CREATE")){

		return CREATE;
	}else if(string_starts_with(request_lql , "ADD")){

		return ADD;
	}else{
		return -1;
	}

}

int obtener_parametros_select(char* linea_request, char* nombre_tabla, u_int16_t* key){

	char* funcion = string_new();

	if((sscanf(linea_request, "%s %s %d", funcion, nombre_tabla, key)) != 3){

		//log
		log_error(logger_kernel, "-LA REQUEST SELECT RECIBIO PARAMETROS INCORRECTOS.-");
		return 0;

	}

	free(funcion);
	return 1;
}

int obtener_parametros_insert(char* linea_request, char* nombre_tabla, u_int16_t* key, char* value, time_t* timestamp){
	char* funcion = string_new();

	if( (sscanf(linea_request, "%s %s %i %s %i", funcion, nombre_tabla, key, value, timestamp)) != 5){

		//
		log_error(logger_kernel , "-LA REQUEST INSERT RECIBIO PARAMETROS INCORRECTOS.-");
		return 0;

	}

	free(funcion);
	return 1;

}

int obtener_parametros_add(char* linea_request, int* numero_memoria, char* consistencia){
	char* funcion = string_new();
	char* memoria = string_new();
	char* to = string_new();

	if((sscanf(linea_request, "%s %s %i %s %s", funcion, memoria, numero_memoria, to, consistencia)) != 5){

		//log
		log_error(logger_kernel , "-LA REQUEST ADD RECIBIO PARAMETROS INCORRECTOS.-");
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

		log_error(logger_kernel, "-LA REQUEST INSERT RECIBIO PARAMETROS INCORRECTOS.-");
		return 0;

	}

	free(funcion);
	return 1;
}

int obtener_parametros_create(char* linea_request, char* nombre_tabla, char* criterio, int* numero_particiones, int* tiempo_compactacion){
	char* funcion = string_new();

	if( (sscanf(linea_request, "%s %s %s %i %i", funcion, nombre_tabla, criterio, numero_particiones, tiempo_compactacion)) != 5 ){

		//log
		log_error( logger_kernel , "-LA REQUEST CREATE RECIBIO PARAMETROS INCORRECTOS.-");
		return 0;
	}

	free(funcion);
	return 1;

}

void obtener_parametros_describe_de_una_tabla(char* linea_request, char* nombre_tabla){
	char* funcion = string_new();

	sscanf(linea_request, "%s %s", funcion, nombre_tabla);
}

void obtener_parametros_describe(char* linea_request){
	char* funcion = string_new();

	sscanf(linea_request, "%s", funcion);
}

void obtener_parametros_drop(char* linea_request, char* nombre_tabla){
	char* funcion = string_new();

	sscanf(linea_request, "%s %s", funcion, nombre_tabla);
}




/*
void enviar_describe(int conexion, char* nombre_tabla){
	operacion_describe* bufferA_Serializar = malloc(sizeof(operacion_describe));

	bufferA_Serializar->size_tabla = strlen(nombre_tabla) + 1;

	bufferA_Serializar->pedido = DESCRIBE;

	bufferA_Serializar->nombre_tabla = malloc(bufferA_Serializar->size_tabla);

	memcpy(bufferA_Serializar->nombre_tabla, nombre_tabla, bufferA_Serializar->size_tabla);

	void* buffer_serializado;
	//OPERACION + TAMAÑO_TABLA + TABLA
	int bytes = sizeof(int) + bufferA_Serializar->size_tabla + sizeof(int);

	buffer_serializado = serializar_mensaje_describe(bufferA_Serializar, bytes);

	send(conexion, buffer_serializado, bytes, 0);

	free(buffer_serializado);

	eliminar_operacion_describe(bufferA_Serializar);
}



void enviar_drop(int conexion, char* nombre_tabla){
	operacion_drop* bufferA_Serializar = malloc(sizeof(operacion_drop));

	bufferA_Serializar->size_tabla = strlen(nombre_tabla) + 1;

	bufferA_Serializar->pedido = DROP;
	bufferA_Serializar->nombre_tabla = malloc(bufferA_Serializar->size_tabla);

	memcpy(bufferA_Serializar->nombre_tabla, nombre_tabla, bufferA_Serializar->size_tabla);

	void* buffer_serializado;
	//OPERACION + TAMAÑO_TABLA + TABLA
	int bytes = sizeof(int) + bufferA_Serializar->size_tabla + sizeof(int);

	buffer_serializado = serializar_mensaje_describe(bufferA_Serializar, bytes);

	send(conexion, buffer_serializado, bytes, 0);

	free(buffer_serializado);

	eliminar_operacion_drop(bufferA_Serializar);
}

void enviar_journal(int conexion){
	void* buffer_serializado;

	buffer_serializado = serializar_mensaje_journal();

	send(conexion, buffer_serializado, sizeof(int), 0);
}
*/
