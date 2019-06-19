/*
 * main.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"main.h"


int main(){
	crearYObtenerDatos();
	creacion_bitmap();
	setAllEstados(LIBRE);

	dato_t* dato = malloc(sizeof(dato_t));
	dato->key = 12;
	dato->timestamp = 1111;
	dato->value = malloc(strlen("Peron")+1);
	memcpy(dato->value, "Peron", strlen("Peron") + 1);

	char* datoStringueado = datoEnFormatoBloque(dato);
//	crear_archivos_particiones("Tabla_A" , 4);
	//llenarBloque(datoStringueado, 0);
	leer_Particiones("Tabla_A");

	return EXIT_SUCCESS;
}


void* conectar_memoria(){
	int puerto = 4445;
	char* ip = "127.0.0.1";
	int socket_cliente;
	int socket_sv = iniciar_servidor(ip, puerto);
	request request;

	socket_cliente = aceptar_conexion(socket_sv);

	request = recibir_request(socket_cliente);

	printf("codigo de operacion: %d\n" , request->cod_op);




	//faltaria mandar la respuesta

	close(socket_sv);
	return NULL;
}


