/*
 * API_Pool.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Pool.h"

Dato request_select(Memoria memoria, char* tabla, u_int16_t key){

	Segmento segmento_tabla;
	Pagina pagina_encontrada;
	Dato dato_encontrado;

	if(existe_segmento(tabla, memoria ,&segmento_tabla)){

		printf("Existe el Segmento: %s\n", segmento_tabla->nombre_tabla);

		if(existe_pagina(segmento_tabla, key, &pagina_encontrada)){
			dato_encontrado = (Dato)pagina_encontrada->flag_modificado;

			printf("El dato (value) encontrado es: %s\n\n", dato_encontrado->value);

		}else{

			//VER ESTO, si se puede hacer una funcion para no repetir logica
			printf("Le pido las cosas al LFS \n\n");

			Dato dato_lfs = pedir_dato_al_LFS(tabla, key);

			pagina_encontrada = solicitar_pagina(memoria , dato_lfs);

			dato_encontrado = pagina_encontrada->referencia_memoria;

			printf("El dato (Value) encontrado es: %s \n\n", dato_encontrado->value);

			liberar_dato(dato_lfs);
		}

	}else{
		//VER ESTO
		/*
		agregar_segmento(tabla, memoria->tabla_segmentos);

		Dato dato_lfs = pedir_dato_al_LFS(tabla, key);

		printf("No Existe el segmento con dicha tabla \n\n");
		*/
	}

	return dato_encontrado;
}

void request_insert(Memoria memoria, char* tabla, u_int16_t key, char* value ){

	Segmento segmento_tabla;
	Pagina pagina_encontrada;
	Dato dato_insert;

	if(string_length(value) > tamanio_value){
		printf("SEGMENTATION FAULT! Te pasaste en el tamaño del value \n");
		return;
	}

	time_t timestamp; ////// HAY QUE GUARDAR ESTO BIEN CON EL TIEMPO ACTUAL

	dato_insert = crear_dato(key, value, timestamp );

	if(existe_segmento(tabla, memoria, &segmento_tabla)){

		if(existe_pagina(segmento_tabla, key, &pagina_encontrada)){

			actualizar_pagina(pagina_encontrada, dato_insert); //VER ESTO

		}else{

			solicitar_pagina(memoria, dato_insert); //SEGUIR Y VER

		}


	}else{
		// DEFINITIVAMENTE VER
		//generar el segmento con el dato solicitando una pagina

	}

	liberar_dato(dato_insert);
}


void request_create(Memoria memoria,  char* tabla, char* consistencia, int numero_particiones, time_t compactacion  ){

	create dato_create = crear_dato_create(tabla, consistencia, numero_particiones, compactacion);
	enviar_request(CREATE, dato_create);

	//seguir!

}
