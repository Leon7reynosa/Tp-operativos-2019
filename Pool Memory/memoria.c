/*
 * memoria.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"memoria.h"
/*
struct MemoriaEstructura{
	void* memoria_contigua;
	t_list* paginas;
	t_list* tabla_segmentos;
	int tamanio;
	int cant_max_datos;
	Memoria seed;

};
*/

t_list* inicializar_paginas(){

	int contador = memoria->cant_max_datos;

	printf("CANT MAX DE DATOS: %i\n", contador);

	printf("tamanio_dato = %i\n", tamanio_dato);

	t_list* paginas = list_create();

	int desplazamiento = 0;

	while(contador > 0){

		Pagina nueva_pagina = crear_pagina((memoria->memoria_contigua + desplazamiento));
		desplazamiento += tamanio_dato;


		list_add(paginas, nueva_pagina);

		contador--;

	}

	printf("DESPLAZAMIENTO: %i\n", desplazamiento);

	return paginas;

}

void inicializar_memoria(int tamanio, int tamanio_value , int tamanio_dato){

	memoria = malloc(sizeof(struct MemoriaEstructura));
	memoria->memoria_contigua  = malloc(tamanio);

	memoria->tabla_segmentos   = list_create();
	memoria->tamanio 		   = tamanio;
	memoria->cant_max_datos    = tamanio / tamanio_dato;
	memoria->paginas		   = inicializar_paginas();

	memoria->seed = list_create();
	inicializar_seeds();

}

void inicializar_seeds(void){

	int cantidad_ips = list_size(ip_seeds);
	int cantidad_puertos = list_size(puerto_seeds);

	if(cantidad_ips != cantidad_puertos){
		printf("No hay la misma cantidad de ips_seeds que de puertos_seeds");
		exit(1);
	}

	char* ip;
	int* puerto;

	Seed nueva_seed;

	for(int i = 0; i < cantidad_ips; i++){

		ip = list_get(ip_seeds, i);
		puerto = list_get(puerto_seeds, i);

		nueva_seed = crear_seed(0, ip, *((int *)puerto));

		list_add(memoria->seed, nueva_seed);

	}

}

//mirar si LFS me pasa las tablas, o las memorias empiezan siempre con una tabla de segmentos vacia
t_list* inicializar_tabla_segmentos(t_list* tablas_a_inicializar){ // tablas a inicializar tiene en "data" un puntero a char*

	t_list* tabla_segmentos = list_create();

	void _meter_segmento(void* nombre_tabla){

		Segmento segmento = crear_segmento((char *) nombre_tabla);

		list_add(tabla_segmentos, segmento);

	}

	list_iterate(tablas_a_inicializar, _meter_segmento);

	return tabla_segmentos;

}

Seed crear_seed(int numero , char* ip, int puerto){

	Seed nueva_seed = malloc(sizeof(struct seedEstructura));

	nueva_seed->numero_memoria = numero;
	nueva_seed->puerto = puerto;

	nueva_seed->ip = malloc(string_length(ip) + 1);
	memcpy(nueva_seed->ip, ip, string_length(ip) + 1);

	return nueva_seed;
}

void liberar_seed(Seed seed_a_liberar){
	free(seed_a_liberar->ip);
	free(seed_a_liberar);
}

void eliminar_segmentos(void){

	list_clean_and_destroy_elements(memoria->tabla_segmentos, liberar_segmento);

}

void sacar_segmento(Segmento segmento){

	int index = list_get_index(memoria->tabla_segmentos, segmento);

	list_remove(memoria->tabla_segmentos, index);

	liberar_segmento(segmento);

}

bool existe_segmento(char* nombre_tabla, Segmento* segmento_encontrado){

	bool _condicion_de_segmento(void* segmento_a_analizar){

		char* nombre_a_analizar = ((Segmento)segmento_a_analizar)->nombre_tabla;

		return string_equals_ignore_case(nombre_a_analizar, nombre_tabla);

	}



	*segmento_encontrado = list_find(memoria->tabla_segmentos, _condicion_de_segmento);

	return (*segmento_encontrado) != NULL;

}

Segmento encontrar_segmento_con_pagina(Pagina pagina_a_buscar){

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

bool hay_pagina_libre(Pagina* pagina_solicitada){

	*pagina_solicitada = list_find(memoria->paginas, esta_libre);

	return *pagina_solicitada != NULL;

}

void guardar_dato_en_memoria(Dato nuevo_dato, void* posicion_memoria){

	int desplazamiento = 0;
	int size_value = strlen(nuevo_dato->value);
	int fragmentacion_interna = tamanio_value - size_value;

	memcpy(posicion_memoria + desplazamiento, &(nuevo_dato->timestamp), sizeof(time_t) );
	desplazamiento += sizeof(time_t);

	memcpy(posicion_memoria + desplazamiento, &(nuevo_dato->key), sizeof(nuevo_dato->key));
	desplazamiento += sizeof(nuevo_dato->key);

	memcpy(posicion_memoria + desplazamiento, nuevo_dato->value, size_value);
	desplazamiento += strlen(nuevo_dato->value);

	if(fragmentacion_interna){

		char nulo = '\0';

		while(fragmentacion_interna > 0){

			memcpy(posicion_memoria + desplazamiento, &nulo, 1);

			desplazamiento++;

			fragmentacion_interna--;
		}

	}

}

void actualizar_pagina(Pagina pagina_encontrada, Dato dato_insert){

	guardar_dato_en_memoria(dato_insert, pagina_encontrada->referencia_memoria);

	actualizar_uso(pagina_encontrada);

}

void* auto_journal(void* argumento){

	while(1){
		usleep(tiempo_journal * 1000);

		pthread_mutex_lock(&mutex_journal);
		realizar_journal();
		pthread_mutex_unlock(&mutex_journal);

	}

	return NULL;
}

void realizar_journal(void){

	//aca voy a tener todos los inserts a mandar
	t_list* inserts = list_create();


	void _crear_inserts(void* _segmento){

		Segmento segmento = (Segmento)_segmento;

		//saco las paginas modificadas de un segmento
		t_list* _paginas_modificadas = paginas_modificadas(segmento->Tabla_paginas);

		void _agregar_a_inserts(void* _pagina){

			Pagina pagina = (Pagina)_pagina;

			Dato dato_a_enviar = decodificar_dato_de_memoria(pagina->referencia_memoria);

			insert nuevo_insert = crear_dato_insert(segmento->nombre_tabla, dato_a_enviar->key, dato_a_enviar->value, dato_a_enviar->timestamp);

			request nueva_request = crear_request(INSERT, nuevo_insert);

			liberar_dato(dato_a_enviar);

			list_add(inserts, nueva_request);

		}

		//itero las paginas modificadas de ese segmento, creando una request por cada una de ellas
		list_iterate(_paginas_modificadas, _agregar_a_inserts);

		//destruyo la estructura t_list* de las paginas_modificadas
		list_destroy(_paginas_modificadas);

	}

	//itero todos los segmentos
	list_iterate(memoria->tabla_segmentos, _crear_inserts);


	list_iterate(inserts, enviar_request);

	eliminar_segmentos();

	list_destroy_and_destroy_elements(inserts, liberar_request);


}



Pagina realizar_algoritmo_reemplazo(void){
	printf("Busco la pagina menos usada!\n");
	Pagina pagina_reemplazada = pagina_menos_usada(memoria->paginas);

	if(pagina_reemplazada == NULL){
		printf("Estan todas las paginas modificadas T.T, Realizar JOURNAL\n");

	}else{

		Segmento segmento_modificado;

		segmento_modificado = encontrar_segmento_con_pagina(pagina_reemplazada);

		sacar_pagina_segmento(segmento_modificado, pagina_reemplazada);
	}

	return pagina_reemplazada;
}

//en caso de realizar journal, vuelve a crear el segmento que le pases
Pagina solicitar_pagina(char* nombre_tabla, Segmento* segmento){

	Pagina pagina_solicitada;

	if(hay_pagina_libre(&pagina_solicitada)){
		printf("Hay una pagina libre, guardo el dato en la referencia que tiene!\n");

	}else{
		printf("Estan todas en uso!Realizo algoritmo de reemplazo!\n");
		pagina_solicitada = realizar_algoritmo_reemplazo();

		if(pagina_solicitada == NULL){
			//significa que esta full la memoria
			realizar_journal();
			*segmento = agregar_segmento(nombre_tabla, memoria->tabla_segmentos);
			pagina_solicitada = solicitar_pagina(nombre_tabla, segmento);
		}

	}

	return pagina_solicitada;

}

Dato pedir_dato_al_LFS(char* tabla, int key){

	select_t dato_select = crear_dato_select(tabla, key);

	Dato dato_recibido;

	request nuevo_select = crear_request(SELECT, dato_select);

	enviar_request(nuevo_select); // ya se libera la request aca

	liberar_request(nuevo_select);

	printf("ESTOY ESPERANDO LA RESPUESTA\n");

	t_dato* dato_crudo = recibir_dato_LFS(socket_lissandra);

	dato_recibido = crear_dato(dato_crudo->key, (char *) dato_crudo->value->buffer, dato_crudo->timestamp);

	liberar_t_dato(dato_crudo);

	return dato_recibido;
}
