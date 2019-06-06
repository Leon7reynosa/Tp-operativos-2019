/*
 * memoria.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"memoria.h"

struct MemoriaEstructura{
	void* memoria_contigua;
	t_list* paginas;
	t_list* tabla_segmentos;
	int tamanio;
	int cant_max_datos;
	Memoria seed;

};

t_list* inicializar_paginas(Memoria memoria){

	int contador = memoria->cant_max_datos;
	t_list* paginas = list_create();
	int desplazamiento = 0;

	while(contador > 0){

		Pagina nueva_pagina = crear_pagina((Dato)(memoria->memoria_contigua + desplazamiento));
		desplazamiento += tamanio_dato;


		list_add(paginas, nueva_pagina);
		desplazamiento += tamanio_dato;

		contador--;

	}

	return paginas;

}

Memoria inicializar_memoria(int tamanio, int tamanio_value , int tamanio_dato, t_list* tablas){

	struct MemoriaEstructura* memoria = malloc(sizeof(struct MemoriaEstructura));

	memoria->memoria_contigua  = malloc(tamanio);
	memoria->tabla_segmentos   = inicializar_tabla_segmentos(tablas);
	memoria->tamanio 		   = tamanio;
	memoria->cant_max_datos    = tamanio / tamanio_dato;
	memoria->paginas		   = inicializar_paginas(memoria);

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


bool existe_segmento(char* nombre_tabla, Memoria memoria, Segmento* segmento_encontrado){

	bool _condicion_de_segmento(void* segmento_a_analizar){

		char* nombre_a_analizar = ((Segmento)segmento_a_analizar)->nombre_tabla;

		return string_equals_ignore_case(nombre_a_analizar, nombre_tabla);

	}

	*segmento_encontrado = list_find(memoria->tabla_segmentos, _condicion_de_segmento);

	return *segmento_encontrado != NULL;

}

Segmento encontrar_segmento_con_pagina(Memoria memoria, Pagina pagina_a_buscar){

	Segmento segmento_encontrado;

	bool _equals(Pagina pagina_aux){
		return pagina_aux == pagina_a_buscar;
	}

	bool _condicion_segmento(Segmento segmento_a_analizar){

		return list_any_satisfy(segmento_a_analizar->Tabla_paginas, _equals);

	}

	segmento_encontrado = list_find(memoria->tabla_segmentos, _condicion_segmento);

	return segmento_encontrado;

}

bool hay_pagina_libre(Memoria memoria, Pagina* pagina_solicitada){


	*pagina_solicitada = list_find(memoria->paginas, esta_libre);

	return *pagina_solicitada != NULL;

}

void guardar_dato_en_memoria(Dato nuevo_dato, void* posicion_memoria){

	int desplazamiento = 0;

	memcpy(posicion_memoria + desplazamiento, &(nuevo_dato->key), sizeof(nuevo_dato->key));
	desplazamiento += sizeof(nuevo_dato->key);

	memcpy(posicion_memoria + desplazamiento, &(nuevo_dato->timestamp), sizeof(time_t) );
	desplazamiento += sizeof(time_t);

	memcpy(posicion_memoria + desplazamiento, &(nuevo_dato->value), tamanio_value);
	desplazamiento += tamanio_value;

}

void realizar_journal(Memoria memoria){
	printf("journal realizado xD mentira crack, no lo hiciste todavia, te tiro un exit de ondis ;) \n");
	exit(1);
}



Pagina realizar_algoritmo_reemplazo(Memoria memoria){

	Pagina pagina_reemplazada = pagina_menos_usada(memoria->paginas);

	if(pagina_reemplazada == NULL){

		realizar_journal(memoria);

		pagina_reemplazada = pagina_menos_usada(memoria->paginas);

	}

	Segmento segmento_modificado;

	segmento_modificado = encontrar_segmento_con_pagina(memoria, pagina_reemplazada);

	sacar_pagina_segmento(segmento_modificado, pagina_reemplazada);

	return pagina_reemplazada;
}

Pagina solicitar_pagina(Memoria memoria, Dato nuevo_dato){

	Pagina pagina_solicitada;

	if(hay_pagina_libre(memoria, &pagina_solicitada)){

		guardar_dato_en_memoria(nuevo_dato, pagina_solicitada->referencia_memoria);

	}else{

		pagina_solicitada = realizar_algoritmo_reemplazo(memoria);
		guardar_dato_en_memoria(nuevo_dato, pagina_solicitada->referencia_memoria);

	}

	pagina_solicitada->flag_en_uso = 1;


	return pagina_solicitada;

}

