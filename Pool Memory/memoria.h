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

Memoria inicializar_memoria(int tamanio, int tamanio_value , int tamanio_dato, t_list* tablas);
t_list* inicializar_tabla_segmentos(t_list* tablas_a_inicializar);
void ingresar_dato_a_memoria(Dato nuevo_dato, void** referencia_a_memoria);
void encontrar_espacio_libre(void** referencia_a_memoria);
Dato pedir_dato_al_LFS(char* tabla, int key);

#endif /* MEMORIA_H_ */
