/*
 * consola.h
 *
 *  Created on: 20 jul. 2019
 *      Author: utnso
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_


#include<stdio.h>
#include<stdlib.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<pthread.h>

#include"Servidor/servidor.h"
#include<commons/collections/list.h>
#include<commons/string.h>

#include"API_Pool.h"
#include"Mensajeria/mensajes.h"
#include"gossiping.h"

bool desconexion_pool;

bool leer_consola(void);
void parsear_request(cod_operacion operacion, char** tokens);
int obtener_cantidad_argumentos(char** tokens);

#endif /* CONSOLA_H_ */
