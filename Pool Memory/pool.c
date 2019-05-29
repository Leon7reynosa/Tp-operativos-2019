/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"

//int

int main (void){
	inicializar_memoria();
	inicializar_tablas();


	return EXIT_SUCCESS;
}

void* conectar_kernel(){

	int puerto = 4444;
	char* ip = "127.0.0.1";
	int socket_cliente;

	socket_sv = iniciar_servidor(ip, puerto);


	socket_cliente = aceptar_conexion(socket_sv);

	while(1){
		recibir_mensaje(socket_cliente);
	}

	close(socket_sv);
	return NULL;
}

void* conectar_lissandra(){

	int puerto = 4445;
	char* ip = "127.0.0.1";
	dato_t* dato;

	conexion_lissandra = conectar_servidor(ip,puerto);
	//enviar_request_select(conexion_lissandra, "Tabla_A",7);
	//mandar_mensaje(conexion_lissandra);


	dato = pedir_key_a_LFS(7 , "Tabla_A");

	printf("\n##################Datos Recibidos#################\n");
	printf("Key: %d\n" , dato->key);
	printf("Value: %s\n" , dato->value );
	printf("Timestamp: %d\n" , dato->timestamp);

	close(conexion_lissandra);
	return  NULL;
}

/////////////////////////// INICIALIZAR ESTRUCTURAS ///////////////////////////
void inicializar_memoria(){

	size_memoria_total = obtener_tamanio_memoria(); //2048

	int cant_datos = size_memoria_total/sizeof(dato_tt);

	int size_real = cant_datos*sizeof(dato_tt);

	int memoria_sin_usar = size_memoria_total - size_real;

	int memoriaFinal = size_memoria_total - memoria_sin_usar;

	memoria_principal = malloc(memoriaFinal);

}

void inicializar_tablas(){



}
