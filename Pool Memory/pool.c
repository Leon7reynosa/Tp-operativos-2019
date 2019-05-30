/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"

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

	conexion_lissandra = conectar_servidor(ip,puerto);
	//enviar_request_select(conexion_lissandra, "Tabla_A",7);
	//mandar_mensaje(conexion_lissandra);

	close(conexion_lissandra);
	return  NULL;
}

/////////////////////////// INICIALIZAR ESTRUCTURAS ///////////////////////////
void inicializar_memoria(){

	tamanio_memoria = obtener_tamanio_memoria(); //2048

	int cant_datos = tamanio_memoria/sizeof(dato_t);

	int size_real = cant_datos*sizeof(dato_t);

	int memoria_sin_usar = tamanio_memoria - size_real;

	int memoriaFinal = tamanio_memoria - memoria_sin_usar;

	memoria_principal = malloc(memoriaFinal);

}

void inicializar_tablas(){

	//hardocodeado duro, tendria que salir del handshake
	crear_segmento("Tabla_A");
	crear_segmento("Tabla_B");


}
