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
#include"t_dato.h"

typedef enum{
	SELECT,
	INSERT,
	CREATE,
	DESCRIBE,
	DROP,
	JOURNAL,
	GOSSIP,
	DESCONEXION
}cod_operacion;

typedef enum{

	E_CREATE = -3,
	E_KEY,
	E_TABLA,
	SUCCESS

}error_request;


struct requestEstructura{

	cod_operacion cod_op;
	void* tipo_request;

};

//struct requestEstructura;
typedef struct requestEstructura* request;

request crear_request(cod_operacion cod_op, void* tipo_request);
void liberar_request(request request_a_liberar);

#endif /* MENSAJERIA_REQUEST_H_ */
