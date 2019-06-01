/*
 * random.c
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#include"manejoDeMemoria.h"

///////////////////////////////////////////////////////// MEMORIA ////////////////////////////////////////////////////////////////////////////////////////////

void inicializar_memoria(){

	memoria_principal = malloc(tamanio_memoria);

	tamanio_dato_memoria = sizeof(u_int16_t) + max_value + sizeof(time_t);

	max_paginas_memoria = tamanio_memoria / tamanio_dato_memoria;

}

void ingresar_dato_a_memoria(dato_en_memoria* nuevo_dato, void** referencia_a_memoria){



}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// SEGMENTOS /////////////////////////////////////////////////////////////////////////////////////////

void inicializar_tabla_segmentos(t_list* tablas_a_inicializar){ // tablas a inicializar tiene en "data" un puntero a char*

	int posicion_lista = 0;  //lo utilizo para ambas listas
	char* nombre_tabla;
	segmento* nuevo_segmento;
	int tamanio_lista = list_size(tablas_a_inicializar);

	tabla_segmentos = list_create();

	while(posicion_lista < tamanio_lista){

		nombre_tabla = (char *)list_get(tablas_a_inicializar, posicion_lista); //agarro el char*

		nuevo_segmento = crear_segmento(nombre_tabla);

		list_add(tabla_segmentos , nuevo_segmento);

		posicion_lista++;
	}

	//tablas a inicializar se liberara en la funcion HANDSHAKE

}

segmento* crear_segmento(char* nombre_tabla){

	t_list* tabla_paginas = list_create();

	segmento* nuevo_segmento = malloc(sizeof(segmento));

	nuevo_segmento->nombre_tabla = nombre_tabla;

	nuevo_segmento->Tabla_paginas = tabla_paginas;

	return nuevo_segmento;
}

void liberar_segmento(segmento* segmento_a_liberar){

	free(segmento_a_liberar->nombre_tabla);
	liberar_tabla_paginas(segmento_a_liberar->Tabla_paginas);
	free(segmento_a_liberar);

}

bool existe_segmento(char* nombre_tabla, segmento** segmento_encontrado){

	bool _condicion_de_segmento(void* segmento_a_analizar){

		char* nombre_a_analizar = ((segmento *)segmento_a_analizar)->nombre_tabla;

		if(strcmp(nombre_a_analizar, nombre_tabla)){
			return false;
		}else{
			return true;
		}

	}

	*segmento_encontrado = list_find(tabla_segmentos, _condicion_de_segmento);

	return *segmento_encontrado != NULL;      // podria poner " ? true : false " para que quede mas entendible, pero es como medio caca

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////// PAGINAS //////////////////////////////////////////////////////////////////////////////////////////

void liberar_tabla_paginas(t_list* paginas_a_liberar){} //completar

bool existe_pagina(segmento* segmento_tabla, u_int16_t key, pagina** pagina_encontrada){

	t_list* tabla_paginas = segmento_tabla->Tabla_paginas;

	bool _condicion_pagina(void* pagina_a_analizar){

		dato_en_memoria* dato_a_analizar = (dato_en_memoria *)((pagina *)pagina_a_analizar)->referencia_memoria;

		if(dato_a_analizar->key == key){
			return true;
		}else{
			return false;
		}

	}

	*pagina_encontrada = list_find(tabla_paginas, _condicion_pagina);

	return *pagina_encontrada != NULL;          // podria poner " ? true : false " para que quede mas entendible, pero es como medio caca

}

pagina* crear_pagina(dato_en_memoria* nuevo_dato){

	pagina* nueva_pagina = malloc(sizeof(pagina));

	void* referencia_a_memoria;

	ingresar_dato_a_memoria(nuevo_dato, void** referencia_a_memoria);



}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
