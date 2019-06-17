/*
 * main.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"main.h"


int main(){

	crearYObtenerDatos();

//	creacion_bitmap();
	setEstado(2, LIBRE);
	dato_t* datito = malloc(sizeof(dato_t));
	datito->key = 123;
	datito->timestamp = 123456;
	datito->value = malloc(strlen("Peron")+1);
	memcpy(datito->value, "Peron", strlen("Peron")+1);

	char* datoString = datoEnFormatoBloque(datito);
//	printf("%s\n", datoString);

//	llenarBloque(datoString, 2);


	leerBloque(0);
	leerBloque(1);
	leerBloque(2);
/*
	char* pathBloque = obtenerPath_Bloque(0);
	int ficheroLectura = open(pathBloque, O_RDONLY, S_IRUSR);
	int posicion = lseek(ficheroLectura, 0, SEEK_END);
	printf("La proxima posicion a ocupar de dicho bloque es la posicion: %i\n", posicion);

	getAllEstados();
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
