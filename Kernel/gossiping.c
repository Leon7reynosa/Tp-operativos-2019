/*
 * gossiping.c
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#include"gossiping.h"


void* realizar_gossiping(){


	while(1){

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		usleep(tiempo_gossiping_kernel * 1000); //despues hay que ponerle 1000

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		log_info(logger_gossip , "Iniciando el GOSSIPING KERNEL");

		pthread_rwlock_rdlock(&semaforo_tabla_gossiping);

		memoria_t* memoria_utilizada = tomar_socket_memoria_aleatorio(tabla_gossiping);

		pthread_rwlock_unlock(&semaforo_tabla_gossiping);

		if(memoria_utilizada != NULL){

			//printf(">>La Memoria Utilizada sera la Memoria N° %d\n" , memoria_utilizada->numero_memoria) ;

			log_info(logger_gossip , "MEMORIA utilizada: memoria %d" , memoria_utilizada->numero_memoria);

		}


		bool terminaste = actualizar_gossiping(memoria_utilizada); //semaforo adentro

		while((memoria_utilizada != NULL) && !terminaste){

			printf("Fallo el gossiping, lo intento denuevo\n");

			log_error(logger_gossip, "Fallo el gossiping, lo intento de nuevo");

			pthread_rwlock_rdlock(&semaforo_tabla_gossiping);

			memoria_utilizada= tomar_socket_memoria_aleatorio(tabla_gossiping);

			pthread_rwlock_unlock(&semaforo_tabla_gossiping);

			terminaste = actualizar_gossiping(memoria_utilizada);
		}


		if( memoria_utilizada == NULL ){
			printf(">>No hay memorias conectadas<<\n");
			log_error(logger_gossip , "No hay memorias conectadas");
		}

		//printf("\n///////////////////////FIN GOSSIP///////////////////////////////\n");
		log_info(logger_gossip , "Fin del GOSSIPING KERNEL\n");
	}
}

void poner_memoria_desconectada(int conexion){

	pthread_rwlock_wrlock(&semaforo_tabla_gossiping);

	void _sos_la_memoria(void* _memoria){

		memoria_t* memoria = (memoria_t* ) _memoria;

		if( memoria->socket = conexion){

			memoria->conectado = false;

		}

	}

	list_iterate(tabla_gossiping , _sos_la_memoria);

	pthread_rwlock_unlock(&semaforo_tabla_gossiping);

}

void remover_memoria_de_tabla_gossiping(memoria_t* memoria_utilizada){

	pthread_rwlock_wrlock(&semaforo_tabla_gossiping);

	remover_conexion(memoria_utilizada->socket , tabla_gossiping);

	pthread_rwlock_unlock(&semaforo_tabla_gossiping);

	liberar_memoria_t(memoria_utilizada); //cuidado

}

void remover_memoria_de_consistencia(memoria_t* memoria){

	//SEMAFOROS AFUERA

	memoria_t* memoria_devuelta_shc;

	pthread_rwlock_wrlock(&semaforo_strong_c);

	if(memoria == Strong_C){

		Strong_C = NULL;

	}

	pthread_rwlock_unlock(&semaforo_strong_c);

	pthread_rwlock_wrlock(&semaforo_eventual_c);

	remover_conexion(memoria->socket, Eventual_C);

	pthread_rwlock_unlock(&semaforo_eventual_c);

	pthread_rwlock_wrlock(&semaforo_strong_hash_c);

	memoria_devuelta_shc = remover_conexion(memoria->socket, Strong_Hash_C);

	pthread_rwlock_unlock(&semaforo_strong_hash_c);

	if(memoria_devuelta_shc != NULL){


		void* _hacete_journal(void* _memoria_shc){

			memoria_t* memoria_shc = (memoria_t* ) _memoria_shc;

			mandar_journal(memoria_shc);

			return NULL;

		}

		list_iterate(Strong_Hash_C , _hacete_journal);

	}

}


memoria_t* remover_conexion( int conexion , t_list* lista_a_remover ){

	bool _tenes_esta_conexion(void* _dato_memoria){

		memoria_t* dato_memoria = (memoria_t* ) _dato_memoria;

		return dato_memoria->socket == conexion;

	}

	memoria_t* dato = list_remove_by_condition(lista_a_remover, _tenes_esta_conexion);

	//deberia liberarla?

	return dato;

}

void eliminar_memoria_t( memoria_t* dato_memoria ){

	free(dato_memoria->ip);

	free(dato_memoria);

}


memoria_t* tomar_socket_memoria_aleatorio(t_list* lista_memorias){

	int socket_return;
	int numero_random;

	bool _estas_conectada(void* _memoria){

		memoria_t* memoria = (memoria_t* ) _memoria;

		return memoria->conectado;

	}

	t_list* lista_memorias_conectadas = list_filter(lista_memorias , _estas_conectada);

	if(list_size(lista_memorias_conectadas) > 0){
		numero_random = rand() % list_size(lista_memorias_conectadas);
	}else{
		return NULL;
	}

	memoria_t* dato_lista = (memoria_t* ) list_get(lista_memorias_conectadas , numero_random);

	return dato_lista;

}


bool actualizar_gossiping(memoria_t* memoria){

	//EL SEMAFORO ESTA AFUERA

	bool respuesta;

	if(memoria == NULL){

		return false;

	}

	pthread_rwlock_wrlock(&memoria->semaforo_memoria);

	void* buffer ;
	cod_operacion gossip= GOSSIP;
	int cantidad_memorias = 0;

	buffer = malloc(sizeof(cod_operacion) + sizeof(int));

	memcpy(buffer , &gossip , sizeof(cod_operacion) );

	memcpy((buffer +  sizeof(cod_operacion)) , &cantidad_memorias , sizeof(int));

	if(  (send(memoria->socket , buffer, sizeof(cod_operacion) + sizeof(int) , 0) ) <= 0 ){

		memoria->conectado = false;

		pthread_rwlock_unlock(&memoria->semaforo_memoria);

		free(buffer);

		return false;

	}

	free(buffer);

	respuesta = recibir_actualizacion_gossiping( memoria);

	pthread_rwlock_unlock(&memoria->semaforo_memoria);

	return respuesta;
}


bool recibir_actualizacion_gossiping(memoria_t* memoria){

	//el semaforo de memoria esta afuera

	int *cantidad_memorias = malloc(sizeof(int));

	if((recv(memoria->socket ,cantidad_memorias,sizeof(int),0)) <= 0 ){

		memoria->conectado =false;

		remover_memoria_de_consistencia(memoria);

		return false;

	}

	memoria_t* dato_memoria_ingresar;

	//printf("\n>>Cantidad de memorias : %d\n"  , *cantidad_memorias);

	for(int i = 0 ; i < *cantidad_memorias ; i++){

		struct MemoriasEstructura* memoria_recv = malloc(sizeof(struct MemoriasEstructura));

		memoria_recv->ip = malloc(sizeof(t_stream));

		recv(memoria->socket, &(memoria_recv->numero_memoria) , sizeof(int) , 0 );

		recv(memoria->socket, &(memoria_recv->ip->size) , sizeof(int) , 0  );

		memoria_recv->ip->buffer = malloc(memoria_recv->ip->size);

		recv(memoria->socket, memoria_recv->ip->buffer , memoria_recv->ip->size , 0  );

		recv(memoria->socket, &(memoria_recv->puerto) , sizeof(int) , 0 );

		log_info(logger_gossip , "Me llego la memoria: %d" , memoria_recv->numero_memoria);

//		//printf("\n>>Me llego la memoria: \n\n");

//
//		//printf("NUMERO: %i\n", memoria_recv->numero_memoria);
//		printf("SIZE IP: %i\n", memoria_recv->ip->size);
//		printf("IP: %s\n", (char *)memoria_recv->ip->buffer);
//		printf("PUERTO: %i\n", memoria_recv->puerto);

		if(!existe_en_tabla_gossiping(memoria_recv)){

//			printf("\n>>La memoria no existe en tablaa gossiping<<\n\n");

			dato_memoria_ingresar = convertir_a_memoria_t(memoria_recv);

//			dato_memoria_ingresar->socket = conectar_servidor(dato_memoria_ingresar->ip,
//											dato_memoria_ingresar->puerto); //esto capaz no va.

			if(dato_memoria_ingresar->socket > 0){

				log_info(logger_gossip, "SE ESTABLECION LA CONEXION CON LA MEMORIA %d." ,
						dato_memoria_ingresar->numero_memoria);

				dato_memoria_ingresar->conectado = true;


			}else{

				dato_memoria_ingresar->conectado = false;

				log_error(logger_gossip, "NO SE PUDO ESTABLECER LA CONEXION CON LA MEMORIA %d.",
						dato_memoria_ingresar->numero_memoria);

			}

			ingresar_a_tabla_gossiping(dato_memoria_ingresar);

		}

		liberar_dato_memoria(memoria_recv);
	}

	free(cantidad_memorias);

	return true;

}

void inicializar_tabla_gossiping(){

	tabla_gossiping = list_create();

}

void ingresar_a_tabla_gossiping(memoria_t* dato_memoria_ingresar){

	pthread_rwlock_wrlock(&semaforo_tabla_gossiping);

	list_add(tabla_gossiping, dato_memoria_ingresar);

	printf("\n>Memoria %d Agregada a Tabla de Gossiping--\n" , dato_memoria_ingresar->numero_memoria);

	log_info(logger_gossip , "La Memoria %d agregada a la tabla de gossiping");

	pthread_rwlock_unlock(&semaforo_tabla_gossiping);

}



struct MemoriasEstructura* crear_memoria_estructura(char* ip , int puerto, int numero_memoria){

	struct MemoriasEstructura* memoria_nueva = malloc(sizeof(struct MemoriasEstructura));

	memoria_nueva->ip->size = strlen(ip) + 1;

	memoria_nueva->ip->buffer = malloc(memoria_nueva->ip->size);

	memcpy(memoria_nueva->ip->buffer, ip, memoria_nueva->ip->size);

	memoria_nueva->numero_memoria = numero_memoria;

	memoria_nueva->puerto = puerto;

	return memoria_nueva;

}


memoria_t* crear_memoria_t(char* ip , int puerto , int numero_memoria){

	memoria_t* memoria_creada = malloc(sizeof(memoria_t));

	memoria_creada->ip = malloc(strlen(ip) + 1);

	memcpy(memoria_creada->ip , ip , strlen(ip) + 1);

	memoria_creada->numero_memoria = numero_memoria;

	memoria_creada->contador_requests = 0;

	memoria_creada->puerto = puerto;

	memoria_creada->socket = conectar_servidor(memoria_creada->ip,	memoria_creada->puerto); //esto capaz no va.

	pthread_rwlock_init(&memoria_creada->semaforo_memoria , NULL);

	if(memoria_creada->socket > 0){


		log_info(logger_kernel, "-Se establecio la conexion con la MEMORIA %d.-" ,
				memoria_creada->numero_memoria);

		memoria_creada->conectado = true;

	}else{

		log_error(logger_kernel, "NO se pudo establecer la conexion con la MEMORIA%d.-",
				memoria_creada->numero_memoria);

		memoria_creada->conectado = false;

	}



	return memoria_creada;

}


memoria_t* convertir_a_memoria_t(struct MemoriasEstructura* dato_memoria){


	memoria_t* dato_convertido = malloc(sizeof(memoria_t));

	dato_convertido->ip = malloc(dato_memoria->ip->size);

	dato_convertido->numero_memoria = dato_memoria->numero_memoria;
	dato_convertido->puerto = dato_memoria->puerto;
	memcpy(dato_convertido->ip , dato_memoria->ip->buffer , dato_memoria->ip->size);

	pthread_rwlock_init(&dato_convertido->semaforo_memoria , NULL);

	dato_convertido->socket = conectar_servidor(dato_convertido->ip, dato_convertido->puerto); //esto capaz no va.

	return dato_convertido;
}

bool existe_en_tabla_gossiping(struct MemoriasEstructura* dato_memoria){

	bool valor_return;

	bool _estas_en_la_tabla(memoria_t* dato){

		return dato->numero_memoria == dato_memoria->numero_memoria;


	}

	pthread_rwlock_rdlock(&semaforo_tabla_gossiping);

	valor_return = list_any_satisfy(tabla_gossiping, _estas_en_la_tabla);

	pthread_rwlock_unlock(&semaforo_tabla_gossiping);

	return valor_return;


}

void liberar_dato_memoria(struct MemoriasEstructura* memoria){

	free(memoria->ip->buffer);
	free(memoria->ip);
	free(memoria);

}

void liberar_memoria_t(memoria_t* dato_a_borrar){

	free(dato_a_borrar->ip);
	free(dato_a_borrar);
}

