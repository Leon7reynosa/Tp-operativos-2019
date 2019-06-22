/*
 * requestGossiping.c
 *
 *  Created on: 21 jun. 2019
 *      Author: utnso
 */


#include"requestGossiping.h"

memoria_dto crear_memoria_dto(int numero, char* ip, int puerto){

	memoria_dto memory = malloc(sizeof(memoria_dto));

	memory->ip = malloc(sizeof(t_stream));
	memory->ip->size = string_length(ip) + 1;
	memory->ip->buffer = malloc(memory->ip->size);
	memcpy(memory->ip->buffer, ip, memory->ip->size);

	memory->numero_memoria = numero;
	memory->puerto = puerto;

	return memory;

}

void eliminar_memoria_dto(memoria_dto memory){

	free(memory->ip->buffer);
	free(memory->ip);
	free(memory);

}

tabla_gossip_dto crear_dto_gossip(int cant_memorias){

	tabla_gossip_dto tabla = malloc(sizeof(tabla_gossip_dto));

	tabla->cant_memorias = cant_memorias;
	tabla->memorias = list_create();

	tabla->bytes = sizeof(tabla->cant_memorias);

	return tabla;
}

void agregar_memoria_gossip_dto(tabla_gossip_dto tabla, memoria_dto memoria_nueva){

	list_add(tabla->memorias, memoria_nueva);

	tabla->bytes += sizeof(memoria_nueva->ip->size) + memoria_nueva->ip->size + sizeof(memoria_nueva->numero_memoria) + sizeof(memoria_nueva->puerto);


}

void liberar_dto_gossip(tabla_gossip_dto dato){


	list_destroy_and_destroy_elements(dato->memorias, eliminar_memoria_dto);

	free(dato);

}

tabla_gossip_dto decodificar_gossiping(conexion){

	int cant_memorias;
	int error_recv;

	int numero_memoria;
	int size_ip;
	char* ip;
	int puerto;


	error_recv = recv(conexion, &cant_memorias, sizeof(int), 0);

	if(error_recv <= 0 ){
		//error
	}

	tabla_gossip_dto tabla_gossip = crear_dto_gossip(cant_memorias);
	memoria_dto memory_dto;

	int i;

	for(i = 0; i < cant_memorias; i++){

		error_recv = recv(conexion, &numero_memoria ,sizeof(int), 0 );

		if(error_recv <= 0 ){
			//error
		}

		error_recv = recv(conexion, &size_ip, sizeof(int), 0 );

		if(error_recv <= 0 ){
			//error
		}

		ip = malloc(size_ip);

		error_recv = recv(conexion, ip, size_ip, 0);

		if(error_recv <= 0 ){
			//error
		}

		error_recv = recv(conexion, &puerto, sizeof(int), 0);

		if(error_recv <= 0 ){
			//error
		}

		memory_dto = crear_memoria_dto(numero_memoria, ip, puerto);

		agregar_memoria_gossip_dto(tabla_gossip, memory_dto);

		free(ip);

	}

	return tabla_gossip;

}
