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

/*
 *3
 * tamanio+ip +
 *
*/

#endif /* GOSSIPING_H_ */
