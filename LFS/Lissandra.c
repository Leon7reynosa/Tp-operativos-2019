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
	char aux[3];

	strcat(aux, *ip);
	printf("%s", aux);


//	obtener_puerto_ip(&puerto,&ip);

//	realizar_select("Tablas/Tabla_A" , 233);

//	insert("Tablas/Tabla_A", 7 , "HOLITAS" ,  1400 );
//	create("Tablas/Tabla_B", "SHC", 1, 50);
//	describe();
//	crear_Binario("Tablas/Tabla_A", 7, "HOLITAS", 1400);
//	verificar_Binario();

//	insert("Tablas/Tabla_A", 8 , "CHAUSITO" ,  1450 );
//	insert("Tablas/Tabla_A", 8 , "CHAUSITO" ,  1450 );
//	insert("Tablas/Tabla_A", 8 , "CHAUSITO" ,  1450 );
//	insert("Tablas/Tabla_A", 8 , "CHAUSITO" ,  1450 );

//	insert("Tablas/Tabla_B", 8 , "FEDETEAMO" ,  1450 );
//	insert("Tablas/Tabla_B", 74 , "TEQUIERO3000" ,  1450 );
//	insert("Tablas/Tabla_B", 1 , "HOLALALO" ,  1450 );

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
	printf("IP = %s\n",ip);
	printf("PUERTO = %d\n",puerto);

	socket_sv = iniciar_servidor( ip ,puerto );

	socket_pool = aceptar_conexion(socket_sv);

	recibir_mensaje(socket_pool);

	close(socket_sv);
	config_destroy(g_config);
	*/
	return EXIT_SUCCESS;
}
