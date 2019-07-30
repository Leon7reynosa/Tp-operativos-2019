/*
 * consola.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include"consola.h"

void* consola(void* argumento){

	char* leido;

	char* string_codigo = string_new();

	cod_operacion codigo;

	menu();

	leido = readline("");

	printf("Leido : %s\n" , leido);

	while(!string_equals_ignore_case(leido, "exit")){

		codigo = identificar_request(leido);

		string_codigo = obtener_string_codigo(codigo);

		free(string_codigo);

		ejecutar_request(codigo, leido);

		free(leido);

		leido = readline("");

	}

	free(leido);

	pthread_exit(NULL);

	return NULL;
}


char* obtener_string_codigo(cod_operacion codigo){
	char* string_codigo = string_new();

	switch(codigo){
		case SELECT:
			string_codigo = string_duplicate("SELECT");
			break;
		case INSERT:
			string_codigo = string_duplicate("INSERT");
			break;
		case CREATE:
			string_codigo = string_duplicate("CREATE");
			break;
		case DESCRIBE:
			string_codigo = string_duplicate("DESCRIBE");
			break;
		case DROP:
			string_codigo = string_duplicate("DROP");
			break;
		case DESCONEXION:
			string_codigo = string_duplicate("DESCONEXION");
			break;
		case EXIT:
			string_codigo = string_duplicate("EXIT");
			break;
		default:
			string_codigo = string_duplicate("REQUEST INVALIDA");
			break;

	}

	return string_codigo;
}


bool ejecutar_request(cod_operacion codigo_request , char* linea_request){

	printf("\n/////////////////////////REQUEST NUEVA DE CONSOLA////////////////////////////\n");

	printf("\nREQUEST: %s\n" , linea_request);

	request nueva_request;
	bool fin_funcion = false;

	int cantidad_parametros;

	void* tipo_request;

	char*nombre_tabla;
	char* value ;
	u_int16_t key;
	time_t timestamp;

	char* consistencia;
	int particiones;
	int compactacion;

	t_list* lista_describe;

	switch( codigo_request ){

		case SELECT:

			printf("\n>>SELECT<<\n");

			if(obtener_parametros_select(linea_request, &nombre_tabla, &key)){


				select_t dato_select = crear_dato_select(nombre_tabla, key);

				dato_t* dato = request_select(dato_select);



				if(dato != NULL){

					mostrar_dato(dato);

					liberar_dato(dato);

				}else{

					printf("\n>NO se encontro el dato\n");

				}






				liberar_dato_select(dato_select);

				fin_funcion = true;
			}

			break;
		case INSERT:

			printf("\n>>INSERT<<\n");

			if( obtener_parametros_insert(linea_request, &nombre_tabla, &key , &value, &timestamp) ){

				if(timestamp < 0 ){

					timestamp = time(NULL);

				}

//			printf("NOMBRE TABLA: %s\n" , nombre_tabla);
//			printf("KEY: %d\n" , key);
//			printf("VALUE : %s\n" , value);

				insert dato_insert = crear_dato_insert(nombre_tabla, key,value, timestamp  );

				request_insert(dato_insert);

				liberar_dato_insert(dato_insert);

				fin_funcion = true;

			}

			break;
		case CREATE:

			printf("\n>>CREATE<<\n");

			if(obtener_parametros_create(linea_request, &nombre_tabla, &consistencia, &particiones, &compactacion)){

				create dato_create = crear_dato_create(nombre_tabla, consistencia, particiones, compactacion);

				request_create(dato_create);

				liberar_dato_create(dato_create);

				fin_funcion = true;
			}

			break;
		case DESCRIBE:

			printf("\n>>DESCRIBE<<\n");

			cantidad_parametros = obtener_parametros_describe(linea_request, &nombre_tabla);

			if(cantidad_parametros == 2){

				//log_info(logger_kernel, "---Se realizara el DESCRIBE para la tabla %s---\n" , nombre_tabla);

				describe_t describe_enviar = crear_dato_describe(nombre_tabla);

				lista_describe = request_describe(describe_enviar);

				mostrar_lista_describe(lista_describe);

				liberar_dato_describe(describe_enviar);

			}else if(cantidad_parametros == 1){

				//log_info(logger_kernel, "---SE REALIZARA UN DESCRIBE GLOBAL--")

				describe_t describe_enviar = crear_dato_describe(NULL);

				lista_describe = request_describe(describe_enviar);

				mostrar_lista_describe(lista_describe);

				liberar_dato_describe(describe_enviar);

			}else{

				fin_funcion = false;
			}

			if(lista_describe != NULL){

				list_destroy_and_destroy_elements(lista_describe, liberar_metadata); //falta el liberar metadata?

			}

			break;
		case DROP:

			printf("\n>>DROP<<\n");

			if(obtener_parametros_drop(linea_request , &nombre_tabla)){

				Drop drop_dato = crear_drop(nombre_tabla);

				request_drop(drop_dato);

				fin_funcion = true;
			}


			break;


		case EXIT:


		default:

			//log
			printf("no reconoci la request\n");

	}

	printf("\n////////////////////////////FIN REQUEST CONSOLA////////////////////////\n\n");

	return fin_funcion;

}


int obtener_parametros_select(char* linea_request, char** nombre_tabla , u_int16_t* key){

	char** parametros = string_split(linea_request , " ");


	for(int i = 0; i < 3 ; i++){

		if(parametros[i] == NULL){

			log_error(logger_request, "-la REQUEST SELECT recibio parametros incorrectos.-");

			int obtener_parametros_drop(char* linea_request, char* nombre_tabla){

				char* funcion = string_new();

				int cantidad_parametros;

				cantidad_parametros = sscanf(linea_request, "%s %s", funcion, nombre_tabla);

				string_to_upper(nombre_tabla);

				return cantidad_parametros;

			}			printf("\n>La REQUEST SELECT recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;
		}

	}

	if( parametros[3] != NULL){

		log_error(logger_request, "-la REQUEST SELECT recibio parametros de mas.-");

		printf("\n>La REQUEST SELECT recibio parametros de mas\n");

		liberar_puntero_doble(parametros);

		return 0;

	}

	string_to_upper(parametros[1]);

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

	auxiliar = string_split(linea_request, comillas);

	for(int i = 0; i < 2 ; i++){

		if( auxiliar[i] == NULL){

			log_error(logger_request, "-la REQUEST INSERT recibio parametros incorrectos.-");

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

			log_error(logger_request, "-la REQUEST INSERT recibio parametros incorrecto.-");

			printf("\n>La REQUEST INSERT recibio parametros incorrecto\n");

			liberar_puntero_doble(parametros);

			liberar_puntero_doble(auxiliar);

			free(value);

			return 0;

		}

	}

	if(parametros[3] != NULL){

		log_error(logger_request, "-la REQUEST INSERT recibio parametros de mas.-");

		printf("\n>La REQUEST INSERT recibio parametros de mas\n");

		liberar_puntero_doble(parametros);

		liberar_puntero_doble(auxiliar);

		free(value);

		return 0 ;

	}

	string_to_upper(parametros[1]);

	*nombre_tabla = malloc(strlen(parametros[1] ) + 1);
	memcpy(*nombre_tabla , parametros[1] , strlen(parametros[1]) + 1);

	*key = atoi(parametros[2]);

	if(auxiliar[2] == NULL){

		*timestamp = -1;

	}else{

		*timestamp = atoi(auxiliar[2]);

	}

	//printf("EN EL OBTENER PARAMETROS LA TABLA ES %s\n" , *nombre_tabla);

	liberar_puntero_doble(parametros);

	liberar_puntero_doble(auxiliar);

	return 1;

}


int obtener_parametros_create(char* linea_request, char** nombre_tabla, char** criterio, int* numero_particiones, int* tiempo_compactacion){

	char** parametros = string_split(linea_request, " ");

	for(int  i =  0 ; i < 5 ; i++){

		if(parametros[i] == NULL){

			log_error(logger_request, "-la REQUEST CREATE recibio parametros incorrectos.-");

			printf("\n>La REQUEST CREATE recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;


		}

	}

	if ( parametros[5] != NULL ){

		log_error(logger_request, "-la REQUEST CREATE recibio parametros de mas.-");

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

		log_error(logger_request, "-la REQUEST DESCRIBE recibio parametros incorrectos.-");

		printf("\n>La REQUEST DESCRIBE recibio parametros incorrectos\n");

		return 0;

	}

	if(parametros[1] == NULL ){


		liberar_puntero_doble(parametros);

		return 1;
	}else{

		if(parametros[2] != NULL ){

			log_error(logger_request, "-la REQUEST DESCRIBE recibio parametros incorrectos.-");

			printf("\n>La REQUEST DESCRIBE recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;
		}else{

			string_to_upper(parametros[1]);

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

			log_error(logger_request, "-la REQUEST DROP recibio parametros incorrectos.-");

			printf("\n>La REQUEST DROP recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			return 0;

		}

	}

	if(parametros[2] != NULL){

		log_error(logger_request, "-la REQUEST DROP recibio parametros de mas.-");

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

void menu(){

	printf("\n################################################################################\n");
	printf("\n1. SELECT [NOMBRE TABLA] [KEY]\n");
	printf("2. INSERT [NOMBRE TABLA] [KEY] [VALUE] [TIMESTAMP]\n");
	printf("3. CREATE [NOMBRE TABLA] [CONSISTENCIA] [NUMERO PARTICIONES] [TIEMPO COMPACTACION]\n");
	printf("4. DESCRIBE [NOMBRE TABLA]\n");
	printf("4. DROP [NOMBRE TABLA]\n");
	printf("Escriba una request: ");
	printf("\n");
}

