/*
 * memoria.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"memoria.h"

void liberar_memoria(void){

	eliminar_segmentos();
	list_destroy(memoria->tabla_segmentos);


	list_destroy_and_destroy_elements(memoria->seed, liberar_seed);
	list_destroy_and_destroy_elements(memoria->tabla_gossiping, liberar_seed);

	list_destroy_and_destroy_elements(memoria->paginas, eliminar_pagina);

	free(memoria->memoria_contigua);

	free(memoria);
}

t_list* inicializar_paginas(){

	int contador = memoria->cant_max_datos;


	t_list* paginas = list_create();

	int desplazamiento = 0;

	while(contador > 0){

		Pagina nueva_pagina = crear_pagina((memoria->memoria_contigua + desplazamiento));
		desplazamiento += tamanio_dato;


		list_add(paginas, nueva_pagina);

		contador--;

	}

	return paginas;

}

void inicializar_memoria(int tamanio, int tamanio_value , int tamanio_dato){

	memoria = malloc(sizeof(struct MemoriaEstructura));
	memoria->memoria_contigua  = malloc(tamanio);

	memoria->tabla_segmentos   = list_create();
	memoria->tamanio 		   = tamanio;
	memoria->cant_max_datos    = tamanio / tamanio_dato;

	memoria->paginas		   = inicializar_paginas();
	memoria->numero_memoria = numero_memoria;

	memoria->seed = list_create();
	inicializar_seeds();

	memoria->tabla_gossiping = list_create();
	inicializar_tabla_gossip();

	log_info(logger, "Se inicializo correctamente la memoria.\n>> Tamanio de la memoria = %i -> Cantidad maxima de datos: %i \n", tamanio, memoria->cant_max_datos);

}

void inicializar_tabla_gossip(void){

	Seed this_memoria = crear_seed(memoria->numero_memoria, ip_escucha, puerto_escucha);

	list_add(memoria->tabla_gossiping, this_memoria);

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

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	while(1){

		pthread_rwlock_rdlock(&semaforo_tiempo_journal);

		usleep(tiempo_journal * 1000);

		pthread_rwlock_unlock(&semaforo_tiempo_journal);

		printf("\n///////////////////////// Auto-Journal/////////////////////////////\n\n");

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		pthread_mutex_lock(&mutex_journal);

		log_info(logger_journal, "Inicia el Auto-Journal");

		realizar_journal();

		log_info(logger_journal, "Termina el Auto-Journal\n");

		pthread_mutex_unlock(&mutex_journal);

		printf("\n////////////////////////FIN Auto-Journal///////////////////////////\n\n");

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

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

		log_info(logger_journal, "Cantidad de paginas a mandar: %i", list_size(_paginas_modificadas));

		void _agregar_a_inserts(void* _pagina){

			Pagina pagina = (Pagina)_pagina;

			Dato dato_a_enviar = decodificar_dato_de_memoria(pagina->referencia_memoria);

			insert nuevo_insert = crear_dato_insert(segmento->nombre_tabla, dato_a_enviar->key, dato_a_enviar->value, dato_a_enviar->timestamp);

			log_info(logger_journal, "Se manda al LFS la tabla %s", (char *)nuevo_insert->tabla->buffer);
			log_info(logger_journal, "Con el timestamp %i", (char *)nuevo_insert->timestamp);
			log_info(logger_journal, "Con la key %i", (char *)nuevo_insert->key);
			log_info(logger_journal, "Con el value %s", (char *)nuevo_insert->value->buffer);

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

	void _enviar_request(void* _request){

		request request_a_enviar = (request)_request;

		estado_request estado;

		if(comprobar_conexion_lissandra(ip_lfs, puerto_lfs)){

			printf("Lissandra esta conectada\n");

			if(!enviar_request(request_a_enviar, socket_lissandra)){

				log_info(logger_journal, "No se pudo mandar la request al FileSystem, esta desconectado");

				desconectar_lissandra();

				return;

			}else{

				usleep(retardo_lfs* 1000);

				estado = recibir_estado_request(socket_lissandra);

				if(estado == ERROR_CONEXION){

					log_info(logger_journal, "No se recibio respuesta del FileSystem, esta desconectado");

					desconectar_lissandra();

					printf("HUBO UN ERROR CON EL JOURNAL\n");

				}else if(estado == ERROR){

					log_info(logger_journal, "No se pudo insertar en el FileSystem");

				}else{

					log_info(logger_journal, "Se mando correctamente");

				}


			}


		}else{

			printf("Lissandra esta desconectada\n");

		}


	}


	list_iterate(inserts, _enviar_request);

	eliminar_segmentos();

	list_destroy_and_destroy_elements(inserts, liberar_request);


}



Pagina realizar_algoritmo_reemplazo(void){

//	log_info(logger, "Empieza el algoritmo de reemplazo");

	Pagina pagina_reemplazada = pagina_menos_usada(memoria->paginas);

	if(pagina_reemplazada == NULL){

		log_info(logger, "La memoria esta FULL");

	}else{

		Segmento segmento_modificado;

		segmento_modificado = encontrar_segmento_con_pagina(pagina_reemplazada);

		sacar_pagina_segmento(segmento_modificado, pagina_reemplazada);

		Dato dato_para_logg = decodificar_dato_de_memoria(pagina_reemplazada->referencia_memoria);

		log_info(logger, "La pagina a reemplazar pertenecia a %s y tenia la key %i", segmento_modificado->nombre_tabla, dato_para_logg->key);

		liberar_dato(dato_para_logg);
	}

	return pagina_reemplazada;
}

//en caso de realizar journal, vuelve a crear el segmento que le pases
Pagina solicitar_pagina(char* nombre_tabla, Segmento* segmento){

	log_info(logger, "Se solicita una nueva pagina");

	Pagina pagina_solicitada;

	if(hay_pagina_libre(&pagina_solicitada)){
		log_info(logger, "Hay una pagina libre para asignar");

	}else{
		log_info(logger, "Estan todas las paginas ocupadas");
		pagina_solicitada = realizar_algoritmo_reemplazo();

		if(pagina_solicitada == NULL){

			log_info(logger, "Realizo Journal para tener espacio disponible");

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

	if(comprobar_conexion_lissandra(ip_lfs, puerto_lfs)){

		printf("Lissandra esta conectado\n");

		if(!enviar_request(nuevo_select, socket_lissandra)){

			desconectar_lissandra();

			log_info(logger, "No se pudo mandar la request al FileSystem, esta desconectado");

			dato_recibido = NULL;

		}else{

			printf("ESTOY ESPERANDO LA RESPUESTA\n");

			usleep(retardo_lfs* 1000);

			t_dato* dato_crudo = recibir_dato_LFS(socket_lissandra);

			if(dato_crudo == NULL){

				dato_recibido = NULL;

			}else{

				printf("Llego la respuesta del File System\n");

				dato_recibido = crear_dato(dato_crudo->key, (char *) dato_crudo->value->buffer, dato_crudo->timestamp);

				liberar_t_dato(dato_crudo);
			}

		}


	}else{

		printf("Lissandra esta desconectada\n");
		//LISSANDRA NO ESTA CONECTADA

	}

	liberar_request(nuevo_select);

	return dato_recibido;
}
