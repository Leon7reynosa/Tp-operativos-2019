/*
 * pagina.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef PAGINA_H_
#define PAGINA_H_

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<commons/collections/list.h>

#include"dato.h"

struct PaginaEstructura{

	void* referencia_memoria;  //no libero esto, por que es memoria!
	bool flag_modificado;
	bool flag_en_uso;
	time_t ultimo_uso;

};

//struct PaginaEstructura;
typedef struct PaginaEstructura* Pagina;

Pagina pagina_menos_usada(t_list* paginas);
t_list* paginas_sin_modificar(t_list* paginas);
Pagina crear_pagina(void* posicion_memoria);
void liberar_pagina(Pagina pagina_a_liberar);
bool esta_libre(Pagina pagina);
void mostrar_datos(Pagina pagina);
void actualizar_uso(Pagina pagina);

#endif /* PAGINA_H_ */
