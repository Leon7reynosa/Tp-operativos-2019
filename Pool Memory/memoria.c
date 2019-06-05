/*
 * memoria.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"memoria.h"

struct MemoriaEstructura{
	void* memoria_contigua;
	t_list* paginas_en_memoria;
	int index_ultimo_insertado;
	t_list* tabla_segmentos;
	int tamanio;
	int tamanio_value;
	int tamanio_dato;
	int cant_max_datos;
	Memoria seed;

};

Memoria inicializar_memoria(int tamanio, int tamanio_value , int tamanio_dato, t_list* tablas){

	struct MemoriaEstructura* memoria = malloc(sizeof(struct MemoriaEstructura));

	memoria->memoria_contigua  = malloc(tamanio);
	memoria->paginas_en_memoria= list_create();
	memoria->tabla_segmentos   = inicializar_tabla_segmentos(tablas);
	memoria->tamanio 		   = tamanio;
	//memoria->tamanio_value     = tamanio_value;
	memoria->tamanio_dato 	   = tamanio_dato;
	memoria->cant_max_datos    = tamanio / tamanio_dato;

	return memoria;
}

t_list* inicializar_tabla_segmentos(t_list* tablas_a_inicializar){ // tablas a inicializar tiene en "data" un puntero a char*

	t_list* tabla_segmentos = list_create();

	void _meter_segmento(void* nombre_tabla){

		Segmento segmento = crear_segmento((char *) nombre_tabla);

		list_add(tabla_segmentos, segmento);

	}

	list_iterate(tablas_a_inicializar, _meter_segmento);

	return tabla_segmentos;

}

void ingresar_dato_a_memoria(Memoria memoria, Pagina pagina_con_dato){

	//?

}


bool existe_segmento(char* nombre_tabla, Memoria memoria  ,Segmento* segmento_encontrado){

	bool _condicion_de_segmento(void* segmento_a_analizar){

		char* nombre_a_analizar = ((Segmento)segmento_a_analizar)->nombre_tabla;

		return string_equals_ignore_case(nombre_a_analizar, nombre_tabla);

	}

	*segmento_encontrado = list_find(memoria->tabla_segmentos, _condicion_de_segmento);

	return *segmento_encontrado != NULL;

}

void encontrar_espacio_libre(void** referencia_a_memoria){

	if(hay_espacio_libre_en_memoria()){

		//buscarEspacioLibre

	}
	else{

		//empezar Algoritmo de reemplazo

	}
}
