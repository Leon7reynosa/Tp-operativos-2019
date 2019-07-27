/*
 * comunity_func.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include "comunity_func.h"

int calcular_particion(int particion_metadata ,u_int16_t key){

	return key % particion_metadata;

}

dato_t* crear_dato(u_int16_t key, char* valor, time_t tiempo){

	dato_t* nuevo = malloc(sizeof(dato_t));

	printf("[CREAR DATO] value que ingreso: %s\n" , valor);

	nuevo->key = key;
	nuevo->value = malloc(string_length(valor) + 1);
	memcpy(nuevo->value, valor, string_length(valor) + 1);
	nuevo->timestamp = tiempo;

	printf("[CREAR DATO] Se ingreso el value: %s\n" , nuevo->value);

	return nuevo;
}

void liberar_dato(dato_t* dato_remove){

	printf("[LIBERACION] Voy a liberar un dato con el value : %s\n" , dato_remove->value);

	free(dato_remove->value);
	printf("[LIBERACION] ya libere el value ");

	free(dato_remove);

	printf("[LIBERACION] Se libero un dato\n");

}

dato_t* timestamp_mas_grande(dato_t* primer_dato , dato_t* segundo_dato){
	//solo el primer dato puede ser null

	dato_t* dato_mas_reciente = NULL;

	if(primer_dato != NULL){

		printf("[A. TIMESTAMP] El primer dato no es null\n");

		if(segundo_dato != NULL){

			printf("[A. TIMESTAMP] El segundo dato no es null\n");

			if(primer_dato->timestamp >= segundo_dato->timestamp){
				printf("[A. TIMESTAMP] El primer dato es mas grande\n");
				dato_mas_reciente = crear_dato(primer_dato->key, primer_dato->value, primer_dato->timestamp);

			}else{
				printf("[A. TIMESTAMP] El segundo es mas grande\n");
				dato_mas_reciente = crear_dato(segundo_dato->key, segundo_dato->value, segundo_dato->timestamp);

			}

		}else{
			printf("[A. TIMESTAMP] El segundo dato es null, queda el primero\n");
			dato_mas_reciente = crear_dato(primer_dato->key, primer_dato->value, primer_dato->timestamp);
			printf("[A. TIMESTAMP] El segundo no es null\n");
			printf("[A. TIMESTAMP] KEY: %i\n", primer_dato->key);
			printf("[A. TIMESTAMP] TIME: %i\n", primer_dato->timestamp);
			printf("[A. TIMESTAMP] VALUE: %s\n", primer_dato->value);

		}

	}else if(segundo_dato != NULL){

		printf("[A. TIMESTAMP] Queda el segundo\n");
		printf("[A. TIMESTAMP] El segundo no es null\n");
		printf("[A. TIMESTAMP] KEY: %i\n", segundo_dato->key);
		printf("[A. TIMESTAMP] TIME: %i\n", segundo_dato->timestamp);
		printf("[A. TIMESTAMP] VALUE: %s\n", segundo_dato->value);
		dato_mas_reciente = crear_dato(segundo_dato->key, segundo_dato->value, segundo_dato->timestamp);
		printf("[REQUEST] Termine de crear el dato\n");

	}

	return dato_mas_reciente;
}


void liberar_puntero_doble(char** puntero){

	int i = 0;
	while(*(puntero + i) != NULL){
		i++;
	}

	for(; i > 0 ; i--){
		free(*(puntero + i));
	}
	free(puntero);

}

dato_t* convertir_a_dato(char* dato_encontrado){

	char** partes_dato = string_split(dato_encontrado, ";");

	time_t dato_timestamp = atoi(partes_dato[0]);
	u_int16_t dato_key = atoi( partes_dato[1] );

	dato_t* dato_return = crear_dato(dato_key , partes_dato[2] ,  dato_timestamp);

	liberar_puntero_doble(partes_dato);

	return dato_return;

}

