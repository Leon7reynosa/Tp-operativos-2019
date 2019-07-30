/*
 * pagina.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"pagina.h"

void eliminar_pagina(Pagina pagina_a_eliminar){

	free(pagina_a_eliminar);

}

void liberar_pagina(Pagina pagina_a_liberar){

	pagina_a_liberar->flag_en_uso = 0;
	pagina_a_liberar->flag_modificado = 0;  //esto estaba en 1 y lo cambie a 0, deberia estar bien asi pero .-.


	//agregar (opcional) que toda la referencia a memoria de la pagina tenga /0 y no otra basura
}


Pagina crear_pagina(void* posicion_memoria){

	struct PaginaEstructura* pagina = malloc(sizeof(struct PaginaEstructura));

	pagina->referencia_memoria = posicion_memoria;

	pagina->flag_en_uso = 0;

	pagina->flag_modificado = 0;

	pagina->ultimo_uso = 0;

	return pagina;

}

bool esta_libre(Pagina pagina){

	return !(pagina->flag_en_uso);
}

//TODO VER si es necesario hacer las paginas sin modificar adentro, o que directamente me pasen las paginas
Pagina pagina_menos_usada(t_list* paginas){

	Pagina pagina_encontrada;
	time_t ts_menor;
	t_list* paginas_no_modificadas = paginas_sin_modificar(paginas);
	printf("[REEMPLAZO] PAGINAS SIN MODIFICAR: %i\n", list_size(paginas_no_modificadas));

	if(list_size(paginas_no_modificadas) == 0){

		pagina_encontrada = NULL;


	}else{

		bool _condicion_mas_usado(Pagina pagina){

			return pagina->ultimo_uso >= ts_menor;

		}

		bool _condicion_timestamp_menor(Pagina pagina){

			ts_menor = pagina->ultimo_uso;

			return list_all_satisfy(paginas_no_modificadas, _condicion_mas_usado);

		}

		pagina_encontrada = list_find(paginas_no_modificadas, _condicion_timestamp_menor);
	}

	list_destroy(paginas_no_modificadas);

	return pagina_encontrada;

}

bool sin_modificar(Pagina pagina){

		return !pagina_modificada(pagina);

}

bool pagina_modificada(Pagina pagina){

	return pagina->flag_modificado;
}

t_list* paginas_modificadas(t_list* paginas){

	return list_filter(paginas, pagina_modificada);

}


t_list* paginas_sin_modificar(t_list* paginas){

	return list_filter(paginas, sin_modificar);

}

void  mostrar_datos(Pagina pagina){

	char nulo = '\0';
	Dato nuevo_dato = malloc(sizeof(struct DatoEstructura));

	nuevo_dato->value = malloc(tamanio_value + 1);

	memcpy(&(nuevo_dato->timestamp), pagina->referencia_memoria, sizeof(time_t));
	memcpy(&(nuevo_dato->key), (pagina->referencia_memoria) + sizeof(time_t), sizeof(u_int16_t));
	memcpy(nuevo_dato->value, (pagina->referencia_memoria) + sizeof(u_int16_t) + sizeof(time_t), tamanio_value);
	memcpy(nuevo_dato->value + tamanio_value, &nulo, 1);

	printf("\n[DATOS] Ahora voy a imprimir la pagina completa!\n");
	printf("timestamp: %d\n",nuevo_dato->timestamp);
	printf("key: %d\n", nuevo_dato->key);
	printf("value: %s\n\n", nuevo_dato->value);

	liberar_dato(nuevo_dato);
}

void actualizar_uso(Pagina pagina){
	pagina->ultimo_uso = time(NULL);
}
