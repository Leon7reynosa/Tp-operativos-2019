/*
 * memoria.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include<stdio.h>
#include<stdlib.h>
#include"segmento.h"
#include<commons/collections/list.h>
#include<commons/string.h>
#include<pthread.h>

#include"Mensajeria/requestSelect.h"
#include"Mensajeria/mensajes.h"
#include"Mensajeria/configuracion.h"


struct seedEstructura{
	int numero_memoria;
	char* ip;
	int puerto;

};
typedef struct seedEstructura* Seed;



struct MemoriaEstructura{
	int numero_memoria;
	void* memoria_contigua;
	t_list* paginas;
	t_list* tabla_segmentos;
	int tamanio;
	int cant_max_datos;
	t_list* seed;            // lista de "Seed"
	t_list* tabla_gossiping; // va a tener seeds para pasarle
};

//struct MemoriaEstructura;
typedef struct MemoriaEstructura* Memoria;

Memoria memoria;

pthread_t journal_thread;
pthread_mutex_t mutex_journal;

pthread_t gossip_thread;
pthread_mutex_t mutex_gossip; //ver si es necesario

t_list* inicializar_paginas(void);
void inicializar_memoria(int tamanio, int tamanio_value , int tamanio_dato);
void inicializar_seeds(void);
t_list* inicializar_tabla_segmentos(t_list* tablas_a_inicializar);
void liberar_memoria(void);

Seed crear_seed(int numero , char* ip, int puerto);
void liberar_seed(Seed seed_a_liberar);

bool existe_segmento(char* nombre_tabla, Segmento* segmento_encontrado);
Segmento encontrar_segmento_con_pagina(Pagina pagina_buscada);
bool hay_pagina_libre(Pagina* pagina_libre);
void guardar_dato_en_memoria(Dato nuevo_dato, void* posicion_memoria);
void* auto_journal(void* argumento);
void realizar_journal(void);
Pagina realizar_algoritmo_reemplazo(void);
Pagina solicitar_pagina(char* nombre_tabla, Segmento* segmento);
Dato pedir_dato_al_LFS(char* tabla, int key);
void actualizar_pagina(Pagina pagina_encontrada, Dato dato_insert);

void eliminar_segmentos(void);

#endif /* MEMORIA_H_ */
