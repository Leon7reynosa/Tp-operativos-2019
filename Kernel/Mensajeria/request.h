/*
 * request.h
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#ifndef MENSAJERIA_REQUEST_H_
#define MENSAJERIA_REQUEST_H_

#include<stdio.h>
#include<stdlib.h>
#include"estructuras.h"

typedef enum{
	SELECT,
	INSERT,
	CREATE,
	DESCRIBE,
	DROP,
	JOURNAL,
	GOSSIP,
	ADD,
	RUN,
	EXIT
}cod_operacion;

struct requestEstructura{

	cod_operacion cod_op;
	void* tipo_request;

};

//struct requestEstructura;
typedef struct requestEstructura* request;

#endif /* MENSAJERIA_REQUEST_H_ */
