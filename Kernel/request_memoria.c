/*
 * request_memoria.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */
#include"request_memoria.h"

void request_add(int numero_memoria, char* consistencia){

	cod_consistencia codigo = identificar_consistencia(consistencia);


	memoria_t* memoria_agregar = list_get(tabla_gossiping, numero_memoria - 1 );

	switch(codigo){

		case SC:

			agregar_a_consistencia(Strong_C , &memoria_agregar->socket);
			break;

		case EC:

			agregar_a_consistencia(Eventual_C , &memoria_agregar->socket);
			break;

		case SHC:
			agregar_a_consistencia(Strong_Hash_C , &memoria_agregar->socket);
			break;

		default:
			//log de error
			printf("FALLO\n");

	}

}
