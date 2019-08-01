/*
 * parser.c
 *
 *  Created on: 7 jul. 2019
 *      Author: utnso
 */


#include"parser.h"

request_parser parser_list[] = {
		{"SELECT" , SELECT},
		{"INSERT" , INSERT},
		{"CREATE" , CREATE},
		{"DESCRIBE", DESCRIBE},
		{"DROP" , DROP},
		{"JOURNAL" , JOURNAL},
		{"GOSSIP" , GOSSIP},
		{"ADD" , ADD},
		{"RUN" , RUN},
		{"EXIT" , EXIT},
		{"METRICS" , METRICS}
};

cod_operacion identificar_request ( char* request_lql){

	char** request_split;

	cod_operacion codigo_return;

	if( string_is_empty(request_lql) || string_equals_ignore_case(request_lql , "\n") || request_lql == NULL ){

		printf("ES VACIA LA REQUEST\n");
		return -1;

	}

	request_split = string_split(request_lql , " ");

	codigo_return  = encontrar_codigo_request(request_split[0]);

	liberar_puntero_doble(request_split);

	return codigo_return;

}

cod_operacion encontrar_codigo_request(char* request){

	int tamanio_lista_parser =  sizeof(parser_list)/sizeof(request_parser);  //pasarlo a global despues

	for(int i = 0; i < tamanio_lista_parser ; i++){

		if(string_equals_ignore_case(request, parser_list[i].request)){

			return parser_list[i].cod_request;

		}
	}

	return -1;

}

void liberar_puntero_doble(char** puntero_doble){

	int i = 0;
	while(*(puntero_doble + i) != NULL){

		free(*(puntero_doble + i));
		i++;

	}

	free(puntero_doble);

}
