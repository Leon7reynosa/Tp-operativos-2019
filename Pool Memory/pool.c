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

	char* tabla_a = "Tabla_A";

	obtener_datos_config();

	t_list* lista_tablas = list_create();

	list_add(lista_tablas, (void*)tabla_a);

	inicializar_memoria(tamanio, tamanio_value, 51 + sizeof(u_int16_t) + sizeof(time_t), lista_tablas);


	printf("/////////////////// INSERT /////////////////////\n");
	request_insert("Tabla_A", 7, "Hola, soy una prueba un poco mas larga.");
	printf("/////////////////// INSERT /////////////////////\n");
	request_insert("Tabla_A", 8, "Hola, soy una prueba que parezco corta, pero no deberia ejecutar");
	printf("/////////////////// INSERT /////////////////////\n");
	request_insert("Tabla_A", 8, "Soy la tercer prueba, aguante boca");
	printf("/////////////////// INSERT /////////////////////\n");
	request_insert("Tabla_B", 10, "IUJU");
	printf("/////////////////// INSERT /////////////////////\n");
	request_insert("Tabla_A", 10, "SOY LA QUINTA PRUEBA");
	printf("/////////////////// SELECT /////////////////////\n");
	request_select("Tabla_A", 10 );
	printf("/////////////////// INSERT /////////////////////\n");
	request_insert("Tabla_A", 10, "Reescribi a la quinta :D");
	printf("/////////////////// SELECT /////////////////////\n");
	request_select("Tabla_A", 10 );
	printf("/////////////////// SELECT /////////////////////\n");
	request_select("Tabla_A", 7 );

	return EXIT_SUCCESS;
}
