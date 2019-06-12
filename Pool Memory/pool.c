/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"

int main (void){


	tamanio_value = 50;
	tamanio_dato = tamanio_value + 1 + sizeof(u_int16_t) + sizeof(time_t);
	t_list* tablas = list_create();
	list_add(tablas, "Tabla_A" );


	obtener_datos_config();

	printf("Me voy a conectar a lissandra\n");
	socket_lissandra = conectar_servidor(ip_lfs, puerto_lfs);

	inicializar_memoria(tamanio, tamanio_value, tamanio_dato, tablas );
	printf("Le mando una request\n");
	request_select("Tabla_A", 7);

	printf("Termino el programa memoria\n");


	return EXIT_SUCCESS;
}

void pruebas(void){
	tamanio_value = 50;
	tamanio_dato = tamanio_value + 1 + sizeof(u_int16_t) + sizeof(time_t);
	char* tabla_a = "Tabla_A";

	obtener_datos_config();

	t_list* lista_tablas = list_create();
	list_add(lista_tablas, (void*)tabla_a);

	inicializar_memoria(tamanio, tamanio_value, 51 + sizeof(u_int16_t) + sizeof(time_t), lista_tablas);

	int i;
	printf("Cantidad de paginas: %i\n", memoria->paginas->elements_count);

	for( i = 0; i < 35; i++){
			printf("/////////////////// INSERT n°%i /////////////////////\n", i + 1);
			request_insert("Tabla_A", i + 1, "Hola, soy alguna prueba.\n");
	}


	printf("Pruebas finalizadas con exito!\n");
}
