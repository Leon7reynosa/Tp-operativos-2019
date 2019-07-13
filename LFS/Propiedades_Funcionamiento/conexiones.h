/*
 * conexiones.h
 *
 *  Created on: 11 jun. 2019
 *      Author: utnso
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<commons/config.h>
#include"Servidor/servidor.h"
#include"configuracion.h"
#include"Mensajeria/request.h"
#include"Mensajeria/recibir.h"

struct estructuraConexionMemoria{

	pthread_t hilo_memoria;
	int socket_memoria;

};

typedef struct estructuraConexionMemoria* Conexion_memoria;

t_list* memorias_conectadas;  //va a tener una lista de Conexion_memoria

void* conectar_varias_memorias(void);

#endif /* CONEXIONES_H_ */
