/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"


int main (void){

	//socket servidor, socket cliente con lissandra y el socket nueva_conexion que administra nuevas conexiones
	int socket_sv, socket_cliente, kernel_conexion, nueva_conexion;
	int puerto; //este puerto va a ser el de lissandra
	char* ip; //esta ip va a ser la de lissandra

	int fdmax;
	int i;

	fd_set master;
	fd_set readfds;

	FD_ZERO(&master);
	FD_ZERO(&readfds);

	//obtengo el puerto y el id para conectarme a lissandra
	obtener_puerto_ip(&puerto,&ip);
	//me conecto a lissandra
	socket_cliente = conectar_servidor(ip,puerto);
	FD_SET(socket_cliente,&master);

	//lo inicio con el puerto 40000 por que se me canto.
	socket_sv = iniciar_servidor(4444);
	FD_SET(socket_sv,&master);

	//acepto al kernel
	kernel_conexion = aceptar_conexion(socket_sv);
	FD_SET(kernel_conexion,&master);

	fdmax = maximo(socket_sv,socket_cliente,kernel_conexion);

	while(1){

		readfds = master;

		if(select(fdmax + 1,&readfds,NULL,NULL,0) == -1){
			perror("Select");
			exit(1);
		}

		for(i = 0; i <= fdmax; i ++){

			if(FD_ISSET(i,&readfds)){

				if(i == socket_sv){
					//me llega una nueva conexion
					nueva_conexion = aceptar_conexion(socket_sv);
					FD_SET(nueva_conexion,&master);
					fdmax = (fdmax < nueva_conexion) ? nueva_conexion : fdmax;
				}
				else if(i == kernel_conexion){
					//me llega algo del kernel
					recibir_mensaje(kernel_conexion,&master);
					FD_CLR(kernel_conexion,&master);
					close(kernel_conexion);
				}
				else if(i == socket_cliente){
					//me llega algo de lissandra
				}
				else{
					//me llega algo de la nueva conexion que todavia no me importa
				}

			}


		}

	}
	config_destroy(g_config);
	close(socket_sv);
	return EXIT_SUCCESS;

}

int maximo(int uno, int dos, int tres){

	if(uno > dos){

		if(uno > tres){
			return uno;
		}

		return tres;
	}

	else if(dos > uno){
		if(dos > tres){
			return dos;
		}
		return tres;
	}
	else{
		exit(-1);
	}
}
