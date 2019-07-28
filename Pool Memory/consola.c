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

	free(buffer);

	string_to_upper(tokens[0]);

	if(string_equals_ignore_case(tokens[0], "SELECT")){
		printf(">>>>>SELECT<<<<<\n");
		parsear_request(SELECT, tokens);
		printf("========================\n");

	}
	else if(string_equals_ignore_case(tokens[0], "INSERT")){
		printf(">>>>>INSERT<<<<<\n")
		parsear_request(INSERT, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "CREATE")){
		printf(">>>>>CREATE<<<<<\n")
		parsear_request(CREATE, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "DESCRIBE")){
		printf(">>>>>DESCRIBE<<<<<\n")
		parsear_request(DESCRIBE, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "DROP")){
		printf(">>>>>DROP<<<<<\n")
		parsear_request(DROP, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "JOURNAL")){
		printf(">>>>>JOURNAL<<<<<\n")
		//parsear_request(JOURNAL, tokens);
		printf("========================\n");
	}
	else if(string_equals_ignore_case(tokens[0], "EXIT")){
		printf(">>>>>EXIT<<<<<\n")
		liberar_puntero_doble(tokens);

		desconexion_pool = true;

	}else{
		printf("No es valido lo ingresado\n");
	}

	liberar_puntero_doble(tokens);


	return false;
}

void parsear_request(cod_operacion operacion, char** tokens){

	int cantidad_argumentos = obtener_cantidad_argumentos(tokens);

	char* tabla;
	u_int16_t key;
	char* value;
	time_t timestamp;
	char* consistencia;
	int tiempo_compactacion;
	int numero_particiones;
	Dato dato;
	estado_request estado;


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

				liberar_dato_select(dato_select); //agregado

				liberar_dato(dato);

			}else{

				printf("Cantidad de argumentos invalido\n");

			}

			break;

		case INSERT:

			if(cantidad_argumentos == 4 || cantidad_argumentos == 5){
					tabla = tokens[1];
					key = atoi(tokens[2]);
					value = tokens[3];
					timestamp = atoi(tokens[4]);

					dato_insert = crear_dato_insert(tabla, key, value, timestamp);

					request_insert(dato_insert);

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

				liberar_dato_describe(dato_describe);

				mostrar_lista_describe(list_describe);

				//eliminar la lista ?

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

			if(cantidad_argumentos == 2){

			}
			else{
				printf("Cantidad de argumentos invalido\n");
			}

			break;

		default:
			break;


	}


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
