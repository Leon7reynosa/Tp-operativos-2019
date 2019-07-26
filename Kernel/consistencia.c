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

			pthread_rwlock_wrlock(&semaforo_strong_c);

			Strong_C = memoria_a_guardar;

			pthread_rwlock_unlock(&semaforo_strong_c);

			break;

		case EC:

			pthread_rwlock_wrlock(&semaforo_eventual_c);

			list_add(Eventual_C, memoria_a_guardar);

			pthread_rwlock_unlock(&semaforo_eventual_c);

			break;

		case SHC:

			pthread_rwlock_wrlock(&semaforo_strong_hash_c);

			list_add(Strong_Hash_C, memoria_a_guardar);

			pthread_rwlock_unlock(&semaforo_strong_hash_c);

			request_journal();

			break;

		default:

			log_error(logger_kernel, "NO SE PUEDE IDENTIFICAR LA CONSISTENCIA.\n");
			break;
	}

}

cod_consistencia identificar_consistencia(char* consistencia){

	string_to_upper(consistencia);

	if(strcmp(consistencia, "SC") == 0){

		printf("\n>>La consistencia es Strong Consistency\n");

		return SC;
	}else if(strcmp(consistencia, "EC") == 0){

		printf("\n>>La consistencia es Eventual Consistency\n");

		return EC;
	}else if(strcmp(consistencia, "SHC") == 0){

		printf("\n>>La consitencia es Strong hash consistency\n");

		return SHC;
	}else{
		printf("\n>>No es una consistencia permitida<<\n");
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

	pthread_rwlock_rdlock(&semaforo_registro_tabla);

	if(!dictionary_has_key(registro_tabla, tabla)){

		printf("No existe la tabla en el registro de tablas\n"); //log

		return -1;

	}

	codigo_consistencia =    identificar_consistencia(  (( Metadata ) dictionary_get(registro_tabla , tabla ) )->consistencia );

	pthread_rwlock_unlock(&semaforo_registro_tabla);

	return codigo_consistencia;
}


memoria_t* tomar_memoria_al_azar(){

	memoria_t* memoria_a_retornar;

	t_list* lista_de_consistencias = lista_memorias_de_consistencia();

	if(list_is_empty(lista_de_consistencias)){

		printf("TODAVIA NO HAY MEMORIAS EN LAS CONSISENCIAS\n");

		return NULL;

	}

	int numero_random_consistencia = rand() % list_size(lista_de_consistencias); // para determinar de cual de los tres codigos de operacion usamos para agarrar la memoria ( SC, EC, SHC)

	memoria_a_retornar = (memoria_t*) list_get(lista_de_consistencias , numero_random_consistencia);

	list_destroy(lista_de_consistencias);

	return memoria_a_retornar;

}

t_list* lista_memorias_de_consistencia(){

	t_list* lista_de_memorias = list_create();

	if(Strong_C != NULL){

		pthread_rwlock_rdlock(&semaforo_strong_c);

		list_add(lista_de_memorias, Strong_C);

		pthread_rwlock_unlock(&semaforo_strong_c);

	}

	void _agregar_si_cumple(void* _memoria){

		memoria_t* memoria = (memoria_t *)_memoria;

		bool _esta_agregado(void* _memoria_2){

				memoria_t* memoria_2 = (memoria_t *)_memoria_2 ;

				return memoria == memoria_2;

		}

		if(list_any_satisfy(lista_de_memorias, _esta_agregado)){
			//no lo agrego

		}else{

			list_add(lista_de_memorias, memoria);

		}

	}


	pthread_rwlock_rdlock(&semaforo_strong_hash_c);

	list_iterate(Strong_Hash_C, _agregar_si_cumple);

	pthread_rwlock_unlock(&semaforo_strong_hash_c);


	pthread_rwlock_rdlock(&semaforo_eventual_c);

	list_iterate(Eventual_C, _agregar_si_cumple);

	pthread_rwlock_unlock(&semaforo_eventual_c);


	return lista_de_memorias;

}

memoria_t* memoria_al_azar_consistencia(t_list* lista_consistencia){

	int index_random = rand() % list_size(lista_consistencia);

	return (memoria_t*) list_get(lista_consistencia, index_random);


}

//TOMA UNA MEMORIA SEGUN LA REQUEST QUE LE PASES
memoria_t* seleccionar_memoria_consistencia(cod_operacion cod_op , void* tipo_request){

	cod_consistencia codigo_consistencia = identificar_consitencia_para_request(cod_op, tipo_request);

	u_int16_t key_utilizada = determinar_key(cod_op , tipo_request);

	return tomar_memoria_segun_codigo_consistencia(codigo_consistencia, key_utilizada);

}


u_int16_t determinar_key(cod_operacion cod_op , void* tipo_request){


	insert request_insert;
	select_t request_select;

	int resultado_return;

	switch(cod_op){

		case INSERT:

			request_insert = (insert) tipo_request;

			return request_insert->key;

		case SELECT:

			request_select = (select_t) tipo_request;

			return request_select->key;

		default:


			pthread_rwlock_rdlock(&semaforo_strong_hash_c);

			if(list_is_empty(Strong_Hash_C)){

				resultado_return =  -1;

			}else{

				resultado_return = rand() % list_size(Strong_Hash_C);

			}

			pthread_rwlock_unlock(&semaforo_strong_hash_c);

			return resultado_return;

	}


}


memoria_t* tomar_memoria_segun_codigo_consistencia(cod_consistencia codigo_consistencia , u_int16_t key){

	int index_memoria;
	int numero_random;
	memoria_t* memoria_a_retornar;

	switch(codigo_consistencia){

		case SC:

			pthread_rwlock_rdlock(&semaforo_strong_c);

			memoria_a_retornar = Strong_C;

			pthread_rwlock_unlock(&semaforo_strong_c);

			return memoria_a_retornar;

			break;

		case SHC:


			pthread_rwlock_rdlock(&semaforo_strong_hash_c);

			if(list_is_empty(Strong_Hash_C)){

				memoria_a_retornar =  NULL;

			}else{

				if( key < 0 ){

					index_memoria = rand() % list_size(Strong_Hash_C);

				}else{

					index_memoria = funcion_hash(key);

				}

				memoria_a_retornar =  (memoria_t*)list_get(Strong_Hash_C , index_memoria);

			}

			pthread_rwlock_unlock(&semaforo_strong_hash_c);

			return memoria_a_retornar;

			break;

		case EC:

			pthread_rwlock_rdlock(&semaforo_eventual_c);

			if(list_is_empty(Eventual_C)){

				return NULL;

			}else{

				if ( memoria_siguiente >= (list_size(Eventual_C) -1) ){

					memoria_siguiente = 0;

				}else{

					memoria_siguiente++;

				}

				memoria_a_retornar = (memoria_t*) list_get(Eventual_C, memoria_siguiente);

			}

			pthread_rwlock_unlock(&semaforo_eventual_c);

			return memoria_a_retornar;

			break;

		default:
			printf("HUBO UN FALLO EN SELECCIONAR UNA MEMORIA DE LAS CONSISTENCIAS\n");
			return NULL;
			break;

	}


	return NULL;

}


int funcion_hash(u_int16_t key) {

	int indice_return;

	pthread_rwlock_rdlock(&semaforo_strong_hash_c);

	indice_return =  key % list_size(Strong_Hash_C);

	pthread_rwlock_unlock(&semaforo_strong_hash_c);

	return indice_return;

}


void mostrar_memoria_utilizada(memoria_t* memoria_utilizada){

	printf("\n>>La memoria utilizada sera: \n");
	printf("Numero Memoria: %d\n" , memoria_utilizada->numero_memoria);
	printf("Socket: %d\n", memoria_utilizada->socket);
	printf("Ip: %s\n" , memoria_utilizada->ip);
	printf("Puerto: %d\n\n" , memoria_utilizada->puerto);

}

void guardar_tabla_consistencia(char* tabla, metadata_t* metadata_tabla){

	pthread_rwlock_wrlock(&semaforo_registro_tabla);

	dictionary_put(registro_tabla , tabla, metadata_tabla);

	pthread_rwlock_unlock(&semaforo_registro_tabla);

}


int obtener_index_memoria(int key){

	pthread_rwlock_rdlock(&semaforo_tabla_gossiping);

	int cantidad_de_memorias = list_size(tabla_gossiping);

	pthread_rwlock_unlock(&semaforo_tabla_gossiping);

	return key%cantidad_de_memorias;
}


void actualizar_metadata(t_list* datos_describe){


	pthread_rwlock_wrlock(&semaforo_registro_tabla);

	dictionary_clean_and_destroy_elements(registro_tabla, liberar_metadata);

	pthread_rwlock_unlock(&semaforo_registro_tabla);

	void _actualizar_en_tabla_metadata(void* dato_describe){

		Metadata dato_metadata = (Metadata) dato_describe;

		pthread_rwlock_wrlock(&semaforo_registro_tabla);

		dictionary_put(registro_tabla, dato_metadata->tabla, dato_metadata);

		pthread_rwlock_unlock(&semaforo_registro_tabla);

	}

	list_iterate(datos_describe, _actualizar_en_tabla_metadata);


}


void agregar_metadata_a_registro_tabla(Metadata metadata_guardar){


	pthread_rwlock_wrlock( &semaforo_registro_tabla );

	if(dictionary_has_key(registro_tabla, metadata_guardar->tabla)){

		dictionary_remove_and_destroy(registro_tabla, metadata_guardar->tabla , liberar_metadata);

	}


	dictionary_put(registro_tabla , metadata_guardar->tabla , metadata_guardar);

	pthread_rwlock_unlock(&semaforo_registro_tabla);

}


bool existe_en_registro_tabla(char* nombre_tabla){

	bool contiene_tabla;

	pthread_rwlock_rdlock(&semaforo_registro_tabla);

	contiene_tabla = dictionary_has_key(registro_tabla,  nombre_tabla);

	pthread_rwlock_unlock(&semaforo_registro_tabla);

	return contiene_tabla;

}

void remover_tabla_de_registro(char* nombre_tabla){

	pthread_rwlock_rdlock(&semaforo_registro_tabla);

	dictionary_remove_and_destroy(registro_tabla, nombre_tabla, liberar_metadata);

	pthread_rwlock_unlock(&semaforo_registro_tabla);

}




