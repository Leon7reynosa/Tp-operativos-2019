/*
 * API_Pool.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Pool.h"

void trabajar_request(request nueva_request , int conexion){

	Dato dato_select;
	t_dato* dato_a_enviar;
	tabla_gossip_dto tabla_recibida;
	t_list* metadata_a_enviar;

	estado_request estado;

	switch(nueva_request->cod_op){

		case SELECT:

			usleep(retardo_memoria * 1000);

			printf(" --SELECT--\n");

			dato_select = request_select( (select_t) nueva_request->tipo_request);

			if(dato_select == NULL){

				estado = ERROR;

				log_info(logger, "No se completo correctamente el select");

				enviar_dato(NULL, conexion, estado);

			}
			 else{

				dato_a_enviar = crear_t_dato(dato_select->key, dato_select->timestamp , dato_select->value);

				estado = SUCCESS;

				enviar_dato(dato_a_enviar, conexion, estado);

				liberar_t_dato(dato_a_enviar);

				liberar_dato(dato_select);

				log_info(logger, "Se envio el dato encontrado");

			}

			break;
		case INSERT:

			usleep(retardo_memoria * 1000);

			printf(" --INSERT--\n");

			request_insert((insert) nueva_request->tipo_request);

			estado = SUCCESS;

			enviar_estado(conexion, estado);

			break;

		case CREATE:

			printf(" --CREATE--\n");

			estado = request_create((create) nueva_request->tipo_request );

			enviar_estado(conexion, estado);

			break;

		case DESCRIBE:

			printf(" --DESCRIBE--\n");

			metadata_a_enviar = request_describe((describe_t) nueva_request->tipo_request);

			if(metadata_a_enviar != NULL){

				estado = SUCCESS;

				enviar_metadata(metadata_a_enviar, conexion, estado);

				list_destroy_and_destroy_elements(metadata_a_enviar, liberar_metadata);

				printf("LIBERE LA METADATA!\n");

			}else{

				estado = ERROR;

				enviar_estado(conexion, estado);

			}

			break;

		case DROP:

			printf(" --DROP--\n");

			estado = request_drop((Drop)nueva_request->tipo_request);

			enviar_estado(conexion, estado);

			break;

		case GOSSIP:

			printf(" --GOSSIP--\n");

			pthread_mutex_lock(&mutex_gossip);

			log_info(logger, "Se realizar intercambio de tablas de gossiping");

			intercambiar_datos( ((tabla_gossip_dto) nueva_request->tipo_request), conexion);

			pthread_mutex_unlock(&mutex_gossip);

			break;

		case JOURNAL:

			printf("--JOURNAL--\n");

			pthread_mutex_lock(&mutex_journal);
			realizar_journal();
			pthread_mutex_unlock(&mutex_journal);

			break;

		default:

			break;
	}

}



Dato request_select(select_t dato){

	log_info(logger, "Se realizara un SELECT");

	Segmento segmento_tabla;
	Pagina pagina_encontrada;
	Dato dato_encontrado;

	pthread_mutex_lock(&mutex_journal);

	if(existe_segmento(dato->tabla->buffer, &segmento_tabla)){

		log_info(logger, "Existe un segmento para %s", (char*) dato->tabla->buffer);

		if(existe_pagina(segmento_tabla, dato->key, &pagina_encontrada)){

			log_info(logger, "Existe una pagina con la key: %i", dato->key);

			mostrar_datos(pagina_encontrada);

			dato_encontrado = decodificar_dato_de_memoria(pagina_encontrada->referencia_memoria);

			actualizar_uso(pagina_encontrada);

		}else{

			log_info(logger, "No existe una pagina con la key %i, la busco en el File System", dato->key);

			Dato dato_lfs = pedir_dato_al_LFS(dato->tabla->buffer, dato->key);

			if(dato_lfs == NULL){

				log_info(logger, "No existe la key en el LFS");
				dato_encontrado = NULL;

			}
			else{
				log_info(logger, "Key encontrada en el File System");

				pagina_encontrada = solicitar_pagina((char*)dato->tabla->buffer, &segmento_tabla);

				agregar_pagina(segmento_tabla, pagina_encontrada);
				actualizar_pagina(pagina_encontrada, dato_lfs);

				liberar_dato(dato_lfs);

				dato_encontrado = decodificar_dato_de_memoria(pagina_encontrada->referencia_memoria); //agrego esto y no trabajo con dato_lfs para hacer siempre																							  //lo mismo

				mostrar_datos(pagina_encontrada);
			}

		}

	}else{
		log_info(logger, "No existe el segmento. Se crea el segmento y se pide el dato al File System.");

		segmento_tabla = agregar_segmento((char*)dato->tabla->buffer, memoria->tabla_segmentos);

		Dato dato_lfs = pedir_dato_al_LFS((char*)dato->tabla->buffer, dato->key);

		//TODO mas de lo mismo que en el elseIf de arriba

		if(dato_lfs == NULL){

			log_info(logger, "No existe la key en el LFS");
			dato_encontrado = NULL;

		}else{

			pagina_encontrada = solicitar_pagina((char*)dato->tabla->buffer, &segmento_tabla);

			agregar_pagina(segmento_tabla, pagina_encontrada);
			actualizar_pagina(pagina_encontrada, dato_lfs);

			liberar_dato(dato_lfs);

			dato_encontrado = decodificar_dato_de_memoria(pagina_encontrada->referencia_memoria); //agrego esto y no trabajo con dato_lfs para hacer siempre
																							 //lo mismo
			mostrar_datos(pagina_encontrada);
		}

	}
	pthread_mutex_unlock(&mutex_journal);

	return dato_encontrado;
}

estado_request request_insert(insert dato){

	log_info(logger, "Se realizara un INSERT");

	Segmento segmento_tabla;
	Pagina pagina_encontrada;
	Dato dato_insert;

	if(string_length((char *)dato->value->buffer) > tamanio_value){
		log_info(logger, "Segmentation Fault! Value demasiado grande");
		return ERROR;
	}
	if(dato->timestamp < 0){
		time_t timestamp = time(NULL);

		if(timestamp < 0){
			//se pudre todooo, el timestamp no esta disponible
			exit(1);
		}

		dato->timestamp = timestamp;
	}

	dato_insert = crear_dato(dato->key, (char *)dato->value->buffer, dato->timestamp );

	pthread_mutex_lock(&mutex_journal);
	if(existe_segmento((char *)dato->tabla->buffer ,&segmento_tabla)){

		log_info(logger, "Existe un segmento para %s", (char*) dato->tabla->buffer);

		if(existe_pagina(segmento_tabla, dato->key, &pagina_encontrada)){

			log_info(logger, "Existe una pagina con la key: %i. La actualizo.", dato->key);

			actualizar_pagina(pagina_encontrada, dato_insert);

			pagina_encontrada->flag_modificado = 1;                      //ver si se puede mover esto

			mostrar_datos(pagina_encontrada);

		}else{

			log_info(logger, "No existe una pagina con la key: %i", dato->key);

			pagina_encontrada = solicitar_pagina((char*)dato->tabla->buffer, &segmento_tabla);

			actualizar_pagina(pagina_encontrada, dato_insert);

			pagina_encontrada->flag_modificado = 1;						//ver si se puede mover esto

			agregar_pagina(segmento_tabla, pagina_encontrada);

			mostrar_datos(pagina_encontrada);

		}


	}else{

		log_info(logger, "No existe el segmento. Se crea el segmento.");

		agregar_segmento((char*)dato->tabla->buffer, memoria->tabla_segmentos);

		if(existe_segmento((char *)dato->tabla->buffer, &segmento_tabla)){ //esto lo uso para encontrarlo nomas (el segmento) xd
//			printf("Ahora si existe el segmento\n");

		}else{
			printf("Deberia existir el segmento, pero me dice que no. NANI?  \n");
			exit(1);
		}

		pagina_encontrada = solicitar_pagina((char*)dato->tabla->buffer, &segmento_tabla);

		actualizar_pagina(pagina_encontrada, dato_insert);

		pagina_encontrada->flag_modificado = 1;	  //ver esto si se puede mover

		agregar_pagina(segmento_tabla, pagina_encontrada);

		mostrar_datos(pagina_encontrada);

	}
	pthread_mutex_unlock(&mutex_journal);

	liberar_dato(dato_insert);

	return SUCCESS;
}


estado_request request_create(create dato_create){

//	log_info(logger, "Se realizara un CREATE");

	request nuevo_create = crear_request(CREATE, dato_create);

	estado_request estado;

	if(enviar_request(nuevo_create, socket_lissandra)){

		estado = recibir_estado_request(socket_lissandra);

		if(estado == SUCCESS){

			free(nuevo_create);

			Segmento segmento_aux;

			pthread_mutex_lock(&mutex_journal);
			if(!existe_segmento((char *)dato_create->tabla->buffer, &segmento_aux)){

				log_info(logger, "No existe un segmento asociado a %s. Se crea el segmento.", (char *)dato_create->tabla->buffer);

				agregar_segmento((char *)dato_create->tabla->buffer , memoria->tabla_segmentos);

			}else{

				log_info(logger , "Existe el segmento asociado a %s.",(char *)dato_create->tabla->buffer );

			}
			pthread_mutex_unlock(&mutex_journal);

		}
	}else{

		estado = ERROR;

	}

	return estado;


}

t_list* request_describe(describe_t dato_describe){

	log_info(logger, "Se realizara un DESCRIBE %s", dato_describe->global ? "GLOBAL" : "DE TABLA: %s", dato_describe->global ? "" : (char *)dato_describe->tabla->buffer );

	request nuevo_describe = crear_request(DESCRIBE, dato_describe);

	enviar_request(nuevo_describe, socket_lissandra);

	t_list* datos_describe;

	printf("Libero la request describe\n");

	free(nuevo_describe);

	printf("ESPERO LA RESPUESTA DEL DESCRIBE!\n");

	datos_describe = recibir_describe(socket_lissandra);

	return datos_describe;

}

estado_request request_drop(Drop datos_drop){

//	log_info(logger, "Se realizara un DROP de tabla: %s", (char *) datos_drop->tabla->buffer);

	request request_drop = crear_request(DROP, datos_drop);

	enviar_request(request_drop, socket_lissandra);

	free(request_drop);

	Segmento segmento_drop;

	estado_request estado;

	estado = recibir_estado_request(socket_lissandra);

	if(estado == SUCCESS){
		pthread_mutex_lock(&mutex_journal);

		if(existe_segmento( (char *) datos_drop->tabla->buffer ,  &segmento_drop )){

//			log_info(logger, "Existe el segmento asociado a %s. Se elimina el segmento.",(char *) datos_drop->tabla->buffer );
			sacar_segmento(segmento_drop);

		}else{
//		log_info(logger, "No existe el segmento asociado a %s",(char *) datos_drop->tabla->buffer );
		}

		pthread_mutex_unlock(&mutex_journal);
		//recibir la poronga de SUCCESS o NOSUCCESS
	}

	return estado;
}
