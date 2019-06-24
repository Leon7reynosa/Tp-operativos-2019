/*
 * requestGossiping.h
 *
 *  Created on: 21 jun. 2019
 *      Author: utnso
 */

#ifndef MENSAJERIA_REQUESTGOSSIPING_H_
#define MENSAJERIA_REQUESTGOSSIPING_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>
#include"request.h"

struct MemoriasEstructura{
	int numero_memoria;
	t_stream* ip;
	int puerto;

};

typedef struct MemoriasEstructura* memoria_dto;

struct DatoTablaGossiping{
	int bytes;
	int cant_memorias;	// con esto la memoria va a iterar tantas veces como cantidad de memorias le mande
	t_list* memorias;  //memoriasEstructura

};

typedef struct DatoTablaGossiping* tabla_gossip_dto;

#endif /* MENSAJERIA_REQUESTGOSSIPING_H_ */
