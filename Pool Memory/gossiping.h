/*
 * gossiping.h
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#ifndef GOSSIPING_H_
#define GOSSIPING_H_

#include"Mensajeria/Cliente/cliente.h"
#include"Mensajeria/mensajes.h"
#include"memoria.h"

#include<commons/collections/list.h>

struct MemoriasEstructura{
	int numero_memoria;
	t_stream* ip;
	int puerto;

};

struct DatoTablaGossiping{
	int bytes;
	int cant_memorias;	// con esto la memoria va a iterar tantas veces como cantidad de memorias le mande
	t_list* memorias;  //memoriasEstructura

};

void* gossiping(void);
void* serializar_gossiping(struct DatoTablaGossiping* dato);
void liberar_dato_memoria(struct MemoriasEstructura* memoria);
void liberar_dato_gossiping(struct DatoTablaGossiping* dato);
void enviar_datos(int memoria2, t_list* memorias );
void intercambiar_datos(Memoria memoria, int memoria2);

#endif /* GOSSIPING_H_ */
