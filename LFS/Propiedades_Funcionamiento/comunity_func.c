/*
 * comunity_func.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include "comunity_func.h"


//void responder_request(request request, int conexion){
//
//	int bytes;
//	char* tabla;
//	dato_t* dato_respuesta ;
//	void* buffer;
//	char* valor;
//	char* consistencia;
//
//	switch(request->cod_op){
//
//		case SELECT:
//
//			tabla = (char*) ((select_t) request->tipo_request)->tabla->buffer;
//
//			dato_respuesta = request_select( tabla , ((select_t) request)->key  );
//
//			buffer = serializar_dato_t(dato_respuesta, &bytes);
//
//			send(conexion, buffer, bytes , 0 );
//
//			liberar_dato(dato_respuesta);
//
//			break;
//
//		case INSERT:
//
//			tabla = (char*) ((insert) request->tipo_request)->tabla->buffer;
//
//			valor = (char*) ((insert) request->tipo_request)->value->buffer;
//
//			request_insert( tabla, ((insert) request)->key , valor, ((insert) request)->timestamp );
//
//			//log de que se inserto todo en orden (tal vez adentro del insert)
//
//			break;
//
//		case CREATE:
//
//			tabla = (char*) ((create) request->tipo_request)->tabla->buffer;
//
//			consistencia =  (char*) ((create) request->tipo_request)->consistencia->buffer;
//
//			request_create(tabla, consistencia , ((create) request->tipo_request)->numero_particiones, ((create) request->tipo_request)->compactacion);
//
//			//log de que se creo todo en  orden. (tal vez va adentro del create)
//
//			break;
//
//		default:
//
//			//log de error
//
//			break;
//
//	}
//
//}

int calcular_particion(int particion_metadata ,u_int16_t key){

	return key % particion_metadata;

}

dato_t* crear_dato(u_int16_t key, char* valor, time_t tiempo){

	dato_t* nuevo = malloc(sizeof(dato_t));

	nuevo->key = key;
	nuevo->value = malloc(strlen(valor) + 1);
	memcpy(nuevo->value, valor, strlen(valor) + 1);
	nuevo->timestamp = tiempo;

	return nuevo;
}

void liberar_dato(dato_t* dato_remove){

	free(dato_remove->value);
	free(dato_remove);

}

dato_t* timestamp_mas_grande(dato_t* primer_dato , dato_t* segundo_dato){
	//solo el primer dato puede ser null

	if(primer_dato == NULL || segundo_dato->timestamp > primer_dato->timestamp){

		return segundo_dato;
	}else {
		return primer_dato;
	}
}


void liberar_puntero_doble(char** puntero){

	int i = 0;
	while(*(puntero + i) != NULL){
		i++;
	}

	for(; i > 0 ; i--){
		free(*(puntero + i));
	}
	free(puntero);

}

dato_t* convertir_a_dato(char* dato_encontrado){

	char** partes_dato = string_split(dato_encontrado, ";");

	time_t dato_timestamp = atoi(partes_dato[0]);
	u_int16_t dato_key = atoi( partes_dato[1] );

	dato_t* dato_return = crear_dato(dato_key , partes_dato[2] ,  dato_timestamp);

	liberar_puntero_doble(partes_dato);

	return dato_return;

}

