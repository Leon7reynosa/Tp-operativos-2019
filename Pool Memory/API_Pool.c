/*
 * API_Pool.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Pool.h"

void request_select(char* tabla, u_int16_t key){

	segmento* segmento_tabla;

	if(existe_segmento(tabla, &segmento_tabla)){

		printf("Existe el Segmento: %s\n", segmento_tabla->nombre_tabla);

		pagina* pagina_encontrada;

		if(existe_pagina(segmento_tabla, key, &pagina_encontrada)){
			//DEVOLVER EL VALOR
			dato_en_memoria* dato_encontrado = (dato_en_memoria *)pagina_encontrada->flag_modificado;
			printf("El dato (value) encontrado es: %s\n\n", dato_encontrado->value);

		}else{
			//PEDIR AL LFS
			printf("No Existe una pagina con esa key, pedisela al LFS \n\n");
		}

	}else{
		// NO EXISTE LA TABLA, SE LA PIDO AL LFS?
		printf("No Existe el segmento con dicha tabla \n\n");
	}


}
