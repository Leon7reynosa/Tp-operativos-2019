/*
 * requestCreate.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#include"requestCreate.h"

struct createEstructura{

	t_stream* tabla;
	t_stream* consistencia;
	int numero_particiones;
	time_t compactacion;

};
