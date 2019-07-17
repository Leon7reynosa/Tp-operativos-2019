/*
 * pool.h
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#ifndef POOL_H_
#define POOL_H_

#include<stdio.h>
#include<stdlib.h>
//#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<pthread.h>

#include"Servidor/servidor.h"
#include"API_Pool.h"
#include"Mensajeria/mensajes.h"
#include"gossiping.h"

//void* conectar_kernel();
//void* conectar_lissandra();

void pruebas(void);
void inicializar_hilos(void);


bool leer_consola(void);
void parsear_request(cod_operacion operacion, char** tokens);
int obtener_cantidad_argumentos(char** tokens);

#endif /* POOL_H_ */
