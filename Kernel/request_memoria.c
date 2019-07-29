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

		log_error(logger_kernel, "-NO se podra ingresar la MEMORIA.-");

		printf("\nNO se podra INGRESAR la MEMORIA\n");

		return;

	}

	pthread_rwlock_wrlock(&memoria_agregar->semaforo_memoria);

	if( !memoria_agregar->conectado){

		memoria_agregar->socket = conectar_servidor(memoria_agregar->ip , memoria_agregar->puerto);

		if(memoria_agregar->socket < 0 ){

			memoria_agregar->conectado = false;

			log_error(logger_kernel , "-La MEMORIA no esta conectada, NO la agregamos a la consistencia.-");

			printf("\n>La MEMORIA no esta conectada\n");

			pthread_rwlock_unlock(&memoria_agregar->semaforo_memoria);

			return ;

		}else{

			memoria_agregar->conectado = true;


		}


	}

	pthread_rwlock_unlock(&memoria_agregar->semaforo_memoria);

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

	log_info(logger_kernel, "-Se ingreso la MEMORIA CORRECTAMENTE.-");

	printf("\n>Se ingreso la MEMORIA CORRECTAMENTE\n");

}


memoria_t* obtener_memoria_de_lista( int numero_memoria ){

	struct MemoriasEstructura* dato_memoria_gossiping;

	pthread_rwlock_rdlock(&semaforo_tabla_gossiping);

	int tamanio_lista_gossip = list_size(tabla_gossiping) ;

	for( int i = 0 ; i < tamanio_lista_gossip ; i++){

		dato_memoria_gossiping = list_get(tabla_gossiping , i);

		if(dato_memoria_gossiping->numero_memoria == numero_memoria){

			memoria_t* memoria_encontrada = dato_memoria_gossiping; //si rompe todo es por esto

			pthread_rwlock_unlock(&semaforo_tabla_gossiping);

			return memoria_encontrada;

		}

	}

	pthread_rwlock_unlock(&semaforo_tabla_gossiping);

	printf("\n>NO se encuentra la memoria %d en tabla de gossip\n", numero_memoria);

	log_error(logger_kernel , "-NO se encuentra la MEMORIA en la tabla gossip.-");

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

