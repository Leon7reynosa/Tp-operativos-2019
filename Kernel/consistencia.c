/*
 * consistencia.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include "consistencia.h"


void inicializar_consistencias(){

	Strong_C = malloc(sizeof(memoria_t));

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

	string_to_upper(consistencia);

	if(strcmp(consistencia, "SC") == 0){
		return SC;
	}else if(strcmp(consistencia, "EC") == 0){
		return EC;
	}else if(strcmp(consistencia, "HSC") == 0){
		return SHC;
	}else{
		printf("fallaste bro\n");
		return -1;
	}

}

cod_consistencia identificar_consitencia_para_tabla(int cod_request, void* tipo_request){

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

			tabla = (char*)((create) tipo_request)->tabla->buffer;
			break;

		case DESCRIBE:

			tabla = (char*) ((describe_t) tipo_request)->tabla->buffer;
			break;

		default:
			printf("CODIGO DE REQUEST INCORRECTO\n");
			break;
	}

	codigo_consistencia = ((metadata_t*) dictionary_get( registro_tabla ,tabla))->codigo_consistencia;

	return codigo_consistencia;
}


memoria_t* seleccionar_memoria_consistencia(request request_a_enviar){

	cod_consistencia codigo_consistencia = identificar_consitencia_para_tabla(request_a_enviar->cod_op, request_a_enviar->tipo_request);
	int index_memoria;
	int numero_random;

	switch(codigo_consistencia){

		case SC:

			return Strong_C;
			break;

		case SHC:

			index_memoria = obtener_index_memoria(request_a_enviar);

			if(index_memoria >= 0 ){

				return list_get(Strong_Hash_C, index_memoria);

			}else{

				numero_random = rand() % list_size(Strong_Hash_C);

				return list_get(Strong_Hash_C, numero_random);

			}
			break;

		case EC:

			numero_random = rand() % list_size(Eventual_C);

			return list_get(Eventual_C, numero_random);

			break;

		default:
			printf("HUBO UN FALLO EN SELECCIONAR UNA MEMORIA DE LAS CONSISTENCIAS\n");
			return NULL;
			break;

	}

}


void guardar_tabla_consistencia(char* tabla, metadata_t* metadata_tabla){

	dictionary_put(registro_tabla , tabla, metadata_tabla);

}


int obtener_index_memoria(int key){

	int cantidad_de_memorias = list_size(tabla_gossiping);

	return key%cantidad_de_memorias;
}




