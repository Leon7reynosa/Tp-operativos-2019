/*
 * consola.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include"consola.h"

void* consola(void* argumento){

	char* leido;

	cod_operacion codigo;

	menu();

	leido = readline(">> ");

	while(!string_equals_ignore_case(leido, "exit")){

		codigo = identificar_request(leido);

		ejecutar_request(codigo, leido);

		free(leido);

		leido = readline(">> ");

	}

	free(leido);

	pthread_exit(NULL);

	return NULL;
}


bool ejecutar_request(cod_operacion codigo_request , char* linea_request){

	printf("\n//////////////////////////////////////REQUEST NUEVA DE CONSOLA////////////////////////////////////\n");

	request nueva_request;
	bool fin_funcion = false;

	int cantidad_parametros;

	void* tipo_request;

	char*nombre_tabla;
	char* value;
	u_int16_t key;
	time_t timestamp;

	char* consistencia;
	int particiones;
	int compactacion;

	t_list* lista_describe;

	switch( codigo_request ){

		case SELECT:

			printf(">>SELECT<<\n");

			if(obtener_parametros_select(linea_request, nombre_tabla, &key) == 3){

				select_t dato_select = crear_dato_select(nombre_tabla, key);

				dato_t* dato = request_select(dato_select);

				//mostrar_dato(dato);

				liberar_dato(dato);

				liberar_dato_select(dato_select);

				fin_funcion = true;
			}

			break;
		case INSERT:

			printf(">>INSERT<<\n");

			if( obtener_parametros_insert(linea_request, nombre_tabla, &key , value, &timestamp) ){

				if(timestamp < 0 ){

					timestamp = time(NULL);

				}

				insert dato_insert = crear_dato_insert(nombre_tabla, key,value, timestamp  );

				request_insert(dato_insert);

				liberar_dato_insert(dato_insert);

				fin_funcion = true;

			}

			break;
		case CREATE:

			printf(">>CREATE<<");

			if(obtener_parametros_create(linea_request, nombre_tabla, consistencia, &particiones, &compactacion)){

				create dato_create = crear_dato_create(nombre_tabla, consistencia, particiones, compactacion);

				request_create(dato_create);

				liberar_dato_create(dato_create);

				fin_funcion = true;
			}

			break;
		case DESCRIBE:

			printf(">>DESCRIBE<<");

			cantidad_parametros = obtener_parametros_describe(linea_request, nombre_tabla);

			if(cantidad_parametros == 2){

				//log_info(logger_kernel, "---Se realizara el DESCRIBE para la tabla %s---\n" , nombre_tabla);

				describe_t describe_enviar = crear_dato_describe(nombre_tabla);

				printf("tabla que enviamos %s\n" , (char*)describe_enviar->tabla->buffer);

				lista_describe = request_describe(describe_enviar);

				liberar_dato_describe(describe_enviar); //capaz no hya que liberaar

			}else if(cantidad_parametros == 1){

				//log_info(logger_kernel, "---SE REALIZARA UN DESCRIBE GLOBAL--")

				lista_describe = request_describe_global();


			}else{
				return false;
			}

			//mostrar_lista_describe(lista_describe); // falta esta funcion?

			list_destroy_and_destroy_elements(lista_describe, liberar_metadata); //falta el liberar metadata?

			break;
		case DROP:

			printf(">>DROP<<\n");

			if(obtener_parametros_drop(linea_request , nombre_tabla)){
				printf("entre al if, nombre : %s\n" , nombre_tabla);

				Drop drop_dato = crear_drop(nombre_tabla);

				request_drop(drop_dato);

				return true;
			}


			break;


		case EXIT:


		default:

			//log
			printf("no reconoci la request\n");

	}

	return fin_funcion;

}


int obtener_parametros_select(char* linea_request, char* nombre_tabla, u_int16_t* key){

	char* funcion = string_new();

	int cantidad_parametros;

	cantidad_parametros = sscanf(linea_request, "%s %s %d", funcion, nombre_tabla, key);

	if( cantidad_parametros != 3){

//		log_error(logger_kernel, "-LA REQUEST SELECT RECIBIO PARAMETROS INCORRECTOS.-\n");
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

	if(sscanf(auxiliar[0] , "%s %s %i" , funcion, nombre_tabla, key) != 3){

//		log_error(logger_kernel, "-LA REQUEST INSERT RECIBIO PARAMETROS INCORRECTOS DE NOMBRE Y KEY.-\n");
		return 0;
	}



	string_append(value , auxiliar[1]);

	if(auxiliar[2] == NULL){

		timestamp = -1;

		return 1;

	}else if( (sscanf(auxiliar[2] , " %d" , timestamp)) != 1 ){

//		log_error(logger_kernel, "-LA REQUEST INSERT RECIBIO PARAMETROS INCORRECTOS DE TIMESTAMP.-\n");
		return 0;

	}

	string_to_upper(nombre_tabla);

	free(auxiliar);
	//free(comillas);
	free(funcion);
	return 1;

}

int obtener_parametros_insert_sin_timestamp(char* linea_request, char* nombre_tabla, u_int16_t* key, char* value){
	char* funcion = string_new();

	if( (sscanf(linea_request, "%s %s %i %s %i", funcion, nombre_tabla, key, value)) != 5 ){

		//log_error(logger_kernel, "-LA REQUEST INSERT RECIBIO PARAMETROS INCORRECTOS.-\n");
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
		//log_error( logger_kernel , "-LA REQUEST CREATE RECIBIO PARAMETROS INCORRECTOS.-\n");
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
//		log_error( logger_kernel , "-LA REQUEST DESCRIBE RECIBIO PARAMETROS INCORRECTOS.-\n");
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

void menu(){

	printf("\n################################################################################\n");
	printf("\n1. SELECT [NOMBRE TABLA] [KEY]\n");
	printf("2. INSERT [NOMBRE TABLA] [KEY] [VALUE] [TIMESTAMP]\n");
	printf("3. CREATE [NOMBRE TABLA] [CONSISTENCIA] [NUMERO PARTICIONES] [TIEMPO COMPACTACION]\n");
	printf("4. ADD MEMORY [NUMERO MEMORIA] TO [CRITERIO]\n");
	printf("5. RUN [ARCHIVO LQL]\n");
	printf("Escriba una request: ");
	printf("\n");
}

