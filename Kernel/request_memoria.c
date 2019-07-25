/*
 * request_memoria.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */
#include"request_memoria.h"

void request_add(int numero_memoria, char* consistencia){

	cod_consistencia codigo = identificar_consistencia(consistencia);


	memoria_t* memoria_agregar = obtener_memoria_de_lista(numero_memoria);

	if(	memoria_agregar == NULL	){

		printf("NO SE PODRA INGRESAR LA MEMORIA, TE TIRO UN EXIT\n");

		exit(1);

	}

	switch(codigo){

		case SC:

			agregar_a_consistencia(SC , memoria_agregar);
			break;

		case EC:

			agregar_a_consistencia(EC , memoria_agregar);
			break;

		case SHC:
			agregar_a_consistencia(SHC , memoria_agregar);
			break;

		default:
			//log de error
			printf("FALLO\n");

			exit(1);

	}

	printf("\nSE INGRESO CORRECTAMENTE LA MEMORIA\n");

}


memoria_t* obtener_memoria_de_lista( int numero_memoria ){

	struct MemoriasEstructura* dato_memoria_gossiping;

	pthread_rwlock_rdlock(&semaforo_tabla_gossiping);

	int tamanio_lista_gossip = list_size(tabla_gossiping) ;

	pthread_rwlock_unlock(&semaforo_tabla_gossiping);

	for( int i = 0 ; i < tamanio_lista_gossip ; i++){

		pthread_rwlock_rdlock(&semaforo_tabla_gossiping);

		dato_memoria_gossiping = list_get(tabla_gossiping , i);

		pthread_rwlock_unlock(&semaforo_tabla_gossiping);

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

