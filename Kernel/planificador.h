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
#include"API_Kernel.h"

typedef struct{

	t_queue* cola_requests;
	char* path_lql;

}t_scripts;

t_list* lista_hilos_exec;

pthread_rwlock_t semaforo_cola_ready;


t_queue* cola_new;
t_queue* cola_ready;
t_queue* cola_exit; //en la cola de exit se almacenararn los t_scripts

void* planificador(t_queue* cola_exec[]);
void cancelar_hilos_execute(void);
void liberar_script(t_scripts* script);
t_queue* parsear_LQL(FILE* archivo_lql);
void inicializar_cola_exec(t_queue* colas[] , int grado_multiprocesamiento);
void inicializar_cola_new(int argc , char* argv[]);
void inicializar_cola_ready(void);
void inicializar_cola_exit(void);
void cola_new_to_ready(void);
void ejecutar_cola_exec(t_queue* cola_exec);


#endif /* PLANIFICADOR_H_ */
