/*
 * gossiping.c
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#include"gossiping.h"


void actualizar_gossiping(){

	int tiempo_ejecucion = tiempo_gossiping_kernel / 1000;
	void* buffer ;

	while(1){

		sleep(tiempo_ejecucion);

		buffer = malloc(sizeof(cod_operacion));

		memcpy(buffer , GOSSIP , sizeof(cod_operacion) );

		send(socket_pool , buffer, sizeof(cod_operacion) , 0);

		recibir_actualizacion_gossiping();

		//luego actualizar la tabla de gossiping

		free(buffer);

	}


}


void recibir_actualizacion_gossiping(){

	int *cantidad_memorias = malloc(sizeof(int));

	recv(socket_pool ,cantidad_memorias,sizeof(int),0);

	for(int i = 0 ; i < cantidad_memorias ; i++){

		memoria_t* memoria_aux = malloc(sizeof(memoria_t));

		recv(socket_pool, &(memoria_aux->numero_memoria) , sizeof(int) , 0 );

		recv(socket_pool, &(memoria_aux->ip->size) , sizeof(int) , 0  );

		memoria_aux->ip->buffer = malloc(memoria_aux->ip->size);

		recv(socket_pool, memoria_aux->ip->buffer , sizeof(int) , 0  );

		recv(socket_pool, &(memoria_aux->puerto) , sizeof(int) , 0 );

		//ACA deberia iniciar la conexion con las memorias

		//memoria_aux->socket = conectar_servidor(memoria_aux->ip->buffer , memoria_aux->puerto);


		//hay que tener cuidado por que ya va a tener algunas memorias en la segunda actualizacion
		//list_add(tabla_gossiping, memoria_aux);
	}



}

