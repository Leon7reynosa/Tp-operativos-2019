/*
 * gossiping.c
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#include"gossiping.h"

void* auto_gossip(void* argumentos){

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	while(1){

//		pthread_rwlock_rdlock(&semaforo_tiempo_gossiping);

		usleep(tiempo_gossiping * 1000);

//		pthread_rwlock_unlock(&semaforo_tiempo_gossiping);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		pthread_mutex_lock(&mutex_gossip);

		printf("\n>>>>>>>>>>>>>>>>>>>>>>>>Auto-Gossip<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

		log_info(logger_gossip, "Se realizara un AUTO-GOSSIPING");


//		log_info(logger_gossip, "Inicia el Auto-Gossiping");
		gossiping();

		log_info(logger_gossip, "Se termino de realizar el AUTO-GOSSIPING\n");

		printf("\n>>>>>>>>>>>>>>>>>>>>>>FIN Auto-Gossip<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

		pthread_mutex_unlock(&mutex_gossip);
//		log_info(logger_gossip, "Termina el Auto-Gossiping\n");


		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	}
	return NULL;
}

void gossiping(){

	int cantidad_seeds = list_size(memoria->seed);

	int i ;

	int socket_seed;
	Seed seed_aux;
	tabla_gossip_dto datos_gossip;

	for(i = 0 ; i < cantidad_seeds ; i++){

		seed_aux = list_get(memoria->seed, i);

		socket_seed = conectar_servidor(seed_aux->ip, seed_aux->puerto);

		if(socket_seed >= 0){

			log_info(logger_gossip, "Se establecio conexion con la memoria seed");

			tabla_gossip_dto dato_gossip = generar_datos_gossip(memoria->tabla_gossiping);

			request nueva_request = crear_request(GOSSIP, dato_gossip);

			if(! enviar_request(nueva_request, socket_seed)){

				printf("No se pudo mandar la request a la memoria seed\n");

				log_info(logger_gossip, "No se pudo mandar la request Gossip a la memoria seed");

				liberar_request(nueva_request);

				close(socket_seed);

				continue;

			}

			log_info(logger_gossip, "Se envio la request de gossiping a la memoria seed");

			datos_gossip = recibir_datos_gossip(socket_seed);

//			log_info(logger, "Actualizacion de tabla gossip");
			actualizar_tabla_gossip(datos_gossip);

			liberar_request(nueva_request);

			liberar_dato_gossiping(datos_gossip);

			close(socket_seed);

		}
		else{

			log_info(logger_gossip, "No se pudo conectar con %s:%i", seed_aux->ip, seed_aux->puerto);
			//nada, proba otro dia crack! (LOGGEAR que la memoria seed no esta conectada)
		}

	}

}

void* serializar_gossiping(struct DatoTablaGossiping* dato){

	void* buffer_serializado = malloc(dato->bytes);
	int desplazamiento = 0;

	memcpy(buffer_serializado + desplazamiento, &(dato->cant_memorias) , sizeof(int));
	desplazamiento += sizeof(int);

	void _serializar_memoria(void* _memoria){

		memoria_dto memoria_aux = (memoria_dto)_memoria;

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

tabla_gossip_dto generar_datos_gossip(t_list* memorias){

	int cant_memorias = list_size(memorias);

	tabla_gossip_dto dato_a_enviar = crear_dto_gossip(cant_memorias);

	printf("Voy a enviar %i memorias\n", dato_a_enviar->cant_memorias);

	log_info(logger_gossip, "Se envian %i memorias", dato_a_enviar->cant_memorias);

	void _agregar_dato(void* _memoria){

		Seed memoria_aux = (Seed) _memoria;

		printf("Agrego la memoria: \n");
		printf("nro: %i\n", memoria_aux->numero_memoria);
		printf("puerto: %i\n", memoria_aux->puerto);
		printf("ip: %s\n", memoria_aux->ip);

		memoria_dto memoria_a_enviar = crear_memoria_dto(memoria_aux->numero_memoria, memoria_aux->ip, memoria_aux->puerto);

		agregar_memoria_gossip_dto(dato_a_enviar, memoria_a_enviar);

	}

	list_iterate(memorias, _agregar_dato);

	return dato_a_enviar;

}


void enviar_datos(int memoria2, t_list* memorias ){

	tabla_gossip_dto dato_a_enviar = generar_datos_gossip(memorias);

	int bytes_enviados = 0;
	int bytes_restantes = dato_a_enviar->bytes;
	int enviados_aux;

	void* buffer;

	buffer = serializar_gossiping(dato_a_enviar);

	while(bytes_enviados < dato_a_enviar->bytes){

		printf("Envio las memorias\n");
		enviados_aux = send(memoria2, buffer + bytes_enviados, bytes_restantes, 0);

		if(enviados_aux == -1){
			perror("Send tiro -1");
				//ver que hacer aca
		}

		bytes_enviados += enviados_aux;
		bytes_restantes -= enviados_aux;

	}

	liberar_dato_gossiping(dato_a_enviar);
	free(buffer);

}

void intercambiar_datos(tabla_gossip_dto tabla_ajena, int conexion){

	log_info(logger_gossip, "Le envio %i memorias", list_size(memoria->tabla_gossiping));

	enviar_datos(conexion, memoria->tabla_gossiping);

	actualizar_tabla_gossip( tabla_ajena );

}

bool existis_en_tabla_gossip(struct MemoriasEstructura* memoria_nuevo){

	bool _contiene_memoria(void* _seed){

		Seed seed = (Seed)_seed;

		return seed->numero_memoria == memoria_nuevo->numero_memoria;


	}

	return list_any_satisfy(memoria->tabla_gossiping , _contiene_memoria);
}

////////////////////////////////////////////////////////////////////////

Seed pasar_memoria_dto_a_seed(memoria_dto dato_dto){

	Seed dato_seed = malloc(sizeof(Seed));

	dato_seed->numero_memoria = dato_dto->numero_memoria;

	dato_seed->puerto = dato_dto->puerto;

	dato_seed->ip = malloc(dato_dto->ip->size);

	memcpy(dato_seed->ip , dato_dto->ip->buffer , dato_dto->ip->size);


	return dato_seed;
}


void actualizar_tabla_gossip(tabla_gossip_dto request_gossip){

	printf(">>>>>>>>>>>>>>>>>>>>>>>>>ACTUALIZACION TABLA GOSSIPING<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

	log_info(logger_gossip, "Actualizo mi tabla de gossiping");

	void _agregar_si_no_esta(void* dato_memoriaEstructura){

		memoria_dto dato_extraido = (memoria_dto) dato_memoriaEstructura;

		printf(">Memoria a actualizar: %i\n", dato_extraido->numero_memoria);
		printf(">IP: %s\n",(char *) (dato_extraido->ip->buffer));
		printf(">PUERTO: %i\n", dato_extraido->puerto);

		if(!existis_en_tabla_gossip(dato_extraido)){

			printf(">No existe en la tabla gossiping la memoria %i, se agrega.\n", dato_extraido->numero_memoria);

			Seed dato_a_agregar = pasar_memoria_dto_a_seed(dato_extraido);

			log_info(logger_gossip, "No existe la memoria %i, la agregamos con los siguientes parametros: ");

			log_info(logger_gossip, "IP: %s", dato_a_agregar->ip);
			log_info(logger_gossip, "PUERTO: %i", dato_a_agregar->puerto);

			list_add(memoria->tabla_gossiping, dato_a_agregar);

		}else{

			printf(">Existe en tabla de gossiping la memoria %i", dato_extraido->numero_memoria);

			log_info(logger_gossip, "Ya existe la memoria %i en la tabla de gossipin", dato_extraido->numero_memoria);

		}


	}

	list_iterate(request_gossip->memorias , _agregar_si_no_esta);

	log_info(logger_gossip, "Termine de actualizar mi tabla gossiping");

	printf(">>>>>>>>>>>>>>>>>>>>>>>>FINALIZO LA ACTUALIZACION<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");


}

