/*
 * utilServer.c
 *
 *  Created on: 11 abr. 2019
 *      Author: utnso
 */

#include"utilServer.h"

int iniciar_servidor(void)
{
	int socket_servidor;

	t_log* serverLogger = log_create("server.log", "Servidor", 1, LOG_LEVEL_DEBUG);

    struct addrinfo hints, *servinfo, *p; //contienen información de la direccion
    									 //del servicio solicitado

    memset(&hints, 0, sizeof(hints)); //iniciarlo como genérico
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    //traduce el nombre del servicio para establecer la direccion del socket
    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
        	perror("error en el bind");
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    log_trace(serverLogger, "Listo para escuchar a mi cliente");

    log_destroy(serverLogger);

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);
	t_log* serverLogger = log_create("server.log", "Servidor", 1, LOG_LEVEL_INFO);
	//Hay que usar LOG_LEVEL_INFO, xq abajo esta log_info xd

	int socket_cliente;
	if((socket_cliente = accept(socket_servidor, (struct sockaddr*) &dir_cliente, &tam_direccion)) == -1){
		perror("Error al aceptar el cliente");
	};

	log_info(serverLogger, "Se conecto el cliente: %s\n", inet_ntoa(dir_cliente.sin_addr));

	log_destroy(serverLogger);

	return socket_cliente;
}

//ESTO ES PROTOCOLO, yo voy a recibir del socket cliente UN COD OPERACION (VER Utils.c)
int recibir_operacion(int socket_cliente)
{

	int cod_op;
	//recibo el Cod OP, sizeof(int) por que ya se que va a ser un entero, recordar: [INT][INT][MENSAJE]
	//distinto de 0 por que recv me devuelve los bytes que tiene el mensaje, 0 cero si no metio nada
	//entonces va copiar el cod_op (los primero sizeof(int) bytes en la direccion COD_OP
	//esos bytes van a contener un numero, que si es cero o lo que sea se encarga el SWITCH DEL SERVER de ver que hace
	if(recv(socket_cliente,&cod_op,sizeof(int),MSG_WAITALL)!= 0){
		return cod_op;
	}
	//si el entero es 0, significa que no metio nada, entonces cierro el cliente, lo desconecto.
	else{
		close(socket_cliente);
		return -1;
	}

}

//recibo [INT][MENSAJE]
void* recibir_buffer(int* size, int socket_cliente){
	void* buffer;

	recv(socket_cliente,size,sizeof(int),MSG_WAITALL);
	//ver aca si necesita espacio para /0
	buffer = malloc(*size);
	//[MENSAJE]
	recv(socket_cliente,buffer, *size,MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente){

	t_log* serverLogger = log_create("server.log","Servidor",1,LOG_LEVEL_DEBUG);
	int size;
	//voy a recibir el resto del mensaje: [INT][MENSAJE] y voy a sacar cuanto es el tamaño
	char* buffer = recibir_buffer(&size,socket_cliente);

	log_info(serverLogger,"me llego el mensaje: %s ",buffer);

	free(buffer);
	log_destroy(serverLogger);


}


