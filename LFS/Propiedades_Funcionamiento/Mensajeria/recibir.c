/*
 * recibir.c
 *
 *  Created on: 19 abr. 2019
 *      Author: utnso
 */

#include"recibir.h"


request recibir_request(int conexion){

	cod_operacion* cod_op = malloc(sizeof(cod_operacion));
	void* tipo_request;
	int pene = 0;

	request request;

	pene = recv(conexion, cod_op,sizeof(cod_operacion),MSG_WAITALL);

	if(pene == -1){
			perror("Fallo al recibir el codigo de operacion.");
			//aca deberiamos sacar a la memoria de la lista?
			//aca deberiamos terminar el hilo?
	}
	if(pene == 0){

		request = crear_request(DESCONEXION, NULL);
		return request;
	}

	printf("codigo_op : %d\n" , *cod_op);

	switch(*cod_op){

		case SELECT:

			printf(">>REALIZAMOS UN SELECT\n");

			tipo_request = decodificar_select(conexion);
			break;

		case INSERT:

			printf(">>REALIZAMOS UN INSERT\n");

			tipo_request = decodificar_insert(conexion);

			break;

		case CREATE:

			printf(">>REALIZAMOS UN CREATE\n");

			tipo_request = decodificar_create(conexion);
			break;

		case DESCRIBE:

			printf("REALIZAMOS UN DESCRIBE\n");

			tipo_request = decodificar_describe(conexion);

			break;

		case DROP:

			printf("REALIZAMOS UN DROP\n");

			tipo_request = decodificar_drop(conexion);

			break;

		default:

			printf(">>NO RECONOCI LA REQUEST :/ \n");

			break;

	}

	request = crear_request(*cod_op, tipo_request);

	return request;

}

