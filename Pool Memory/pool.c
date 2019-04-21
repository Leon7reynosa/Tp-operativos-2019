/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"


int main (void){

	int conexion;
	char* ip;
	char* puerto;


	ip = "127.0.0.1";
	puerto = "4444";

	conexion = crear_conexion( ip , puerto );

	//char* valor = "Chino sos un cochino";

	//enviar_mensaje(valor , conexion);

	esperar_cliente(conexion);

	liberar_conexion(conexion);


	return 1;
}
