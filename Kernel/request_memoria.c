/*
 * request_memoria.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */
#include"request_memoria.h"

void request_add(int numero_memoria, char* consistencia){

	printf("consistencia de la request: %s\n" , consistencia);

	cod_consistencia codigo = identificar_consistencia(consistencia);

	printf("el codigo es: %d\n" , codigo);


	memoria_t* memoria_agregar = obtener_memoria_de_lista(numero_memoria);

	printf("bueno hasat aca llegamos\n");

	if(	memoria_agregar == NULL	){

		printf("NO SE PODRA INGRESAR LA MEMORIA, TE TIRO UN EXIT\n");

		exit(1);

	}

	printf("casi llego al swithc :D");

	switch(codigo){

		case SC:

			agregar_a_consistencia(Strong_C , memoria_agregar);
			break;

		case EC:

			agregar_a_consistencia(Eventual_C , memoria_agregar);
			break;

		case SHC:
			agregar_a_consistencia(Strong_Hash_C , memoria_agregar);
			break;

		default:
			//log de error
			printf("FALLO\n");

			exit(1);

	}

	printf("SE INGRESO CORRECTAMENTE LA MEMORIA\n");

}


memoria_t* obtener_memoria_de_lista( int numero_memoria ){

	struct MemoriasEstructura* dato_memoria_gossiping;

	for( int i = 0 ; i < list_size(tabla_gossiping) ; i++){

		dato_memoria_gossiping = list_get(tabla_gossiping , i);

		if(dato_memoria_gossiping->numero_memoria == numero_memoria){

			memoria_t* memoria_encontrada =
					crear_dato_memoria_consistencia(dato_memoria_gossiping->ip, dato_memoria_gossiping->numero_memoria, dato_memoria_gossiping->puerto);

			return memoria_encontrada;

		}

	}

	printf("NO se encuentra la memoria %d en tabla de gossip\n", numero_memoria);

	return NULL;

}

memoria_t* crear_dato_memoria_consistencia(char* ip , int numero_memoria, int puerto){


	memoria_t* memoria_creada = malloc(sizeof(memoria_t));

	memoria_creada->ip = malloc(strlen(ip) + 1);

	memcpy(memoria_creada->ip , ip, strlen(ip) + 1);

	memoria_creada->numero_memoria = numero_memoria;

	memoria_creada->puerto = puerto;

	memoria_creada->socket = conectar_servidor(ip, puerto);

	if( memoria_creada->socket < 0 ){

		printf("NO SE PUDO CONECTAR A LA MEMORIA %i\n" , numero_memoria);

		return NULL;

	}


	return memoria_creada;


}

