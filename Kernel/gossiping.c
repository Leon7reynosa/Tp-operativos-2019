/*
 * gossiping.c
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#include"gossiping.h"


void* realizar_gossiping(){


	while(1){

		usleep(tiempo_gossiping_kernel * 100); //despues hay que ponerle 1000

		printf("Voy a realizar el gossiping :D\n");

		int conexion = tomar_socket_memoria_aleatorio(tabla_gossiping);

		printf("el socket es: %d\n" , conexion);

		bool terminaste = actualizar_gossiping(conexion);

		while((conexion > 0) && !terminaste){

			printf("Fallo el gossiping, lo intento denuevo");

			close(conexion);

			memoria_t* memoria = remover_conexion(conexion, tabla_gossiping);
			remover_memoria_de_consistencia(memoria);
			eliminar_memoria_t(memoria);

			conexion = tomar_socket_memoria_aleatorio(tabla_gossiping);

			terminaste = actualizar_gossiping(conexion);
		}

		printf("no entre al while\n");

		if(conexion < 0 ){
			printf("No hay memorias conectadas\n");
		}

		printf("Se realizo el gossiping\n");
	}
}

void remover_memoria_de_consistencia(memoria_t* memoria){

	remover_conexion(memoria->socket, Eventual_C);

	remover_conexion(memoria->socket, Strong_Hash_C);

	if(memoria == Strong_C){

		Strong_C = NULL;

	}

}


memoria_t* remover_conexion( int conexion , t_list* lista_a_remover ){


	bool _tenes_esta_conexion(void* _dato_memoria){

		memoria_t* dato_memoria = (memoria_t* ) _dato_memoria;

		return dato_memoria->socket == conexion;

	}

	memoria_t* dato = list_remove_by_condition(lista_a_remover, _tenes_esta_conexion);

	return dato;

}

void eliminar_memoria_t( memoria_t* dato_memoria ){

	free(dato_memoria->ip);

	free(dato_memoria);

}


int tomar_socket_memoria_aleatorio(t_list* lista_memorias){

	int socket_return;
	int numero_random;

	if(list_size(lista_memorias) > 0){
		numero_random = rand() % list_size(lista_memorias);
	}else{
		return -1;
	}

	memoria_t* dato_lista = (memoria_t* ) list_get(lista_memorias , numero_random);

	return dato_lista->socket;

}


bool actualizar_gossiping(int conexion){

	void* buffer ;
	cod_operacion gossip= GOSSIP;
	int cantidad_memorias = 0;

	printf("entre al actualizar gossipings\n");

	buffer = malloc(sizeof(cod_operacion) + sizeof(int));

	memcpy(buffer , &gossip , sizeof(cod_operacion) );

	printf("entre al actualizar gossipings\n");

	memcpy((buffer +  sizeof(cod_operacion)) , &cantidad_memorias , sizeof(int));

	if(  (send(conexion , buffer, sizeof(cod_operacion) + sizeof(int) , 0) ) < 0 ){

		return false;

	}

	printf("entre al actualizar gossipings\n");

	free(buffer);

	return recibir_actualizacion_gossiping(conexion);
}


bool recibir_actualizacion_gossiping(int conexion){

	int *cantidad_memorias = malloc(sizeof(int));

	if((recv(conexion ,cantidad_memorias,sizeof(int),0)) < 0 ){

		return false;

	}

	memoria_t* dato_memoria_ingresar;

	for(int i = 0 ; i < cantidad_memorias ; i++){

		struct MemoriasEstructura* memoria_recv = malloc(sizeof(struct MemoriasEstructura));

		memoria_recv->ip = malloc(sizeof(t_stream));

		recv(conexion, &(memoria_recv->numero_memoria) , sizeof(int) , 0 );

		recv(conexion, &(memoria_recv->ip->size) , sizeof(int) , 0  );

		memoria_recv->ip->buffer = malloc(memoria_recv->ip->size);

		recv(conexion, memoria_recv->ip->buffer , memoria_recv->ip->size , 0  );

		recv(conexion, &(memoria_recv->puerto) , sizeof(int) , 0 );

		printf("Me llego la memoria: \n");

		printf("NUMERO: %i\n", memoria_recv->numero_memoria);
		printf("SIZE IP: %i\n", memoria_recv->ip->size);
		printf("IP: %s\n", (char *)memoria_recv->ip->buffer);
		printf("PUERTO: %i\n", memoria_recv->puerto);

		if(!existe_en_tabla_gossiping(memoria_recv)){

			dato_memoria_ingresar = convertir_a_memoria_t(memoria_recv);

//			dato_memoria_ingresar->socket = conectar_servidor(dato_memoria_ingresar->ip,
//											dato_memoria_ingresar->puerto); //esto capaz no va.

			if(dato_memoria_ingresar->socket > 0){

				ingresar_a_tabla_gossiping(dato_memoria_ingresar);

				log_info(logger_kernel, "SE ESTABLECION LA CONEXION CON LA MEMORIA %d.\n" ,
						dato_memoria_ingresar->numero_memoria);

			}else{

				log_error(logger_kernel, "NO SE PUDO ESTABLECER LA CONEXION CON LA MEMORIA %d.\n",
						dato_memoria_ingresar->numero_memoria);

				liberar_memoria_t(dato_memoria_ingresar);

			}

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

	list_add(tabla_gossiping, dato_memoria_ingresar);

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

	memoria_creada->puerto = puerto;

	memoria_creada->socket = conectar_servidor(memoria_creada->ip,	memoria_creada->puerto); //esto capaz no va.

	if(memoria_creada->socket > 0){

		ingresar_a_tabla_gossiping(memoria_creada);

		log_info(logger_kernel, "SE ESTABLECION LA CONEXION CON LA MEMORIA %d.\n" ,
				memoria_creada->numero_memoria);

	}else{

		log_error(logger_kernel, "NO SE PUDO ESTABLECER LA CONEXION CON LA MEMORIA %d.\n",
				memoria_creada->numero_memoria);

		liberar_memoria_t(memoria_creada);

	}

	printf("termine el crear memoria_t\n");

	return memoria_creada;

}


memoria_t* convertir_a_memoria_t(struct MemoriasEstructura* dato_memoria){

	memoria_t* dato_convertido = malloc(sizeof(memoria_t));
	dato_convertido->ip = malloc(dato_memoria->ip->size);

	dato_convertido->numero_memoria = dato_memoria->numero_memoria;
	dato_convertido->puerto = dato_memoria->puerto;
	memcpy(dato_convertido->ip , dato_memoria->ip->buffer , dato_memoria->ip->size);

	dato_convertido->socket = conectar_servidor(dato_memoria->ip, dato_memoria->puerto); //esto capaz no va.

	return dato_convertido;
}

bool existe_en_tabla_gossiping(struct MemoriasEstructura* dato_memoria){



	bool _estas_en_la_tabla(memoria_t* dato){

		return dato->numero_memoria == dato_memoria->numero_memoria;


	}

	return list_any_satisfy(tabla_gossiping, _estas_en_la_tabla);

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

