/*
 * memoria.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include"segmento.h"

struct MemoriaEstructura;
typedef struct MemoriaEstructura* Memoria;

void inicializar_memoria(void);
void inicializar_tabla_segmentos(t_list* nombre_tablas);
void ingresar_dato_a_memoria(Dato nuevo_dato, void** referencia_a_memoria);
void encontrar_espacio_libre(void** referencia_a_memoria);

#endif /* MEMORIA_H_ */
