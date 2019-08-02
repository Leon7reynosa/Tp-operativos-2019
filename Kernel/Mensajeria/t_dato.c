/*
 * t_dato.c
 *
 *  Created on: 23 jun. 2019
 *      Author: utnso
 */

#include"t_dato.h"

t_dato* crear_t_dato(u_int16_t key, time_t timestamp , char* value){

	t_dato* dato_creado = malloc(sizeof(t_dato));

	dato_creado->key = key;

	dato_creado->timestamp = timestamp;

	dato_creado->value = malloc(sizeof(t_stream));

	dato_creado->value->size = strlen(value) + 1;
	dato_creado->value->buffer = malloc(dato_creado->value->size);
	memcpy(dato_creado->value->buffer , value , dato_creado->value->size);

	return dato_creado;

}

bool es_un_numero(char* numero_principal){

	int i = 0;



	if(numero_principal == NULL){

		return false;
	}

	if(string_equals_ignore_case(numero_principal , "\n") || string_is_empty(numero_principal)){

		return false ;

	}

	char* numero = malloc(strlen(numero_principal) + 1);
	memcpy(numero, numero_principal , strlen(numero_principal ) +1);

	string_trim(&numero);

	while( numero[i] != '\0' ){

		if(numero[i] == '\n'){
		}

		if( !isdigit(numero[i]) ){

			free(numero);

			return false;

		}

		i++;
	}

	free(numero);

	return true;
}


void liberar_t_dato(t_dato* dato){

	free(dato->value->buffer);
	free(dato->value);
	free(dato);

}

void mostrar_t_dato(t_dato* dato_a_mostrar){

	printf("\nDato Recibido: \n");
	printf("Key: %d\n" , dato_a_mostrar->key);
	printf("Value: %s\n" , (char*) dato_a_mostrar->value->buffer);
	printf("Timestamp: %d\n\n" , dato_a_mostrar->timestamp);


}
