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

	dato_t* nuevo = (dato_t*) malloc(sizeof(dato_t));

	nuevo->key = key;
	nuevo->value = malloc(strlen(valor) + 1);
	memcpy(nuevo->value, valor, strlen(valor) + 1);
	nuevo->timestamp = tiempo;

	return nuevo;
}

dato_t* timestamp_mas_grande(dato_t* primer_dato , dato_t* segundo_dato){
	//solo el primer dato puede ser null

	if(primer_dato == NULL || segundo_dato->timestamp > primer_dato->timestamp){

		return segundo_dato;
	}else {
		return primer_dato;
	}
}

