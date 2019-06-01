/*
 * test1.c
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#include"test1.h"

void test1(){
	printf("Test 1\n\n");
	printf("Deberia decirme que existe el segmento \"Tabla_A\" pero no existe una pagina con la key\n");
	inicializar_tabla_segmentos(tablas_LFS());

	printf("Respuesta: \n");
	request_select("Tabla_A",7);

	printf("Finalizo el test1! \n\n");

}

t_list* tablas_LFS(){

	t_list* tablas_LFS = list_create();


	char* tabla_a = malloc(8);
	strcpy(tabla_a, "Tabla_A");

	char* tabla_b = malloc(8);
	strcpy(tabla_b, "Tabla_B");

	list_add(tablas_LFS, tabla_a);
	list_add(tablas_LFS, tabla_b);

	return tablas_LFS;

}

void test2(){
	printf("Test2 \n");



}
