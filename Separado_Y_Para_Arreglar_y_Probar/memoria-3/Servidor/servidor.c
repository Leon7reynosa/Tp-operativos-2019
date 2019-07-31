/*
 * servidor.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include"servidor.h"

int iniciar_servidor(char* ip,int puerto){

	//Estructura que contiene la info del servidor
	struct sockaddr_in server_addr;
	int socket_servidor;

	int yes = 1;

	//familia de direcciones IP
	server_addr.sin_family = AF_INET;
	//defino un puerto cualquiera y lo paso a su representacion binaria
	server_addr.sin_port = htons(puerto);
	//le asigno la direccion del pc que estoy usando
	//server_addr.sin_addr.s_addr = INADDR_ANY;
	inet_aton(ip,&(server_addr.sin_addr.s_addr));
	//seteo el resto de la estructura en 0

	memset(&(server_addr.sin_zero),'\0',8);
	//creo el socket servidor para escuchar
	if((socket_servidor = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("Error al crear el socket");
		exit(1);
	}

	//digo que voy a reutilizar la direccion despues
	if(setsockopt(socket_servidor,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) == -1){
		perror("Error en setsockopt");
		exit(1);
	}

	//asocio el socket a un puerto
	if (bind(socket_servidor,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1){
		perror("Error en el bind del socket");
		exit(1);
	}

	//pongo el socket a escuchar conexiones
	if(listen(socket_servidor,SOMAXCONN) == -1){
		perror("Error al escuchar");
		exit(1);
	}

	printf("El servidor esta escuchando...\n");


	return socket_servidor;

}


char* obtener_ip_local(){

	 struct ifaddrs * ifAddrStruct=NULL;
	    struct ifaddrs * ifa=NULL;
	    void * tmpAddrPtr=NULL;

	    char* ip_encontrada = malloc(31);

	    getifaddrs(&ifAddrStruct);

	    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
	        if (!ifa->ifa_addr) {
	            continue;
	        }
	        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
	            // is a valid IP4 Address

	            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
	            char addressBuffer[INET_ADDRSTRLEN];
	            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

	            if( addressBuffer[0] == '1' && addressBuffer[1] == '9' ){ // habria que ver esto

	           	   memcpy(ip_encontrada, addressBuffer , strlen(addressBuffer ) + 1);

	           	   if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
	           	   return ip_encontrada;

	           	}

	        }
	    }

	    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
	    return ip_encontrada;

}



int aceptar_conexion(int socket_listener){

	struct sockaddr_in cliente;
	int socket_conexion;
	int sin_size = sizeof(struct sockaddr_in);

	if((socket_conexion = accept(socket_listener,(struct sockaddr *)&cliente,&sin_size)) == -1){
		perror("Error al aceptar conexion");
		exit(-1);
	}

	char* direccion = inet_ntoa((struct in_addr)cliente.sin_addr);
	int puerto = ntohs(cliente.sin_port);

//	printf("[Servidor] Se conecto el cliente: n°%d -> %s:%d\n",socket_conexion,direccion,puerto);

	if(send(socket_conexion,"[Servidor] Te Conectaste!\n",26,0) == -1){
		perror("No le notifique que se conecto");
		exit(1);
	}

	//free(direccion);

	return socket_conexion;
}
