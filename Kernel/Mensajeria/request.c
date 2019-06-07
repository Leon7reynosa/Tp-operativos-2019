/*
 * request.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */


#include"request.h"
/*
struct requestEstructura{

	cod_operacion cod_op;
	void* tipo_request;

};
*/

request crear_request(cod_operacion cod_op, void* tipo_request){

	struct requestEstructura* nueva_request = malloc(sizeof(struct requestEstructura));

	nueva_request->cod_op = cod_op;
	nueva_request->tipo_request = tipo_request;

	return nueva_request;

}
