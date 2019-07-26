/*
 * requestJournal.c
 *
 *  Created on: 26 jul. 2019
 *      Author: utnso
 */


#include"requestJournal.h"

char* serializar_journal(request request){

	char* buffer = malloc(sizeof(cod_operacion));

	memcpy(buffer , &(request->cod_op), sizeof(cod_operacion));

	return buffer;
}
