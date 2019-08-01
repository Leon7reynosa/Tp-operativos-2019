/*
 * requestJournal.c
 *
 *  Created on: 26 jul. 2019
 *      Author: utnso
 */


#include"requestJournal.h"

void* serializar_journal(request request){

	void* buffer = malloc(sizeof(cod_operacion));

	memcpy(buffer , &(request->cod_op), sizeof(cod_operacion));

	return buffer;
}


void mandar_journal(memoria_t* memoria_a_journal){

	request request_journal = crear_request(JOURNAL, NULL);

	void* buffer = serializar_journal(request_journal);

	send( memoria_a_journal->socket , buffer, sizeof(cod_operacion) , 0 ) ;

	printf("Fin del Journal\n");

	free(buffer);

	liberar_request(request_journal);

}
