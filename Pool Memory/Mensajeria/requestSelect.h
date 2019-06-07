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


struct selectEstructura;
typedef struct selectEstructura* select_t;

void* serializar_select(request request_select);
select_t crear_dato_select(char* tabla, u_int16_t key);
void liberar_dato(select_t);

#endif /* MENSAJERIA_REQUESTSELECT_H_ */
