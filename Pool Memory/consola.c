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

		parsear_request(SELECT, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "INSERT")){

		parsear_request(INSERT, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "CREATE")){

		parsear_request(CREATE, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "DESCRIBE")){

		parsear_request(DESCRIBE, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "DROP")){

		//parsear_request(DROP, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "JOURNAL")){

		//parsear_request(JOURNAL, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "EXIT")){

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
	time_t timestamp;

	select_t dato_select;
	Dato dato;

	insert dato_insert;


	switch(operacion){

		case SELECT:

			if(cantidad_argumentos == 2){

				tabla = tokens[1];
				key = atoi(tokens[2]);       //VER ESTO

				dato_select = crear_dato_select(tabla, key);

				dato = request_select(dato_select);

				liberar_dato(dato);

			}else{

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
