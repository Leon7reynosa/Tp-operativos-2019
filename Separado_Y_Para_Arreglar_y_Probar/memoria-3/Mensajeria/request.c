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

void liberar_request(request request_a_liberar){

	switch(request_a_liberar->cod_op){

		case SELECT:

			liberar_dato_select(request_a_liberar->tipo_request);

			break;
		case INSERT:

			liberar_dato_insert(request_a_liberar->tipo_request);

			break;

		case CREATE:

			liberar_dato_create(request_a_liberar->tipo_request);

			break;

		case GOSSIP:

			liberar_dato_gossiping(request_a_liberar->tipo_request);

			break;

		case DESCRIBE:

			liberar_dato_describe(request_a_liberar->tipo_request);

			break;

		case DROP:

			liberar_drop(request_a_liberar->tipo_request);

			break;

		default:

			break;

	}

	free(request_a_liberar);


}
