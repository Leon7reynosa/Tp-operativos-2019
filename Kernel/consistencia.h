/*
 * consistencia.h
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#ifndef CONSISTENCIA_H_
#define CONSISTENCIA_H_

#include<commons/collections/list.h>

typedef enum{
	SC,
	EC,
	SHC
}cod_consistencia;


//van a ser listas de distintas memorias, es decir, sus sockets
t_list* Strong_C;
t_list* Eventual_C;
t_list* Strong_Hash_C;

#endif /* CONSISTENCIA_H_ */


