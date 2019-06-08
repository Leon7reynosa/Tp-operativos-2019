/*
 * main.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"main.h"


int main(){

	dato_t* datovich = malloc(sizeof(dato_t));
	datovich->key = 10;
	datovich->timestamp = 123456;
	datovich->value = "Peron es de Liniers";

	dato_t datovichSinPuntero;
		datovichSinPuntero.key = 10;
		datovichSinPuntero.timestamp = 123456;
		datovichSinPuntero.value = malloc(strlen("Peron")+1);
		memcpy(datovichSinPuntero.value, "Peron", strlen("Peron")+1);

	llenarBloque(datovichSinPuntero);

//	crear_Binario_tabla("Tabla_A", 10, "lol", 123456);

	dato_t datoObtenido;
	FILE* f = fopen("2.bin", "rb");
	fread(&datoObtenido, sizeof(dato_t), 1, f);
	printf("\n\n DATOS OBTENIDOS\n");
	mostrarDato(datoObtenido);
	fclose(f);
/*
	pthread_t conexion_memoria;

	pthread_create(&conexion_memoria , NULL , conectar_memoria, NULL);




	pthread_join(conexion_memoria, NULL);
*/
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
