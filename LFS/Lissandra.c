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

	insert("Tabla_A" , 7, "leonidas" , 1231231);
	insert("Tabla_B" , 56, "nawell" , 4546 );
	insert("Tabla_A" , 8, "lalo callate", 789789);
/*
	dato_t* dato_prueba;
	dato_t* dato_obtenido;
	dato_t* dato_prueba_dos;
	dato_t* dato_prueba_tres;
	dato_t* dato_prueba_cuatro;

	dato_prueba = crear_dato(7 , "lo mas vital" , 3800);
	dato_prueba_dos = crear_dato(7 , "vamos bokita" , 2930);
	dato_prueba_cuatro = crear_dato(5 , "vamo niuls" , 12930);
	dato_prueba_tres = crear_dato(7 , "vamo niuls" , 1930);

	ingresar_A_Un_binario("Tabla_A" , 7);
	ingresar_a_memtable(dato_prueba , "Tabla_A");
	ingresar_a_memtable(dato_prueba_dos , "Tabla_B");
	ingresar_a_memtable(dato_prueba_cuatro, "Tabla_A");
	ingresar_a_memtable(dato_prueba_tres, "Tabla_B");



	dato_obtenido = realizar_select("Tabla_A" , 7);

	printf("\n########DATO OBTENIDO###########\n");
	printf("CLAVE : %d\n" , dato_obtenido->key);
	printf("VALOR : %s\n" , dato_obtenido->value);
	printf("TIMESTAMP: %d\n" , dato_obtenido->timestamp);

*/
	//pthread_t conexion_memoria;

	//pthread_create(&conexion_memoria , NULL , conectar_memoria, NULL);


//	obtener_puerto_ip(&puerto,&ip);

	//realizar_select("Tabla_A" , 233);
	/*
	insert("Tabla_A", 7 , "HOLITAS" ,  1400 );
	insert("Tabla_A", 29 , "Segundo" ,  1450 );
	insert("Tabla_B", 73 , "CHINO COCHINO" ,  140);
	*/

	describe();
//	crear_Binario("Tabla_A", 7, "HOLITAS", 1400);


	///////////////////////////////////////////////////////////////////////
	//ESTO ES PARA PROBAR QUE SE INSERTEN BIEN LAS COSAS EN LA MEMTABLE
	//Esta funcion es unicamente para que la API_Lissandra nos envie la memtable y podamos recorrerla

	printf("\n##############MEMTABLE################\n");
	tabla_memtable* aux = conseguirMemtable();
	bloque_tabla* auxbloque;
	while(aux != 0){
		auxbloque = aux->primer_bloque;

		printf("Nombre tabla: %s \n", aux->nombre_tabla);
		while(auxbloque != NULL){
			printf("\nKey: %d \n", auxbloque->dato_t->key);
			printf("Value: %s \n", auxbloque->dato_t->value);
			printf("timestamp; %d\n", auxbloque->dato_t->timestamp);
			auxbloque = auxbloque->dato_sig;
		}
		aux = aux->sig_tabla;
	}

	////////////////////////////////////////////////////////////////////////

	/*

	socket_sv = iniciar_servidor( ip ,puerto );

	socket_pool = aceptar_conexion(socket_sv);

	recibir_mensaje(socket_pool);

	close(socket_sv);
	config_destroy(g_config);
	*/
	//realizar_select("Tabla_A" , 7);

	//pthread_join(conexion_memoria, NULL);
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



		mandar_select(socket_cliente, select_enviar);

		free(datos_select);


	//}

	close(socket_sv);
	return NULL;
}


