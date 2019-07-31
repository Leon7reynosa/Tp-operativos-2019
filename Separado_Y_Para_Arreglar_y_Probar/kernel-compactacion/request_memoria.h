/*
 * request_memoria.h
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#ifndef REQUEST_MEMORIA_H_
#define REQUEST_MEMORIA_H_

#include"consistencia.h"
#include"kernel.h"
#include"Mensajeria/Cliente/cliente.h"
#include<commons/collections/list.h>
#include"consistencia.h"


void request_add(int numero_memoria, char* consistencia);
memoria_t* obtener_memoria_de_lista( int numero_memoria );
memoria_t* crear_dato_memoria_consistencia(char* ip , int numero_memoria, int puerto);


#endif /* REQUEST_MEMORIA_H_ */
