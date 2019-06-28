/*
 * gossiping.c
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#include"gossiping.h"


void actualizar_gossiping(){

	int tiempo_ejecucion = tiempo_gossiping_kernel * 1000000;
	void* buffer ;
	cod_operacion gossip= GOSSIP;
	int cantidad_memorias = 0;

	while(1){

		buffer = malloc(sizeof(cod_operacion) + sizeof(int));

		memcpy(buffer , &gossip , sizeof(cod_operacion) );

		memcpy((buffer +  sizeof(cod_operacion)) , &cantidad_memorias , sizeof(int));

		send(conexion_memoria , buffer, sizeof(cod_operacion) , 0);

		recibir_actualizacion_gossiping();

		free(buffer);

		usleep(tiempo_ejecucion);

	}


}


void recibir_actualizacion_gossiping(){

	int *cantidad_memorias = malloc(sizeof(int));

	recv(conexion_memoria ,cantidad_memorias,sizeof(int),0);

	memoria_t* dato_memoria_ingresar;

	for(int i = 0 ; i < cantidad_memorias ; i++){

		struct MemoriasEstructura* memoria_recv = malloc(sizeof(struct MemoriasEstructura));

		recv(conexion_memoria, &(memoria_recv->numero_memoria) , sizeof(int) , 0 );

		recv(conexion_memoria, &(memoria_recv->ip->size) , sizeof(int) , 0  );

		memoria_recv->ip->buffer = malloc(memoria_recv->ip->size);

		recv(conexion_memoria, memoria_recv->ip->buffer , sizeof(int) , 0  );

		recv(conexion_memoria, &(memoria_recv->puerto) , sizeof(int) , 0 );

		if(!existe_en_tabla_gossiping(memoria_recv)){

			dato_memoria_ingresar = convertir_a_memoria_t(memoria_recv);

			dato_memoria_ingresar->socket = conectar_servidor(dato_memoria_ingresar->ip,
					dato_memoria_ingresar->puerto);

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



}

void ingresar_a_tabla_gossiping(memoria_t* dato_memoria_ingresar){

	list_add(tabla_gossiping, dato_memoria_ingresar);

}


memoria_t* convertir_a_memoria_t(struct MemoriasEstructura* dato_memoria){

	memoria_t* dato_convertido = malloc(sizeof(memoria_t));
	dato_convertido->ip = malloc(dato_memoria->ip->size);

	dato_convertido->numero_memoria = dato_memoria->numero_memoria;
	dato_convertido->puerto = dato_memoria->puerto;
	memcpy(dato_convertido->ip , dato_memoria->ip->buffer , dato_memoria->ip->size);


	return dato_convertido;
}

bool existe_en_tabla_gossiping(struct MemoriasEstructura* dato_memoria){



	bool _estas_en_la_tabla(memoria_t* dato){

		return (!strcmp(dato->ip , dato_memoria->ip->buffer) &&
				(dato->puerto == dato_memoria->puerto));


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

