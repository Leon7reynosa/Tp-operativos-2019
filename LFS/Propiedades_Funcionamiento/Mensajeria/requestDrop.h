/*
 * requestDrop.h
 *
 *  Created on: 18 jul. 2019
 *      Author: utnso
 */

#ifndef PROPIEDADES_FUNCIONAMIENTO_REQUESTDROP_H_
#define PROPIEDADES_FUNCIONAMIENTO_REQUESTDROP_H_

#include<stdio.h>
#include<stdlib.h>

#include"request.h"
#include<commons/string.h>

struct dropEstructura{

	int bytes;
	t_stream* tabla;

};

typedef struct dropEstructura* Drop;


#endif /* PROPIEDADES_FUNCIONAMIENTO_REQUESTDROP_H_ */
