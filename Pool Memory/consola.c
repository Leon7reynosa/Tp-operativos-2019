/*
 * consola.c
 *
 *  Created on: 20 jul. 2019
 *      Author: utnso
 */

#include"consola.h"


bool leer_consola(void){

	int tamanio_buffer = 128;

	char* buffer = malloc(tamanio_buffer + 1);

	fgets(buffer, tamanio_buffer, stdin);

	*(buffer + tamanio_buffer) = '\0';

	if(string_length(buffer) + 1 >= tamanio_buffer){
		printf("Pusiste 200 millones de caracteres en la consola\n");
	}

	buffer = realloc(buffer, string_length(buffer) + 1);

	char** tokens = string_split(buffer, " ");

	string_to_upper(tokens[0]);

	if(string_equals_ignore_case(tokens[0], "SELECT")){
		printf(">>>>>SELECT<<<<<\n");
		parsear_request(SELECT, tokens);
		printf("========================\n");

	}
	else if(string_equals_ignore_case(tokens[0], "INSERT")){
		printf(">>>>>INSERT<<<<<\n");
		parsear_request(INSERT, &buffer);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "CREATE")){
		printf(">>>>>CREATE<<<<<\n");
		parsear_request(CREATE, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "DESCRIBE")){
		printf(">>>>>DESCRIBE<<<<<\n");
		parsear_request(DESCRIBE, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "DROP")){
		printf(">>>>>DROP<<<<<\n");
		parsear_request(DROP, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "JOURNAL")){
		printf(">>>>>JOURNAL<<<<<\n");
		parsear_request(JOURNAL, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "EXIT")){

		printf(">>>>>EXIT<<<<<\n");

		desconexion_pool = true;

	}else{
		printf("No es valido lo ingresado\n");
	}

	free(buffer);

	liberar_puntero_doble(tokens);


	return false;
}

void parsear_request(cod_operacion operacion, char** tokens){

	int cantidad_argumentos;

	char* tabla;
	u_int16_t key;
	char* value;
	time_t timestamp;
	char* consistencia;
	int tiempo_compactacion;
	int numero_particiones;
	Dato dato;
	estado_request estado;

	if(operacion != INSERT){

		cantidad_argumentos = obtener_cantidad_argumentos(tokens);
	}else{

		cantidad_argumentos = obtener_parametros_insert(*tokens , &tabla , &key , &value , &timestamp);

	}

	select_t dato_select;

	insert dato_insert;

	describe_t dato_describe;
	t_list* list_describe;

	create dato_create;

	Drop dato_drop;

	switch(operacion){

		case SELECT:

			if(cantidad_argumentos == 2){

				tabla = tokens[1];
				key = atoi(tokens[2]);       //VER ESTO

				dato_select = crear_dato_select(tabla, key);

				dato = request_select(dato_select);

				if(dato == NULL){

					printf("No se encontro la key solicitada\n");
					liberar_dato_select(dato_select);
					return;
				}

				printf("Dato: \n");
				printf(">Key = %i\n", dato->key);
				printf(">Value = %s\n", dato->value);
				printf(">Timestamp = %i\n", dato->timestamp);

				liberar_dato_select(dato_select); //agregado

				liberar_dato(dato);

			}else{

				printf("Cantidad de argumentos invalido\n");

			}

			break;

		case INSERT:

			if(cantidad_argumentos ){

					dato_insert = crear_dato_insert(tabla, key, value, timestamp);

					estado = request_insert(dato_insert);

					mostrar_terminacion_request_segun_estado(estado);

					//liberar parametros?

					liberar_dato_insert(dato_insert);

			}

			else{
				printf("Cantidad de argumentos invalido\n");
			}

			break;

		case CREATE:

			if(cantidad_argumentos == 5){
				tabla = tokens[1];
				consistencia = tokens[2];
				numero_particiones = atoi(tokens[3]);
				tiempo_compactacion = atoi(tokens[4]);

				dato_create = crear_dato_create(tabla, consistencia, numero_particiones, tiempo_compactacion);

				estado = request_create(dato_describe);

				mostrar_terminacion_request_segun_estado(estado);

				liberar_dato_create(dato_create);


			}
			else{
				printf("Cantidad de argumentos invalido\n");
			}

			break;

		case DESCRIBE:

			if(cantidad_argumentos == 1 || cantidad_argumentos == 2){

				tabla = tokens[1];

				dato_describe = crear_dato_describe(tabla);

				list_describe = request_describe(dato_describe);

				if(list_describe == NULL){

					printf("Fallo el describe\n");
					liberar_dato_describe(dato_describe);

					return;

				}

				liberar_dato_describe(dato_describe);

				mostrar_lista_describe(list_describe);

				list_destroy_and_destroy_elements(list_describe, liberar_dato_describe);

			}
			else{
				printf("Cantidad de argumentos invalido\n");
			}

			break;

		case DROP:

			if(cantidad_argumentos == 2){
				tabla = tokens[1];

				dato_drop = crear_drop(tabla);

				estado = request_drop(dato_drop);

				mostrar_terminacion_request_segun_estado(estado);

				liberar_drop(dato_drop);

			}
			else{
				printf("Cantidad de argumentos invalido\n");
			}

			break;

		case JOURNAL:

			if(cantidad_argumentos == 1){

				pthread_mutex_lock(&mutex_journal);

				realizar_journal();

				pthread_mutex_lock(&mutex_journal);

				printf("Se realizo el Journal\n");

			}
			else{
				printf("Cantidad de argumentos invalido\n");
			}

			break;

		default:
			break;


	}


}

int obtener_parametros_insert(char* linea_request, char** nombre_tabla, u_int16_t* key, char** value, time_t* timestamp ){

	char** auxiliar;
	char** parametros;
	char* comillas = "\"";

	auxiliar = string_split(linea_request, comillas  );

	for(int i = 0; i < 2 ; i++){

		if( auxiliar[i] == NULL){

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

			printf("\n>La REQUEST INSERT recibio parametros incorrectos\n");

			liberar_puntero_doble(parametros);

			liberar_puntero_doble(auxiliar);

			free(value);

			return 0;

		}

	}

	if(parametros[3] != NULL){

		printf("\n>La REQUEST INSERT recibio parametros incorrectos\n");

		liberar_puntero_doble(parametros);

		liberar_puntero_doble(auxiliar);

		free(value);

		return 0 ;

	}

	*nombre_tabla = malloc(strlen(parametros[1] ) + 1);
	memcpy(*nombre_tabla , parametros[1] , strlen(parametros[1]) + 1);

	*key = atoi(parametros[2]);

	if(auxiliar[2] == NULL){

		*timestamp = -1;

	}else{

		*timestamp = atoi(auxiliar[2]);

	}

	printf("HASTA ACA LA TABLA ES: %s\n" , *nombre_tabla);
	printf("Y LA KEY ES: %d\n"  ,  *key);


	liberar_puntero_doble(parametros);

	liberar_puntero_doble(auxiliar);

	return 1;

}


void liberar_puntero_doble(char** puntero_doble){

	int i = 0;
	while(*(puntero_doble + i) != NULL){

		free(*(puntero_doble + i));
		i++;

	}

	free(puntero_doble);

}

int obtener_cantidad_argumentos(char** tokens){

	int i = 1;
	int cantidad = 0;

	while(*(tokens + i) != NULL){
		cantidad++;
		i++;
	}

	return cantidad;
}

void mostrar_terminacion_request_segun_estado(estado_request estado){
	if(estado == SUCCESS){
		printf("==== TERMINO CORRECTAMENTE LA REQUEST ====\n");
	}
	else{
		printf("==== ERROR AL REALIZAR LA REQUEST ====\n");
	}
}
