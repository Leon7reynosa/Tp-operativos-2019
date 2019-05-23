/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"



int main (void){
	tamanio_memoria = obtener_tamanio_memoria();
	memoria_principal = malloc(tamanio_memoria);

	pruebita();

	printf("Vamos con el select \n\n");
	realizar_select("Tabla_A",1);

	pthread_t conexion_lissandra;


	/*
	pthread_t conexion_kernel;


	pthread_create(&conexion_kernel , NULL , conectar_kernel , NULL);
	pthread_create(&conexion_lissandra, NULL, conectar_lissandra, NULL);


	pthread_join(conexion_kernel , NULL);
	*/

	// esto es del handshake, tabla_A y tabla_B

	pthread_join(conexion_lissandra , NULL);

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

	mandar_mensaje(conexion_lissandra);

	close(conexion_lissandra);
	return  NULL;
}

void pruebita(){
		segmento* segmento_prueba = malloc(sizeof(segmento));
		pagina* pagina_prueba = malloc(sizeof(pagina));

		pagina_prueba->dato.key = 1;
		pagina_prueba->dato.timestamp = 50;
		pagina_prueba->dato.value = malloc(TAMANIO_MAX_VALUE);
		memcpy(pagina_prueba->dato.value,"vamo niubel",strlen("vamo niubel") + 1);

		printf("VALOR: %s \n" , pagina_prueba->dato.value);

		pagina_prueba->numero_pagina = 1;
		pagina_prueba->flag_modificado = FALSE;
		pagina_prueba->siguiente_pagina = NULL;


		segmento_prueba->primera_pagina = pagina_prueba;
		segmento_prueba->siguiente_segmento = NULL;
		segmento_prueba->tabla = "Tabla_A";

		printf("value desde el segmento: %s\n",segmento_prueba->primera_pagina->dato.value);

		memcpy(memoria_principal,segmento_prueba,sizeof(segmento));

		printf("value desde la memoria: %s\n\n",memoria_principal->primera_pagina->dato.value);
}
