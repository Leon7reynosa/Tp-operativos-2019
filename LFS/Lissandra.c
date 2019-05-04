/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */

#include "Lissandra.h"

int main(void){

	int socket_sv;
	int socket_pool;
	char* ip;
	int puerto;



	obtener_puerto_ip(&puerto,&ip);

	realizar_select("Tablas/Tabla_A" , 233);

	insert("Tablas/Tabla_A", 7 , "HOLITAS" ,  1400 );
	insert("Tablas/Tabla_A", 8 , "CHAUSITO" ,  1450 );


	/*
	printf("IP = %s\n",ip);
	printf("PUERTO = %d\n",puerto);

	socket_sv = iniciar_servidor( ip ,puerto );

	socket_pool = aceptar_conexion(socket_sv);

	recibir_mensaje(socket_pool);

	close(socket_sv);
	config_destroy(g_config);
	*/
	return EXIT_SUCCESS;
}

