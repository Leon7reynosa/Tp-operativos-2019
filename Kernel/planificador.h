/*
 * planificador.h
 *
 *  Created on: 13 jun. 2019
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include"Config/configuracion.h"
#include"Mensajeria/Cliente/cliente.h"

typedef struct{

	t_queue* cola_requests;
	char* path_lql;

}t_planificador;


t_queue* cola_new;
t_queue* cola_ready;
t_queue* cola_exit;

t_queue* parsear_LQL(FILE* archivo_lql);
void inicializar_cola_exec(t_queue* colas[] , int grado_multiprocesamiento);
void inicializar_cola_new(void);
void inicializar_cola_ready(void);
void inicializar_cola_exit(void);
void cola_new_to_ready(void);
void cola_ready_a_exec(t_queue* cola_exec);


#endif /* PLANIFICADOR_H_ */
