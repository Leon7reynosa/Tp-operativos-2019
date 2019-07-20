/*
 * consistencia.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include "consistencia.h"


void inicializar_registro_tabla(){

	registro_tabla = dictionary_create();

}

void inicializar_consistencias(){

	Strong_C = NULL;

	Eventual_C= list_create();

	Strong_Hash_C = list_create();

}

void agregar_a_consistencia(cod_consistencia codigo, memoria_t* memoria_a_guardar){

	switch(codigo){

		case SC:

			Strong_C = memoria_a_guardar;
			break;

		case EC:

			list_add(Eventual_C, memoria_a_guardar);
			break;

		case SHC:

			list_add(Strong_Hash_C, memoria_a_guardar);
			break;

		default:

			log_error(logger_kernel, "NO SE PUEDE IDENTIFICAR LA CONSISTENCIA.\n");
			break;
	}

}

cod_consistencia identificar_consistencia(char* consistencia){

	printf("entre aca gg\n");

	string_to_upper(consistencia);

	if(strcmp(consistencia, "SC") == 0){

		printf(">>La consistencia es Strong Consistency\n");

		return SC;
	}else if(strcmp(consistencia, "EC") == 0){

		printf(">>La consistencia es Eventual Consistency\n");

		return EC;
	}else if(strcmp(consistencia, "SHC") == 0){

		printf(">>La consitencia es Strong hash consistency\n");

		return SHC;
	}else{
		printf(">>No es una consistencia permitida<<\n");
		return -1;
	}

}

cod_consistencia identificar_consitencia_para_request(int cod_request, void* tipo_request){

	char* tabla;

	cod_consistencia codigo_consistencia;

	switch(cod_request){

		case SELECT:

			tabla = (char*)((select_t) tipo_request)->tabla->buffer;
			break;

		case INSERT:

			tabla = (char*)((insert) tipo_request)->tabla->buffer;
			break;

		case CREATE:

			tabla = (char*)((create) tipo_request)->consistencia->buffer; //en este caso usamos la consistencia

			printf("la consistencia es esta: %s\n" , tabla);

			return identificar_consistencia(tabla);

			break;

		case DESCRIBE:

			tabla = (char*) ((describe_t) tipo_request)->tabla->buffer;
			break;

		case DROP:

			tabla = (char*) ((Drop) tipo_request)->tabla->buffer;
			break;

		default:
			printf("CODIGO DE REQUEST INCORRECTO\n");
			break;
	}

	printf("tabla: %s\n" , tabla);

	if(!dictionary_has_key(registro_tabla, tabla)){

		printf("No existe la tabla en el registro de tablas\n"); //log

		return -1;

	}

	printf("pase el if lol\n");

	codigo_consistencia =    identificar_consistencia(  (( Metadata ) dictionary_get(registro_tabla , tabla ) )->consistencia );

	printf("holas\n");

	printf("CODIGO DE CONSISTENCIA: %d\n" , codigo_consistencia);

	return codigo_consistencia;
}


memoria_t* tomar_memoria_al_azar(){

	t_list* lista_de_consistencias = lista_memorias_de_consistencia();

	printf("tamaño de lista: %d\n" , list_size(lista_de_consistencias));

	int numero_random_consistencia = rand() % list_size(lista_de_consistencias); // para determinar de cual de los tres codigos de operacion usamos para agarrar la memoria ( SC, EC, SHC)

	return (memoria_t*) list_get(lista_de_consistencias , numero_random_consistencia);

	//aca creo que falta liberar

}

t_list* lista_memorias_de_consistencia(){

	t_list* lista_de_memorias = list_create();

	printf("entre a la lista de memorias de consistencia\n");

	if(Strong_C != NULL){

		printf("no entro aca\n");

		list_add(lista_de_memorias, Strong_C);

	}

	printf("hago el iterate\n");

	void _agregar_si_cumple(void* _memoria){

		memoria_t* memoria = (memoria_t *)_memoria;

		bool _esta_agregado(void* _memoria_2){

				memoria_t* memoria_2 = (memoria_t *)_memoria_2 ;

				printf("hice uno\n");

				return memoria == memoria_2;

		}

		if(list_any_satisfy(lista_de_memorias, _esta_agregado)){
			//no lo agrego

		}else{

			printf("aca entro una sola vez\n");

			list_add(lista_de_memorias, memoria);

		}

	}

	list_iterate(Strong_Hash_C, _agregar_si_cumple);
	printf("pase el iterate\n");

	list_iterate(Eventual_C, _agregar_si_cumple);

	printf("pase el segundo iterate\n");

	return lista_de_memorias;

}

memoria_t* memoria_al_azar_consistencia(t_list* lista_consistencia){

	int index_random = rand() % list_size(lista_consistencia);

	return (memoria_t*) list_get(lista_consistencia, index_random);


}

//TOMA UNA MEMORIA SEGUN LA REQUEST QUE LE PASES
memoria_t* seleccionar_memoria_consistencia(cod_operacion cod_op , void* tipo_request){

	cod_consistencia codigo_consistencia = identificar_consitencia_para_request(cod_op, tipo_request);

	printf("llegue hasta este paso\n");

	printf("codigo_consistencia: %d\n" , codigo_consistencia);

	return tomar_memoria_segun_codigo_consistencia(codigo_consistencia);

}

memoria_t* tomar_memoria_segun_codigo_consistencia(cod_consistencia codigo_consistencia){

	int index_memoria;
	int numero_random;

	printf("estoy aca\n");

	printf("codigo: %d\n" , codigo_consistencia);

	switch(codigo_consistencia){

		case SC:

			return Strong_C;
			break;

		case SHC:

//			index_memoria = obtener_index_memoria(request_a_enviar);
//
//			if(index_memoria >= 0 ){
//
//				return list_get(Strong_Hash_C, index_memoria);
//
//			}else{
//
//				numero_random = rand() % list_size(Strong_Hash_C); //aca no deberia
//
//				return list_get(Strong_Hash_C, numero_random);
//
//			}

			//Aca deberiamos ver la funcion hashs, por ahora devuelvo la primera de las memorias

			if(list_is_empty(Strong_Hash_C)){

				return NULL;
			}

			return list_get(Strong_Hash_C , 0);


			break;

		case EC:

			printf("llegue aqui\n");

			if(list_is_empty(Eventual_C)){

				return NULL;

			}

			numero_random = rand() % list_size(Eventual_C);

			return list_get(Eventual_C, numero_random);

			break;

		default:
			printf("HUBO UN FALLO EN SELECCIONAR UNA MEMORIA DE LAS CONSISTENCIAS\n");
			return NULL;
			break;

	}

	printf("termine :/\n");

	return NULL;

}


void mostrar_memoria_utilizada(memoria_t* memoria_utilizada){

	printf("\n>>La memoria utilizada sera: \n");
	printf("Numero Memoria: %d\n" , memoria_utilizada->numero_memoria);
	printf("Socket: %d\n", memoria_utilizada->socket);
	printf("Ip: %s\n" , memoria_utilizada->ip);
	printf("Puerto: %d\n\n" , memoria_utilizada->puerto);

}

void guardar_tabla_consistencia(char* tabla, metadata_t* metadata_tabla){

	dictionary_put(registro_tabla , tabla, metadata_tabla);

}


int obtener_index_memoria(int key){

	int cantidad_de_memorias = list_size(tabla_gossiping);

	return key%cantidad_de_memorias;
}


void actualizar_metadata(t_list* datos_describe){


	void _actualizar_en_tabla_metadata(void* dato_describe){

		Metadata dato_metadata = (Metadata) dato_describe;

		//agregar semaforo de escritura

		if(dictionary_has_key(registro_tabla, dato_metadata->tabla)){

			dictionary_remove_and_destroy(registro_tabla , dato_metadata->tabla , liberar_metadata);

		}


		dictionary_put(registro_tabla, dato_metadata->tabla, dato_metadata);

		//agregar semaforo de escritura

	}

	list_iterate(datos_describe, _actualizar_en_tabla_metadata);



}




