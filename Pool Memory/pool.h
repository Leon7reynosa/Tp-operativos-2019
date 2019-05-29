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
#include<pthread.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include"Servidor/servidor.h"
#include"API_Pool.h"

void inicializar_memoria(void/*luego posible "pool"*/);
void* conectar_lissandra(void);
void* conectar_kernel(void);
void pruebita();//Luego borrar

#endif /* POOL_H_ */
