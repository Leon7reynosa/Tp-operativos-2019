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
		{"RUN" , RUN}
};

cod_operacion identificar_request ( char* request_lql){

	char** request_split;

	cod_operacion codigo_return;

	request_split = string_split(request_lql , " ");

	printf("%s\n" , request_split[0]);

	codigo_return  = encontrar_codigo_request(request_split[0]);

	free(request_split);

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
