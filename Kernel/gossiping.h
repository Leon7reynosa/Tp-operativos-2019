/*
 * gossiping.h
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#ifndef GOSSIPING_H_
#define GOSSIPING_H_

#include<commons/collections/list.h>
#include"Config/configuracion.h"
#include"Mensajeria/request.h"
#include"Mensajeria/Cliente/cliente.h"

typedef struct{

	int numero_memoria;
	t_stream* ip;
	int puerto;
	int socket;

}memoria_t; //PARA LA TABLA_GOSSIPING

t_list* tabla_gossiping;

void actualizar_gossiping(void);

#endif /* GOSSIPING_H_ */
