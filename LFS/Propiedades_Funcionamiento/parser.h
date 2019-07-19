/*
 * parser.h
 *
 *  Created on: 7 jul. 2019
 *      Author: utnso
 */

#ifndef PARSER_H_
#define PARSER_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/string.h>
#include"Mensajeria/request.h"

typedef struct {

	char* request;
	cod_operacion cod_request;

}request_parser;




cod_operacion identificar_request ( char* request_lql);
cod_operacion encontrar_codigo_request(char* request);


#endif /* PARSER_H_ */
