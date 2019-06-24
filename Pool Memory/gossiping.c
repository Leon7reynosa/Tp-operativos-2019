/*
 * gossiping.c
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#include"gossiping.h"

void* gossiping(){

	int cantidad_seeds = list_size(memoria->seed);
	int i ;

	int socket_seed;
	Seed seed_aux;

	for(i = 0 ; i < cantidad_seeds ; i++){

		seed_aux = list_get(memoria->seed, i);

		socket_seed = conectar_servidor(seed_aux->ip, seed_aux->puerto);

		if(socket_seed >= 0){

			intercambiar_datos(memoria, socket_seed);

			close(socket_seed);
		}
		else{
			//nada, proba otro dia crack! (LOGGEAR que la memoria seed no esta conectada)
		}

	}


	return NULL;
}

void* serializar_gossiping(struct DatoTablaGossiping* dato){

	void* buffer_serializado = malloc(dato->bytes);
	int desplazamiento = 0;

	memcpy(buffer_serializado + desplazamiento, &(dato->cant_memorias) , sizeof(int));
	desplazamiento += sizeof(int);

	void _serializar_memoria(void* _memoria){

		struct MemoriasEstructura* memoria_aux = (struct MemoriasEstructura *)_memoria;

		memcpy(buffer_serializado + desplazamiento, &(memoria_aux->numero_memoria) , sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(buffer_serializado + desplazamiento, &(memoria_aux->ip->size) , sizeof(int));
		desplazamiento += sizeof(int);

		memcpy(buffer_serializado + desplazamiento, memoria_aux->ip->buffer , memoria_aux->ip->size);
		desplazamiento += memoria_aux->ip->size;

		memcpy(buffer_serializado + desplazamiento, &(memoria_aux->puerto) , sizeof(int));
		desplazamiento += sizeof(int);

	}

	list_iterate(dato->memorias, _serializar_memoria);

	return buffer_serializado;

}

void liberar_dato_memoria(struct MemoriasEstructura* memoria){

	free(memoria->ip->buffer);
	free(memoria->ip);
	free(memoria);

}

void liberar_dato_gossiping(struct DatoTablaGossiping* dato){


	list_destroy_and_destroy_elements(dato->memorias, liberar_dato_memoria);

	free(dato);

}



void enviar_datos(int memoria2, t_list* memorias ){

	struct DatoTablaGossiping* dato_a_enviar = malloc(sizeof(struct DatoTablaGossiping));
	dato_a_enviar->memorias = list_create();

	dato_a_enviar->cant_memorias = list_size(memorias);

	dato_a_enviar->bytes = sizeof(int);

	void _agregar_dato(void* _memoria){

		Seed memoria_aux = (Seed) _memoria;

		struct MemoriasEstructura* memoria = malloc(sizeof(struct MemoriasEstructura));
		memoria->numero_memoria = memoria_aux->numero_memoria;
		dato_a_enviar->bytes += sizeof(int);

		memoria->ip = malloc(sizeof(t_stream));
		memoria->ip->size = strlen(memoria_aux->ip) + 1;
		dato_a_enviar->bytes += sizeof(int);

		memoria->ip->buffer = malloc(memoria->ip->size);
		memcpy(memoria->ip->buffer, memoria_aux->ip, memoria->ip->size);
		dato_a_enviar->bytes += memoria->ip->size;

		memoria->puerto = memoria_aux->puerto;
		dato_a_enviar->bytes += sizeof(int);

		list_add(dato_a_enviar->memorias, memoria);

	}

	list_iterate(memorias, _agregar_dato);

	//Los bytes deberian quedar como: cant_memorias(int) + (n_memoria(int) + tamanio_ip(int) + elIP(tamanio_ip) + puerto(int)) * cant_memorias
	void* buffer;

	buffer = serializar_gossiping(dato_a_enviar);

	send(memoria2, buffer, dato_a_enviar->bytes, 0);

	liberar_dato_gossiping(dato_a_enviar);
	free(buffer);

}

void intercambiar_datos(Memoria memoria, int memoria2){


	enviar_datos(memoria2, memoria->tabla_gossiping);

}

bool existis_en_tabla_gossip(struct MemoriasEstructura* memoria_nuevo){


	bool _contiene_memoria(void* _seed){

		Seed seed = (Seed)_seed;

		return seed->numero_memoria == memoria_nuevo->numero_memoria;


	}

	return list_any_satisfy(memoria->tabla_gossiping , _contiene_memoria);
}

