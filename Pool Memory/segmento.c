/*
 * segmento.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"segmento.h"

struct SegmentoEstructura{

	char* nombre_tabla;
	t_list* Tabla_paginas; //en el campo "data" va a contener un puntero a una estructura "pagina"


};


Segmento crear_segmento(char* nombre_tabla){

	struct SegmentoEstructura* segmento = malloc(sizeof(struct SegmentoEstructura));

	segmento->nombre_tabla = nombre_tabla;

	segmento->Tabla_paginas = list_create();

	return segmento;
}

void liberar_tabla_paginas(t_list* tabla_paginas){

	list_clean_and_destroy_elements(tabla_paginas, liberar_pagina);
}

void liberar_segmento(Segmento segmento_a_liberar){

	free(segmento_a_liberar->nombre_tabla);
	liberar_tabla_paginas(segmento_a_liberar->Tabla_paginas);
	free(segmento_a_liberar);

}

bool existe_pagina(Segmento segmento_tabla, u_int16_t key, Pagina* pagina_encontrada){

	t_list* tabla_paginas = segmento_tabla->Tabla_paginas;

	bool _condicion_pagina(void* pagina_a_analizar){

		Dato dato_a_analizar = (Dato)((Pagina)pagina_a_analizar)->referencia_memoria;

		return dato_a_analizar->key == key;

	}

	*pagina_encontrada = list_find(tabla_paginas, _condicion_pagina);

	return *pagina_encontrada != NULL;          // podria poner " ? true : false " para que quede mas entendible, pero es como medio caca

}

//esta funcion te compara el puntero void* dato con el puntero que tiene la lista, osea te compara dos posiciones de memoria
int list_get_index(t_list* lista, void* dato){

	int index;
	void* dato_aux;

	for(index = 0 ; index< lista->elements_count ; index++){
		dato_aux = list_get_element(lista, index);

		if(dato == dato_aux){

			return index;
		}

	}

	return -1;
}

void sacar_pagina_segmento(Segmento segmento, Pagina pagina){

	int index = list_get_index(segmento->Tabla_paginas, pagina);

	list_remove(segmento->Tabla_paginas, index);

	pagina->flag_en_uso = 0;

}
