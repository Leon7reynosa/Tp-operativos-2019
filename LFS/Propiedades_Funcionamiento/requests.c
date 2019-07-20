/*
 * api_memory.c
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#include "requests.h"


void trabajar_request(request request_a_operar , int conexion){

	dato_t* dato_request;
	t_list* metadata_describe;

	switch( request_a_operar->cod_op ){

		case SELECT:

			log_trace(logger_request, "Request de SELECT recibida por el socket %i !\n", conexion);

			printf("CON LOS SIGUIENTES DATOS:\n");
			printf("KEY: %i\n", ((select_t)request_a_operar->tipo_request)->key);
			printf("TABLA: %s\n",	(char *) ((select_t)request_a_operar->tipo_request)->tabla->buffer);


			dato_request = request_select( (select_t) request_a_operar->tipo_request );

			if(dato_request == NULL){

				printf("No se encontro la key solicitada!\n");

			}else{

				printf("Se encontro la key: %i\n", dato_request->key);
				printf("El value: %s\n", dato_request->value);
				printf("El timestamp: %i\n", dato_request->timestamp);

			}

			mandar_select(conexion, dato_request);

			log_trace(logger_request, "Request de SELECT terminada por el socket %i !\n", conexion);

			break;

		case INSERT:

			log_trace(logger_request, "Request de INSERT recibida por el socket %i !\n", conexion);

			request_insert((insert) request_a_operar->tipo_request );

			log_trace(logger_request, "Request de INSERT terminada por el socket %i !\n", conexion);

			break;

		case CREATE:

			log_trace(logger_request, "Request de CREATE recibida por el socket %i !\n", conexion);

			request_create( (create) request_a_operar->tipo_request);

			log_trace(logger_request, "Request de CREATE terminada por el socket %i !\n", conexion);


			break;

		case DESCRIBE:

			log_trace(logger_request, "Request de DESCRIBE recibida por el socket %i !\n", conexion);

			metadata_describe = request_describe( (describe_t) request_a_operar->tipo_request );

			if(metadata_describe != NULL){

				enviar_metadata(metadata_describe, conexion);

				list_destroy_and_destroy_elements(metadata_describe, liberar_metadata);

			}else{
				//NOSE!
			}

			log_trace(logger_request, "Request de DESCRIBE terminada por el socket %i !\n", conexion);

			break;

		case DROP:

			log_trace(logger_request, "Request de DROP recibida por el socket %i !\n", conexion);
			request_drop((Drop)(request_a_operar->tipo_request));
			log_trace(logger_request, "Request de DROP solicitada por el socket %i !\n", conexion);
			break;

		default:

			break;
	}




}

dato_t* request_select(select_t datos_select){

	 dato_t* dato_binarios;
	 dato_t* dato_temporales;
	 dato_t* dato_memtable;

	 char* path_particion_a_buscar;

	 char* nombre_tabla = (char *)datos_select->tabla->buffer;

	 dato_t* dato_mas_nuevo = NULL;

	 log_info(logger_lfs, "Solicitud de request -- SELECT -- \n");

	 // SEMAFORO DE ESTRUCTURA TABLAS
	 pthread_rwlock_rdlock(&(lock_diccionario_compactacion)); 		            // turbios
	 thread_args* argumentos_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

	 //SEMAFORO DE TABLA ESPECIFICA
	 pthread_rwlock_rdlock(&(argumentos_tabla->lock_tabla)); 	 	 	 	   //  turbios

	 if(existe_la_tabla(nombre_tabla)){

		 metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

		 int particion_objetivo = calcular_particion(metadata_tabla->particion , datos_select->key);

		 path_particion_a_buscar = obtenerPath_ParticionTabla(nombre_tabla, particion_objetivo);

		 dato_binarios = buscar_dato_en_particion(path_particion_a_buscar, datos_select->key);

		 dato_temporales = buscar_dato_en_temporales(nombre_tabla, datos_select->key);

		 //ACA HAY UN SEMAFORO DE MEMTABLE
         dato_memtable = obtener_dato_con_mayor_timestamp_tabla(nombre_tabla, datos_select->key);
         /////////////////////////////////

		 dato_t* dato_aux = timestamp_mas_grande(dato_temporales, dato_binarios);

		 dato_mas_nuevo = timestamp_mas_grande(dato_memtable, dato_aux);

		 if(dato_aux != NULL){
			 liberar_dato(dato_aux);
		 }
		 if(dato_binarios != NULL){
			 liberar_dato(dato_binarios);
		 }
		 if(dato_temporales != NULL){
			 liberar_dato(dato_temporales);
		 }
		 if(dato_memtable != NULL){
			 liberar_dato(dato_memtable);
		 }

		 free(metadata_tabla);
		 free(path_particion_a_buscar);

	 }

	 else{
		 //log ?
	 }


	 pthread_rwlock_unlock(&(argumentos_tabla->lock_tabla));
	 ////////////////////////////////////////////////////////
	 pthread_rwlock_unlock(&(lock_diccionario_compactacion));
	 ///////////////////////////////////////////////////////////////////////////////

	 log_info(logger_lfs, "Request -- SELECT -- realizada !!!\n");
	 return  dato_mas_nuevo;

 }



 void request_insert(insert datos_insert){

	 dato_t* dato_ingresar;
	 metadata_t* metadata_insert;

	 char* nombre_tabla = (char *) datos_insert->tabla->buffer;

	 log_info(logger_lfs, "Inicio de request -- INSERT -- \n");

	 //SEMAFORO DE DICCIONARIO COMPACTACION
	 pthread_rwlock_rdlock(&(lock_diccionario_compactacion));
	 thread_args* argumentos_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

	 //SEMAFORO DE TABLA EN ESPECIFICO
	 pthread_rwlock_rdlock(&(argumentos_tabla->lock_tabla));

	 if(existe_la_tabla(nombre_tabla)){

		 metadata_insert = obtener_metadata(nombre_tabla);

		 dato_ingresar = crear_dato(datos_insert->key, (char *)datos_insert->value->buffer, datos_insert->timestamp);

		 //ACA HAY UN SEMAFORO DE MEMTABLE
		 ingresar_a_memtable(dato_ingresar, nombre_tabla);
		 ///////////////////////////////////////////////////////////////////////

	 }

	 else{

//		 log_error(logger_lfs, "Fallo el -- INSERT -- \n");

	 }

	 pthread_rwlock_unlock(&(argumentos_tabla->lock_tabla));

	 pthread_rwlock_unlock(&(lock_diccionario_compactacion));

	 log_info(logger_lfs, "Request -- INSERT -- realizada !!!\n");

 }


 void request_create(create datos_create){

	 log_info(logger_lfs, "Inicio de request -- INSERT -- \n");

	 char* nombre_tabla = (char *)datos_create->tabla->buffer;

	 char* criterio = (char *)datos_create->consistencia->buffer;


	 string_to_upper(nombre_tabla);

	 if(existe_la_tabla(nombre_tabla)){
		 //log ?
		 return;

	 }

	 char* path_tabla = obtenerPathTabla(nombre_tabla);

	 //ESTA FUNCION TE BLOQUEA EN ESCRITURA EL DICCIONARIO Y LA TABLA ESPECIFICA
	 correr_compactacion(datos_create->compactacion, nombre_tabla);

	 crear_directorio(path_tabla);

	 crear_metadata(nombre_tabla, criterio , datos_create->numero_particiones, datos_create->compactacion); //MAL?

	 crear_archivos_particiones(nombre_tabla, datos_create->numero_particiones);

	 correr_compactacion(datos_create->compactacion, nombre_tabla);

	 //YA BLOQUEADO EN crear_directorio
	 thread_args* argumentos_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

	 //DESBLOQUEO LO DE CREAR_DIRECTORIO
	 pthread_rwlock_unlock(&(argumentos_tabla->lock_tabla));
	 pthread_rwlock_unlock(&(lock_diccionario_compactacion));

	 free(path_tabla);

	 log_info(logger_lfs, "Request -- INSERT -- realizada !!!\n");

 }

t_list* request_describe(describe_t request){

	t_list* metadatas;

	if( request->global ){
		//TIENE SEMAFOROS ADENTROO!
		metadatas = request_describe_global();

	}

	else{

		// SEMAFOROOS
		pthread_rwlock_rdlock(&(lock_diccionario_compactacion));

		thread_args* argumento_tabla = dictionary_get(diccionario_compactador, (char *) request->tabla->buffer);

		pthread_rwlock_rdlock(&(argumento_tabla->lock_tabla));


		Metadata metadata_obtenida = request_describe_particular((char*) request->tabla->buffer);

		///////////////////////////////////////////////////////////
		pthread_rwlock_unlock(&(argumento_tabla->lock_tabla));
		//////////////////////////////////////////////////////////////
		pthread_rwlock_unlock(&(lock_diccionario_compactacion));

		if(metadata_obtenida != NULL){

			metadatas = list_create();

			list_add(metadatas, metadata_obtenida);

		}else{
			metadatas = NULL;
		}

	}

	return metadatas;

}


Metadata request_describe_particular(char* nombre_tabla){

	t_config* config_describe;

	Metadata metadata_tabla;

	log_info(logger_lfs, "Inicio de request -- DESCRIBE PARTICULAR --\n");

	if(existe_la_tabla(nombre_tabla)){

		char* path_tabla_metadata = obtener_path_metadata_de_tabla(nombre_tabla);

		int particiones;
		char* consistencia;
		int tiempo_compactacion;

		config_describe = config_create(path_tabla_metadata);

		particiones = config_get_int_value(config_describe , "PARTITIONS");
		consistencia = config_get_string_value(config_describe , "CONSISTENCY");
		tiempo_compactacion = config_get_int_value( config_describe , "COMPACTION_TIME");

		metadata_tabla = crear_estructura_metadata(nombre_tabla, consistencia, particiones, tiempo_compactacion);

		config_destroy(config_describe);

		free(path_tabla_metadata);

	 }

	else{

		metadata_tabla = NULL;
	 }

	log_info(logger_lfs, "Request -- DESCRIBE -- realizada !!!\n");

	return metadata_tabla;
}

t_list* request_describe_global(void){

	log_info(logger_lfs, "Inicio de request -- DESCRIBE GLOBAL --\n");

	char* path_directorio_tabla = obtenerPathDirectorio_Tablas();

	DIR *dir1, *dir2;
	struct dirent *tabla, *tabla_particular;

	t_list* metadatas = list_create();

	if((dir1 = opendir(path_directorio_tabla)) != NULL){

		pthread_rwlock_rdlock(&(diccionario_compactador));

		while((tabla = readdir(dir1)) != NULL){

			if(!string_equals_ignore_case(tabla->d_name, ".") && !string_equals_ignore_case(tabla->d_name, "..")){

				thread_args* argumento_tabla = dictionary_get(diccionario_compactador, tabla->d_name);

				pthread_rwlock_rdlock(&(argumento_tabla->lock_tabla));

				char* path_para_tabla_particular = string_new();

				string_append(&path_para_tabla_particular, path_directorio_tabla);
				string_append(&path_para_tabla_particular, "/");
				string_append(&path_para_tabla_particular, tabla->d_name);

				if((dir2 = opendir(path_para_tabla_particular)) != NULL){

					while((tabla_particular = readdir(dir2)) != NULL){

						if(string_equals_ignore_case(tabla_particular->d_name, "Metadata.config")){

							Metadata metadata_obtenida = request_describe_particular(tabla->d_name);

							list_add(metadatas , metadata_obtenida);

							break;
						}

					}

					closedir(dir2);

				}

				pthread_rwlock_unlock(&(argumento_tabla->lock_tabla));

				free(path_para_tabla_particular);

			}

		}

		pthread_rwlock_unlock(&(diccionario_compactador));

	}

	closedir(dir1);
	free(path_directorio_tabla);

	//no deberia pasar, pero q c yo
	if(list_size(metadatas) <= 0){
		list_destroy(metadatas);
		metadatas = NULL;
	}

	log_info(logger_lfs, "Request -- DESCRIBE GLOBAL -- realizada\n");

	return metadatas;

}




void request_drop(Drop request_drop){

	log_info(logger_lfs, "Inicio de request -- DROP --\n");

	char* nombre_tabla = (char *)request_drop->tabla->buffer;

	DIR *dir1, *dir2;

	struct dirent* tabla, *tabla_particular;

	char* path_directorio_tabla = obtenerPathDirectorio_Tablas();

//	abortar_hilo_compactador(nombre_tabla);//ESTO NO SE SI VA ACA O MAS ABAJO

	if((dir1 = opendir(path_directorio_tabla)) != NULL){

		//SEMAFOROS!!!!!!!!!!!!!!!!!
		pthread_rwlock_wrlock(&(lock_diccionario_compactacion));
		thread_args* argumento_tabla = dictionary_get(diccionario_compactador, nombre_tabla);
		pthread_rwlock_wrlock(&(argumento_tabla->lock_tabla));

		while((tabla = readdir(dir1))){


			if(!string_equals_ignore_case(tabla->d_name, ".") && !string_equals_ignore_case(tabla->d_name, "..")){


				if(string_equals_ignore_case(tabla->d_name, nombre_tabla)){
					char* path_para_tabla_particular = string_new();

					string_append(&path_para_tabla_particular, path_directorio_tabla);
					string_append(&path_para_tabla_particular, "/");
					string_append(&path_para_tabla_particular, tabla->d_name);

					dir2 = opendir(path_para_tabla_particular);
					while((tabla_particular = readdir(dir2)) != NULL){

						if(!string_equals_ignore_case(tabla_particular->d_name, ".") && !string_equals_ignore_case(tabla_particular->d_name, "..")){

							char* path_para_archivo = string_new();

							string_append(&path_para_archivo, path_para_tabla_particular);
							string_append(&path_para_archivo, "/");
							string_append(&path_para_archivo, tabla_particular->d_name);

							if(string_ends_with(tabla_particular->d_name, ".bin") || string_ends_with(tabla_particular->d_name, ".tmp") ||
							   string_ends_with(tabla_particular->d_name, ".tmpc"))
							{
								thread_args* atributos_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

								Particion particion = leer_particion(path_para_archivo);
								printf("Se va a eliminar el siguiente archivo: \n");
								printf("%s\n", path_para_archivo);
								mostrar_particion(particion);
								printf("\n\n");
								liberar_particion(particion);
								eliminar_particion(path_para_archivo);

								//get_all_estados();
							}

							unlink(path_para_archivo);
							free(path_para_archivo);
						}
					}
					closedir(dir2);
					rmdir(path_para_tabla_particular);
					free(path_para_tabla_particular);
					break;
				}

			}
		}

		//TIENE ELIMINACION DEL SEMAFORO ADENTRO
		abortar_hilo_compactador(nombre_tabla);

		//LIBERO EL SEMAFORO!
		pthread_rwlock_unlock(&(lock_diccionario_compactacion));



	}

	closedir(dir1);

	free(path_directorio_tabla);

	log_info(logger_lfs, "Request -- DROP-- realizada\n");
}
