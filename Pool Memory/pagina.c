/*
 * pagina.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"pagina.h"
/*
struct PaginaEstructura{

	Dato referencia_memoria;  //no libero esto, por que es memoria!
	bool flag_modificado;
	bool flag_en_uso;
	time_t ultimo_uso;

};
*/
void liberar_pagina(Pagina pagina_a_liberar){

	pagina_a_liberar->flag_en_uso = 0;
	pagina_a_liberar->flag_modificado = 1;

}


Pagina crear_pagina(void* posicion_memoria){

	struct PaginaEstructura* pagina = malloc(sizeof(struct PaginaEstructura));

	pagina->referencia_memoria = posicion_memoria;

	pagina->flag_en_uso = 0;

	pagina->flag_modificado = 0;

	/* pagina->ultimo_uso = ?*/

	return pagina;

}

bool esta_libre(Pagina pagina){

	return !(pagina->flag_en_uso);
}

Pagina pagina_menos_usada(t_list* paginas){

	Pagina pagina_encontrada;
	time_t ts_menor;
	t_list* paginas_no_modificadas = paginas_sin_modificar(paginas);
	printf("PAGINAS SIN MODIFICAR: %i\n", paginas_no_modificadas->elements_count);

	if(paginas_no_modificadas->elements_count == 0){

		pagina_encontrada = NULL;


	}else{

		bool _condicion_mas_usado(Pagina pagina){

			return pagina->ultimo_uso >= ts_menor;

		}

		bool _condicion_timestamp_menor(Pagina pagina){

			ts_menor = pagina->ultimo_uso;

			return list_all_satisfy(paginas, _condicion_mas_usado);

		}

		pagina_encontrada = list_find(paginas, _condicion_timestamp_menor);
	}

	list_destroy(paginas_no_modificadas);

	return pagina_encontrada;

}

t_list* paginas_sin_modificar(t_list* paginas){


	bool _sin_modificar(void* _pagina){

		Pagina pagina = (Pagina)_pagina;

		return pagina->flag_modificado < 1;

	}

	return list_filter(paginas, _sin_modificar);

}

void  mostrar_datos(Pagina pagina){


	Dato nuevo_dato = malloc(sizeof(struct DatoEstructura));
	nuevo_dato->value = malloc(tamanio_value);
	memcpy(&(nuevo_dato->timestamp), pagina->referencia_memoria, sizeof(time_t));
	memcpy(&(nuevo_dato->key), (pagina->referencia_memoria) + sizeof(time_t), sizeof(u_int16_t));
	memcpy(nuevo_dato->value, (pagina->referencia_memoria) + sizeof(u_int16_t) + sizeof(time_t), tamanio_value);

	printf("Ahora voy a imprimir la pagina completa!\n");
	printf("timestamp: %i\n",nuevo_dato->timestamp);
	printf("key: %d\n", nuevo_dato->key);
	printf("value: %s\n", nuevo_dato->value);

	liberar_dato(nuevo_dato);
}
