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
	char* ip = "lol";
	int puerto;

	ingresar_A_Un_binario("Tabla_A" , 7);


	pthread_t conexion_memoria;

	pthread_create(&conexion_memoria , NULL , conectar_memoria, NULL);



//	obtener_puerto_ip(&puerto,&ip);

	//realizar_select("Tabla_A" , 233);
	/*
	insert("Tabla_A", 7 , "HOLITAS" ,  1400 );
	insert("Tabla_A", 29 , "Segundo" ,  1450 );
	insert("Tabla_B", 73 , "CHINO COCHINO" ,  140);
	*/

	//create("Tabla_M", "SHC", 1, 50);
//	describe_especifico("Tabla_A");
//	crear_Binario("Tablas/Tabla_A", 7, "HOLITAS", 1400);
//	verificar_Binario();

	///////////////////////////////////////////////////////////////////////
	//ESTO ES PARA PROBAR QUE SE INSERTEN BIEN LAS COSAS EN LA MEMTABLE
	//Esta funcion es unicamente para que la API_Lissandra nos envie la memtable y podamos recorrerla
/*	tabla_memtable* aux = conseguirMemtable();
	bloque_tabla* auxbloque;
	while(aux != 0){
		auxbloque = aux->primer_bloque;

		printf("Nombre tabla: %s \n", aux->nombre_tabla);
		while(auxbloque != 0){
			printf("Key: %d \n", auxbloque->dato_t->key);
			printf("Value: %s \n", auxbloque->dato_t->value);
			auxbloque = auxbloque->dato_sig;
		}
		printf("hola \n");
		aux = aux->sig_tabla;
		printf("chau \n");

		printf("asd \n");
	}
*/
	////////////////////////////////////////////////////////////////////////

	/*

	socket_sv = iniciar_servidor( ip ,puerto );

	socket_pool = aceptar_conexion(socket_sv);

	recibir_mensaje(socket_pool);

	close(socket_sv);
	config_destroy(g_config);
	*/
	//realizar_select("Tabla_A" , 7);

	pthread_join(conexion_memoria, NULL);
	//char* final = obtenerPath_ParticionTabla("Tabla_C", 1);


	return EXIT_SUCCESS;

}

void* conectar_memoria(){

	int puerto = 4445;
	char* ip = "127.0.0.1";
	int socket_cliente;
	socket_sv = iniciar_servidor(ip, puerto);
	dato_t* select_enviar = malloc(sizeof(dato_t));


	socket_cliente = aceptar_conexion(socket_sv);

	//while(1){  el while no espera a recibir una solicitud nueva, manda muchas veces lo mismo

		operacion_select* datos_select = recibir_solicitud(socket_cliente);
		printf("la tabla recibida es: %s\n" , datos_select->nombre_tabla );
		printf("la key solicitada es: %d\n\n " , datos_select->key);

		select_enviar = realizar_select(datos_select->nombre_tabla, datos_select->key);

		printf("\n\nlo que recibe es : %s\n\n" , select_enviar->value);

		mandar_select(socket_cliente, select_enviar);

		free(datos_select);


	//}

	close(socket_sv);
	return NULL;
}


