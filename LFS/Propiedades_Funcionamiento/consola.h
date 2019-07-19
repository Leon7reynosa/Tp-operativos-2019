/*
 * consola.h
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<unistd.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include"Mensajeria/requestCreate.h"
#include"Mensajeria/requestDescribe.h"
#include"Mensajeria/requestDrop.h"
#include"Mensajeria/requestInsert.h"
#include"Mensajeria/requestSelect.h"
#include"Mensajeria/mensajes.h"


void* consola(void* argumento);
bool ejecutar_request(cod_operacion codigo_request , char* linea_request);
void menu(void);


#endif /* CONSOLA_H_ */
