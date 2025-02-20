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
	estado_request estado;

	switch( request_a_operar->cod_op ){

		case SELECT:

			printf("[REQUEST] Se realizara un SELECT");

			log_info(logger_request, "Request de SELECT recibida por el socket %i !", conexion);

			dato_request = request_select( (select_t) request_a_operar->tipo_request );

			if(dato_request == NULL){
				printf("[REQUEST] No se encontro la key solicitada!\n");

				log_info(logger_request, "No se encontro el dato, mando error");

				mandar_select(conexion, dato_request, ERROR);

			}else{

				log_info(logger_request, "[REQUEST] Se encontro la key: %i", dato_request->key);
				log_info(logger_request,"[REQUEST] El value: %s\n", dato_request->value);
				log_info(logger_request, "[REQUEST] El timestamp: %i\n", dato_request->timestamp);

				mandar_select(conexion, dato_request, SUCCESS);

				liberar_dato(dato_request);

			}

			log_info(logger_request, "Request de SELECT terminada por el socket %i !\n", conexion);

			printf("[REQUEST] Finalizo el SELECT\n\n");

			break;

		case INSERT:

			printf("[REQUEST] Se realizara un INSERT\n");

			log_info(logger_request, "Request de INSERT recibida por el socket %i !", conexion);

			estado = request_insert((insert) request_a_operar->tipo_request );

			if(estado == ERROR){

				log_info(logger_request , "Fallo la Request INSERT\n");


			}else{

				log_info(logger_request, "Request de INSERT terminada por el socket %i !\n", conexion);

			}

			mandar_estado(conexion, estado);

			printf("[REQUEST] Finalizo el INSERT\n\n");

			break;

		case CREATE:

			printf("[REQUEST] Se realizara un CREATE\n");

			log_info(logger_request, "Request de CREATE recibida por el socket %i !", conexion);

			estado = request_create( (create) request_a_operar->tipo_request);

			if(estado == ERROR){

				log_info(logger_request , "Fallo la Request CREATE\n");

			}else{

				log_info(logger_request, "Request de CREATE terminada correctamente por el socket %i !\n", conexion);

			}

			mandar_estado(conexion, estado);

			printf("[REQUEST] Finalizo el CREATE\n\n");

			break;

		case DESCRIBE:

			printf("[REQUEST] Se realizara un DESCRIBE\n");

			log_info(logger_request, "Request de DESCRIBE recibida por el socket %i !", conexion);

			metadata_describe = request_describe( (describe_t) request_a_operar->tipo_request );

			if(metadata_describe != NULL){

				estado = SUCCESS;

				enviar_metadata(metadata_describe, conexion, estado);

				list_destroy_and_destroy_elements(metadata_describe, liberar_metadata);

			}else{

				estado = ERROR;

				mandar_estado(conexion, estado);

			}


			log_info(logger_request, "Request de DESCRIBE terminada por el socket %i !\n", conexion);

			printf("[REQUEST] Finalizo el DESCRIBE\n\n");

			break;

		case DROP:

			printf("[REQUEST] Se realizara un DROP");

			log_info(logger_request, "Request de DROP recibida por el socket %i !", conexion);

			estado = request_drop((Drop)(request_a_operar->tipo_request));

			if(estado == ERROR){

				log_info(logger_request , "-Hubo un error con la request DROP.-\n");


			}else{

				log_info(logger_request, "Request de DROP solicitada por el socket %i !\n", conexion);

			}

			mandar_estado(conexion, estado);

			printf("[REQUEST] Finalizo el DROP\n\n");

			break;

		default:

			break;
	}




}

dato_t* request_select(select_t datos_select){

	 dato_t* dato_binarios;
	 dato_t* dato_temporales;
	 dato_t* dato_memtable;

	 dato_t* dato_mas_nuevo = NULL;

	 char* path_particion_a_buscar;

	 char* nombre_tabla = (char *)datos_select->tabla->buffer;

	 log_info(logger_lfs, "Solicitud de request -- SELECT -- \n");

	 pthread_rwlock_rdlock(&(lock_diccionario_compactacion));

	 if(existe_la_tabla(nombre_tabla)){

		thread_args* argumentos_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

		pthread_rwlock_rdlock(&(argumentos_tabla->lock_tabla));

//		printf("[SELECT %s : key %i] Existe la tabla\n", nombre_tabla, datos_select->key);

		metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

//		printf("[SELECT] ya obtuve la metadata\n");

		int particion_objetivo = calcular_particion(metadata_tabla->particion , datos_select->key);

//		printf("[SELECT %s : key %i] La particion de la key deberia ser %i\n", nombre_tabla, datos_select->key, particion_objetivo);

		path_particion_a_buscar = obtenerPath_ParticionTabla(nombre_tabla, particion_objetivo);

//		printf("[SELECT] obtuvimos el path: %s\n" , path_particion_a_buscar);

		dato_binarios = buscar_dato_en_particion(path_particion_a_buscar, datos_select->key);

//		printf("[SELECT] ya buscamos el dato enla particion\n");

		dato_temporales = buscar_dato_en_temporales(nombre_tabla, datos_select->key);

		pthread_rwlock_unlock(&(argumentos_tabla->lock_tabla));

//		printf("[SELECT] ya buscamos el dato en temporales\n");

		 //ACA HAY UN SEMAFORO DE MEMTABLE
        dato_memtable = obtener_dato_con_mayor_timestamp_tabla(nombre_tabla, datos_select->key);

//      printf("[SELECT] fallo en obtener_dato_mayor timestamp\n");

		dato_t* dato_aux = timestamp_mas_grande(dato_temporales, dato_binarios);

//		printf("[SELECT] fallo en obtener timestamp mas grande primero\n");

		dato_mas_nuevo = timestamp_mas_grande(dato_memtable, dato_aux);

//		printf("[SELECT] ya tenemos el mas grande\n");

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

//		 printf("[SELECT %s : key %i] No existe la tabla\n", nombre_tabla, datos_select->key);

		 log_info(logger_request, "NO SE ENCUENTRA LA TABLA");

	 }


	 pthread_rwlock_unlock(&(lock_diccionario_compactacion));

//	 printf("\nTERMINO LA REQUEST SELECT\n");
	 log_info(logger_lfs, "Request -- SELECT -- realizada !!!\n");
	 return  dato_mas_nuevo;

 }



 estado_request request_insert(insert datos_insert){

	 dato_t* dato_ingresar;
//	 metadata_t* metadata_insert;
	 estado_request estado;

	 char* nombre_tabla = (char *) datos_insert->tabla->buffer;

	 log_info(logger_lfs, "Inicio de request -- INSERT -- \n");

	 pthread_rwlock_rdlock(&(lock_diccionario_compactacion)); 	//SEMAFORO DE DICCIONARIO COMPACTACION

	 if(existe_la_tabla(nombre_tabla)){

		 thread_args* argumentos_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

		 pthread_rwlock_wrlock(&lock_memtable);

		 pthread_rwlock_rdlock(&(argumentos_tabla->lock_tabla)); 	//SEMAFORO DE TABLA EN ESPECIFICO

		 dato_ingresar = crear_dato(datos_insert->key, (char *)datos_insert->value->buffer, datos_insert->timestamp);

		 //ACA HAY UN SEMAFORO DE MEMTABLE

		 ingresar_a_memtable(dato_ingresar, nombre_tabla);

		 printf("[INSERT] Se inserto correctamente en la memtable\n");

		 estado = SUCCESS;

		 pthread_rwlock_unlock(&(argumentos_tabla->lock_tabla));

		 pthread_rwlock_unlock(&lock_memtable);

		 ///////////////////////////////////////////////////////////////////////

	 }else{

		 printf("[INSERT] No existe la tabla\n");

//		 log_error(logger_lfs, "Fallo el -- INSERT -- \n");

		 estado = ERROR;

	 }

	 pthread_rwlock_unlock(&(lock_diccionario_compactacion));

	 log_info(logger_lfs, "Request -- INSERT -- realizada !!!\n");

	 return estado;

 }


 estado_request request_create(create datos_create){

	 log_info(logger_lfs, "Inicio de request -- INSERT -- \n");

	 char* nombre_tabla = (char *)datos_create->tabla->buffer;

	 char* criterio = (char *)datos_create->consistencia->buffer;


	 string_to_upper(nombre_tabla);

	 if(existe_la_tabla(nombre_tabla)){
		 //log
		 log_error(logger_request , "Ya existe la tabla\n");
		 return ERROR;

	 }

//	 printf("[CREATE] No existe la tabla\n");

	 char* path_tabla = obtenerPathTabla(nombre_tabla);

	 //ESTA FUNCION TE BLOQUEA EN ESCRITURA EL DICCIONARIO Y LA TABLA ESPECIFICA

//	 printf("[CREATE] Corro la compactacion de la tabla %s\n", nombre_tabla);

	 correr_compactacion(datos_create->compactacion, nombre_tabla);

//	 printf("[CREATE] Creo el directorio asociado a la tabla\n");

	 crear_directorio(path_tabla);

//	 printf("[CREATE] Creo la metadata asociada a la tabla\n");

	 crear_metadata(nombre_tabla, criterio , datos_create->numero_particiones, datos_create->compactacion); //MAL?

//	 printf("[CREATE] Creo las particiones asociadas a la tabla\n");

	 crear_archivos_particiones(nombre_tabla, datos_create->numero_particiones);

//	 correr_compactacion(datos_create->compactacion, nombre_tabla);

	 //YA BLOQUEADO EN crear_directorio
	 thread_args* argumentos_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

	 //DESBLOQUEO LO DE CREAR_DIRECTORIO
	 pthread_rwlock_unlock(&(argumentos_tabla->lock_tabla));
	 pthread_rwlock_unlock(&(lock_diccionario_compactacion));

	 free(path_tabla);

	 log_info(logger_lfs, "Request -- INSERT -- realizada !!!\n");

	 printf("[CREATE] Termino correctamente\n");

	 return SUCCESS;

 }

t_list* request_describe(describe_t request){

	t_list* metadatas;

	if( request->global ){
		//TIENE SEMAFOROS ADENTROO!;
		metadatas = request_describe_global();

	}

	else{

		pthread_rwlock_rdlock(&(lock_diccionario_compactacion));

		if(existe_la_tabla((char *)(request->tabla->buffer))){

			thread_args* argumento_tabla = dictionary_get(diccionario_compactador, (char *) request->tabla->buffer);

			pthread_rwlock_rdlock(&(argumento_tabla->lock_tabla));

			Metadata metadata_obtenida = request_describe_particular((char*) request->tabla->buffer);

					///////////////////////////////////////////////////////////
			pthread_rwlock_unlock(&(argumento_tabla->lock_tabla));
					//////////////////////////////////////////////////////////////

			metadatas = list_create();

			list_add(metadatas, metadata_obtenida);

		}else{

			metadatas = NULL;

		}

		pthread_rwlock_unlock(&(lock_diccionario_compactacion));

	}

	return metadatas;

}


Metadata request_describe_particular(char* nombre_tabla){

	t_config* config_describe;

	Metadata metadata_tabla;

	log_info(logger_lfs, "Inicio de request -- DESCRIBE PARTICULAR --\n");

//	if(existe_la_tabla(nombre_tabla)){

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

//	 }
//
//	else{
//
//		metadata_tabla = NULL;
//	 }

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

		pthread_rwlock_rdlock(&(lock_diccionario_compactacion));

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

		pthread_rwlock_unlock(&(lock_diccionario_compactacion));

	}

	closedir(dir1);
	free(path_directorio_tabla);

	//no deberia pasar, pero q c yo
//	if(list_size(metadatas) <= 0){
//		list_destroy(metadatas);
//		metadatas = NULL;
//	}

	log_info(logger_lfs, "Request -- DESCRIBE GLOBAL -- realizada\n");

	return metadatas;

}




estado_request request_drop(Drop request_drop){

	log_info(logger_lfs, "Inicio de request -- DROP --\n");

	char* nombre_tabla = (char *)(request_drop->tabla->buffer);

	DIR *dir1, *dir2;

	estado_request estado = ERROR;

	struct dirent* tabla, *tabla_particular;

	char* path_directorio_tabla = obtenerPathDirectorio_Tablas();

//	if(existe_tabla((char*)request_drop->tabla->buffer)){
//	}

	if((dir1 = opendir(path_directorio_tabla)) != NULL){

		//SEMAFOROS!!!!!!!!!!!!!!!!!
		pthread_rwlock_wrlock(&(lock_diccionario_compactacion));
//		thread_args* argumento_tabla = dictionary_get(diccionario_compactador, nombre_tabla);
//		pthread_rwlock_wrlock(&(argumento_tabla->lock_tabla));

		while((tabla = readdir(dir1))){


			if(!string_equals_ignore_case(tabla->d_name, ".") && !string_equals_ignore_case(tabla->d_name, "..")){


				if(string_equals_ignore_case(tabla->d_name, nombre_tabla)){

					thread_args* argumento_tabla = dictionary_get(diccionario_compactador, nombre_tabla);
					pthread_rwlock_wrlock(&(argumento_tabla->lock_tabla));

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

								Particion particion = leer_particion(path_para_archivo);

//								mostrar_particion(particion);
//								printf("\n");
								liberar_particion(particion);

								eliminar_particion(path_para_archivo);

//								printf("====Se elimino dicha particion====\n");

								//get_all_estados();
							}

							unlink(path_para_archivo);
							free(path_para_archivo);
						}
					}

					closedir(dir2);
					rmdir(path_para_tabla_particular);

					free(path_para_tabla_particular);

					estado = SUCCESS;

					abortar_hilo_compactador(nombre_tabla);

					break;
				}

			}
		}

		//TIENE ELIMINACION DEL SEMAFORO ADENTRO
//		abortar_hilo_compactador(nombre_tabla);

		//LIBERO EL SEMAFORO!
		pthread_rwlock_unlock(&(lock_diccionario_compactacion));

//		estado = SUCCESS;

	}

	closedir(dir1);

	free(path_directorio_tabla);

	printf("=== Se elimino la tabla %s ===\n", (char*)request_drop->tabla->buffer);
	log_info(logger_lfs, "Request -- DROP-- realizada\n");

	return estado;

}
