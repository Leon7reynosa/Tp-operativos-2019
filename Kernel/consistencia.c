/*
 * consistencia.c
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#include "consistencia.h"


void inicializar_consistencias(){

	Strong_C = list_create();

	Eventual_C= list_create();

	Strong_Hash_C = list_create();

}

void agregar_a_consistencia(t_list* consistencia , int* socket){

	list_add(consistencia, socket);

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

void* actualizar_tabla_gossiping(){
	//aca deberiamos pedirle, la tabla del gossiping a la memoria principal.




}
