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

				log_info(logger, "Se encontro el dato con los siguientes parametros:");
				log_info(logger, "Timestamp: %i", dato_select->timestamp);
				log_info(logger, "Key: %i", dato_select->key);
				log_info(logger, "Value: %s", dato_select->value);

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

			log_info(logger, "Se recibio una request de GOSSIPING. Se realizara intercambio de tablas de gossiping");

			log_info(logger_gossip, "Se recibio una request de GOSSIPING");

			intercambiar_datos( ((tabla_gossip_dto) nueva_request->tipo_request), conexion);

			log_info(logger_gossip, "Se termino de realizar la request de GOSSIPING\n");

			log_info(logger, "Se termino de realizar la request de GOSSIPING");


			pthread_mutex_unlock(&mutex_gossip);

			break;

		case JOURNAL:

			printf("--JOURNAL--\n");

			pthread_mutex_lock(&mutex_journal);

			log_info(logger, "Se realizara un JOURNAL");

			realizar_journal();

			log_info(logger, "Se termino de realizar el JOURNAL");

			pthread_mutex_unlock(&mutex_journal);

			break;

		default:

			break;
	}

}



Dato request_select(select_t dato){

	log_info(logger, "Se realizara un SELECT con los siguientes parametros:");

	log_info(logger, "Tabla: %s", (char *)(dato->tabla->buffer));
	log_info(logger, "Key: %i", dato->key);

	Segmento segmento_tabla;
	Pagina pagina_encontrada;
	Dato dato_encontrado;

	pthread_mutex_lock(&mutex_journal);

	if(existe_segmento(dato->tabla->buffer, &segmento_tabla)){

		log_info(logger, "Existe un segmento para la tabla %s", (char*) dato->tabla->buffer);

		if(existe_pagina(segmento_tabla, dato->key, &pagina_encontrada)){

			log_info(logger, "Existe una pagina con la key: %i", dato->key);

//			mostrar_datos(pagina_encontrada);

			dato_encontrado = decodificar_dato_de_memoria(pagina_encontrada->referencia_memoria);

			actualizar_uso(pagina_encontrada);

		}else{

			log_info(logger, "No existe una pagina con la key %i, la busco en el File System", dato->key);

			Dato dato_lfs = pedir_dato_al_LFS(dato->tabla->buffer, dato->key);

			if(dato_lfs == NULL){

				log_info(logger, "No se encontro la key");  //
				dato_encontrado = NULL;

			}
			else{

				log_info(logger, "Key encontrada en el File System");

				pagina_encontrada = solicitar_pagina((char*)dato->tabla->buffer, &segmento_tabla);

				agregar_pagina(segmento_tabla, pagina_encontrada);

				actualizar_pagina(pagina_encontrada, dato_lfs);

				liberar_dato(dato_lfs);

				dato_encontrado = decodificar_dato_de_memoria(pagina_encontrada->referencia_memoria); //agrego esto y no trabajo con dato_lfs para hacer siempre																							  //lo mismo

//				mostrar_datos(pagina_encontrada);
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

//			mostrar_datos(pagina_encontrada);
		}

	}
	pthread_mutex_unlock(&mutex_journal);

	log_info(logger, "Se termino de realizar el SELECT");

	return dato_encontrado;
}

estado_request request_insert(insert dato){

	log_info(logger, "Se realizara un INSERT con los siguientes parametros");

	log_info(logger, "Tabla: %s",(char *)(dato->tabla->buffer));
	log_info(logger, "Key: %i", dato->key);
	log_info(logger, "Value: %s", (char *)(dato->value->buffer));

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

	log_info(logger, "Timestamp: %i", dato->timestamp);

	dato_insert = crear_dato(dato->key, (char *)dato->value->buffer, dato->timestamp );

	pthread_mutex_lock(&mutex_journal);
	if(existe_segmento((char *)dato->tabla->buffer ,&segmento_tabla)){

		log_info(logger, "Existe un segmento para %s", (char*) dato->tabla->buffer);

		if(existe_pagina(segmento_tabla, dato->key, &pagina_encontrada)){

			log_info(logger, "Existe una pagina con la key: %i. La actualizo.", dato->key);

			actualizar_pagina(pagina_encontrada, dato_insert);

			pagina_encontrada->flag_modificado = 1;                      //ver si se puede mover esto

//			mostrar_datos(pagina_encontrada);

		}else{

			log_info(logger, "No existe una pagina con la key: %i", dato->key);

			pagina_encontrada = solicitar_pagina((char*)dato->tabla->buffer, &segmento_tabla);

			actualizar_pagina(pagina_encontrada, dato_insert);

			pagina_encontrada->flag_modificado = 1;						//ver si se puede mover esto

			agregar_pagina(segmento_tabla, pagina_encontrada);

//			mostrar_datos(pagina_encontrada);

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

//		mostrar_datos(pagina_encontrada);

	}
	pthread_mutex_unlock(&mutex_journal);

	liberar_dato(dato_insert);

	log_info(logger, "Se termino de realizar el INSERT");

	return SUCCESS;
}


estado_request request_create(create dato_create){

	log_info(logger, "Se realizara un CREATE con los siguientes parametros: ");

	log_info(logger, "Tabla: %s", (char *)(dato_create->tabla->buffer));
	log_info(logger, "Consistencia: %s",  (char *)(dato_create->consistencia->buffer));
	log_info(logger, "Cantidad de particiones: %i", dato_create->numero_particiones);
	log_info(logger, "Tiempo de compactacion: %i", dato_create->compactacion);


	request nuevo_create = crear_request(CREATE, dato_create);

	estado_request estado;

	pthread_mutex_lock(&mutex_journal);

	if(comprobar_conexion_lissandra(ip_lfs, puerto_lfs)){


		if(enviar_request(nuevo_create, socket_lissandra)){

				usleep(retardo_lfs* 1000);

				estado = recibir_estado_request(socket_lissandra);

				if(estado == SUCCESS){

					Segmento segmento_aux;

					if(!existe_segmento((char *)dato_create->tabla->buffer, &segmento_aux)){

						log_info(logger, "No existe un segmento asociado a %s. Se crea el segmento.", (char *)dato_create->tabla->buffer);

						agregar_segmento((char *)dato_create->tabla->buffer , memoria->tabla_segmentos);

					}else{

						log_info(logger , "Existe el segmento asociado a %s.",(char *)dato_create->tabla->buffer );

					}

				}else if (estado == ERROR_CONEXION){

					log_info(logger, "No se recibio respuesta del FileSystem, esta desconectado");

					estado = ERROR;

					desconectar_lissandra();

				}else{

					log_info(logger, "No se pudo realizar el create en el FileSystem");

				}


			}else{

				log_info(logger, "No se pudo mandar la request al FileSystem, esta desconectado");

				desconectar_lissandra();

				estado = ERROR;

			}

	}else{

		printf("Lissandra no esta conectada\n");

		log_info(logger, "Lissandra no esta conectada");

		estado = ERROR;

	}

	pthread_mutex_unlock(&mutex_journal);

	free(nuevo_create);

	return estado;


}

t_list* request_describe(describe_t dato_describe){

	if(dato_describe->global){

		log_info(logger, "Se realizara un DESCRIBE GLOBAL");

	}else{

		log_info(logger, "Se realizara un DESCRIBE de la TABLA %s", (char *)dato_describe->tabla->buffer);

	}



	request nuevo_describe = crear_request(DESCRIBE, dato_describe);

	t_list* datos_describe;

	pthread_mutex_lock(&mutex_journal);

	if(comprobar_conexion_lissandra(ip_lfs, puerto_lfs)){

		if(!enviar_request(nuevo_describe, socket_lissandra) ){

			log_info(logger, "No se pudo mandar la request al FileSystem, esta desconectado");

			desconectar_lissandra();

			free(nuevo_describe);

			pthread_mutex_unlock(&mutex_journal);

			return NULL;

		}else{

			usleep(retardo_lfs* 1000);

			datos_describe = recibir_describe(socket_lissandra);

		}


	}else{

		printf("Lissandra no esta conectada\n");

		log_info(logger, "Lissandra no esta conectada");

		datos_describe = NULL;

	}

	pthread_mutex_unlock(&mutex_journal);

	free(nuevo_describe);

	if(dato_describe->global){

		log_info(logger, "Se termino de realizar el DESCRIBE GLOBAL");


	}else{

		log_info(logger, "Se termino de realizar el DESCRIBE de la TABLA %s", (char *)dato_describe->tabla->buffer);

	}

	return datos_describe;

}

estado_request request_drop(Drop datos_drop){

	log_info(logger, "Se realizara un DROP de tabla: %s", (char *) datos_drop->tabla->buffer);

	request request_drop = crear_request(DROP, datos_drop);

	Segmento segmento_drop;

	estado_request estado;

	pthread_mutex_lock(&mutex_journal);

	if(comprobar_conexion_lissandra(ip_lfs, puerto_lfs)){

		if(! enviar_request(request_drop, socket_lissandra) ){

			log_info(logger, "No se pudo mandar la request al FileSystem, esta desconectado");

			desconectar_lissandra();

			free(request_drop);

			pthread_mutex_unlock(&mutex_journal);

			return ERROR;

		}else{

			usleep(retardo_lfs* 1000);

			estado = recibir_estado_request(socket_lissandra);

			if(estado == SUCCESS){

				if(existe_segmento( (char *) datos_drop->tabla->buffer ,  &segmento_drop )){

					log_info(logger, "Existe el segmento asociado a %s. Se elimina el segmento.",(char *) datos_drop->tabla->buffer );
					sacar_segmento(segmento_drop);

				}else{
					log_info(logger, "No existe el segmento asociado a %s",(char *) datos_drop->tabla->buffer );
				}


			}else if(estado == ERROR_CONEXION){

				log_info(logger, "No se recibio respuesta del FileSystem, esta desconectado");

				estado = ERROR;

				desconectar_lissandra();

			}else{

				log_info(logger, "No se pudo realizar el Drop en el FileSystem");

			}

		}


	}else{

		printf("Lissandra no esta conectada\n");

		log_info(logger, "Lissandra esta desconectada");

	}

	pthread_mutex_unlock(&mutex_journal);

	free(request_drop);


	log_info(logger, "Se termino de realizar el DROP");

	return estado;
}
