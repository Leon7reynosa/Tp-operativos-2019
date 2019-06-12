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

#include"Mensajeria/requestSelect.h"
#include"Mensajeria/mensajes.h"

struct MemoriaEstructura{
	void* memoria_contigua;
	t_list* paginas;
	t_list* tabla_segmentos;
	int tamanio;
	int cant_max_datos;
	struct MemoriaEstructura* seed;
};

//struct MemoriaEstructura;
typedef struct MemoriaEstructura* Memoria;

Memoria memoria;


t_list* inicializar_paginsa(void);
void inicializar_memoria(int tamanio, int tamanio_value , int tamanio_dato, t_list* tablas);
t_list* inicializar_tabla_segmentos(t_list* tablas_a_inicializar);

bool existe_segmento(char* nombre_tabla, Segmento* segmento_encontrado);
Segmento encontrar_segmento_con_pagina(Pagina pagina_buscada);
bool hay_pagina_libre(Pagina* pagina_libre);
void guardar_dato_en_memoria(Dato nuevo_dato, void* posicion_memoria);
void realizar_journal(void);
Pagina realizar_algoritmo_reemplazo(void);
Pagina solicitar_pagina(void);
Dato pedir_dato_al_LFS(char* tabla, int key);
void actualizar_pagina(Pagina pagina_encontrada, Dato dato_insert);

#endif /* MEMORIA_H_ */
