/*
 * consistencia.h
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#ifndef CONSISTENCIA_H_
#define CONSISTENCIA_H_

#include<commons/collections/list.h>
#include"gossiping.h"

typedef enum{
	SC,
	EC,
	SHC
}cod_consistencia;

typedef struct{

	cod_consistencia codigo_consistencia;

}metadata_t;

t_dictionary* registro_tabla; //va a tener char* nombre_tabla y los elementos van a ser metadata_t

//van a ser listas de distintas memorias, es decir, sus sockets
memoria_t* Strong_C;
t_list* Eventual_C;
t_list* Strong_Hash_C;

#endif /* CONSISTENCIA_H_ */


