/*
 * conexiones.c
 *
 *  Created on: 11 jun. 2019
 *      Author: utnso
 */


#include"conexiones.h"

void* conectar_varias_memorias(){

	//Creo que si usamos hilos tendriamos mucho overhead, debido a los context switchs

	fd_set memoriasfds;

	struct sockaddr_in client_address;

	fd_set descriptor_lectura;

	int new_fd;

	int address_len;

	int fdmax;

	int nbytes;

	int socket_servidor = iniciar_servidor(ip_lfs , puerto_lfs);
	FD_ZERO(&memoriasfds);

	FD_SET(socket_servidor , &memoriasfds);

	fdmax = socket_servidor;

	while(0){

		descriptor_lectura = memoriasfds;
		//En el 5to parametro, va NULL asi se queda esperando hasta que pase algo
		if(select(fdmax + 1 , &descriptor_lectura , NULL, NULL, NULL) == -1){
			perror("error del select.");
			exit(1);
		}

		for(int i = 0 ; i<=fdmax ; i++){

			if(FD_ISSET(i, &descriptor_lectura)){
				if(i == socket_servidor){

					address_len = sizeof(client_address);

					if((new_fd = aceptar_conexion(&client_address)) == -1){

						perror("fallo el accept del select.");

					}else{

						FD_SET(new_fd , &memoriasfds);
						if(new_fd > fdmax){
							fdmax = new_fd;
						}

						ptrinf("select server: new connection from %s on socket %d\n" , inet_ntoa(client_address.sin_addr) , new_fd);

					}
				}else{

					//En estos casos vamos a hacer que no se puedan desconectar las memorias.

					request nueva_request = recibir_request(i);

					responder_request(nueva_request, i);

					liberar_request(nueva_request);

					}
				}
			}
		}
	return NULL;
}

/*
void* conectar_varias_memorias_versionChinua(){
	int cantidadDeMemorias;

	fd_set descriptoresLectura;
	int socketServidor;
	int socketMemoria[cantidadDeMemorias];

	FD_ZERO(&descriptoresLectura);
	FD_SET(socketServidor, &descriptoresLectura);

	for(int i = 0; i < cantidadDeMemorias; i++){
		FD_SET(socketMemoria[i], &descriptoresLectura);
	}

	//verificar socketServidor para ver si hay un nuevo cliente y todos los socketMemoria[]
	//para ver si nos han enviado algo o cerrado el socket

	select (cantidadDeMemorias + 1, &descriptoresLectura, NULL, NULL, NULL);

	//TRATEMIENTO DE CONEXIONES DE MEMORIAS
	for(int j = 0; j < cantidadDeMemorias; j++){
		if(FD_ISSET(socketMemoria[j], &descriptoresLectura)){
			if((Lee_Socket(socketMemoria[j], (char*)&buffer, sizeof(int)) > 0)){
				//tratamiento del request que va a recibir
				//Lee Socket es lo mismo que la funcion read()
			}
			else{
				//ERROR
			}
		}
	}

	if(FD_ISSET(socketServidor, &descriptoresLectura)){
		//CREO QUE SERVIRIA PARA EL ADD DE MEMORIAS
	}



}
*/
