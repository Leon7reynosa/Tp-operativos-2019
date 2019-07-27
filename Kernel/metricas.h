/*
 * metricas.h
 *
 *  Created on: 26 jul. 2019
 *      Author: utnso
 */

#ifndef METRICAS_H_
#define METRICAS_H_

#include"consistencia.h"
#include"gossiping.h"
#include<pthread.h>

typedef struct{

	t_list* lista_insert; //van a tener tiempo;
	t_list* lista_select;


}metrica_t;

metrica_t* metrica_sc;
metrica_t* metrica_ec;
metrica_t* metrica_shc;

pthread_rwlock_t semaforo_metrica_sc;
pthread_rwlock_t semaforo_metrica_ec;
pthread_rwlock_t semaforo_metrica_shc;

void* realizar_metrics();
void inicializar_metricas(void);
void inicializar_metrica(metrica_t** metrica_a_init);
void agregar_a_metrica(cod_operacion codigo_operacion , void* tipo_request, time_t tiempo_ejecucion);
void sumador_metrica( cod_operacion codigo_operacion , metrica_t* metrica,  time_t tiempo_ejecucion );
void request_metrics(void);
void mostrar_memory_load(void);
int total_ejecuciones(void);
void sumar_contador_memoria(memoria_t** memoria);
void mostrar_metricas(metrica_t* metrica);
int sumatoria_tiempos (t_list* lista_a_sumar);

#endif /* METRICAS_H_ */
