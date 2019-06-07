/*
 * requestSelect.h
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#ifndef MENSAJERIA_REQUESTSELECT_H_
#define MENSAJERIA_REQUESTSELECT_H_

#include<stdio.h>
#include<stdlib.h>

#include"request.h"

struct selectEstructura{

	int bytes;
	t_stream* tabla;
	u_int16_t key;

};

//struct selectEstructura;
typedef struct selectEstructura* select_t;

select_t decodificar_select(int conexion);
void* serializar_select(request request_select);
select_t crear_dato_select(char* tabla, u_int16_t key);
void liberar_dato_select(select_t);

#endif /* MENSAJERIA_REQUESTSELECT_H_ */
