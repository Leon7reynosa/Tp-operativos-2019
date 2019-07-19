/*
 * api_memory.c
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#include "requests.h"


//capaz tiene que retoranr algo
void trabajar_request(request request_a_operar , int conexion){

	dato_t* dato_request;
	t_list* metadata_describe;

	switch( request_a_operar->cod_op ){

		case SELECT:

			printf(">>SE REALIZARA EL SELECT\n");

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

			printf("///////////////////////////TERMINO EL SELECT//////////////////////////////////////");
			//falta hacer mas cosas aca, habria que reenviarlo al pool

			break;

		case INSERT:

			printf("Se realizara Insert\n");

			request_insert((insert) request_a_operar->tipo_request );

			printf("///////////////////Termino el insert//////////////////\n");

			break;

		case CREATE:

			printf("Se realizara Create\n");

			request_create( (create) request_a_operar->tipo_request);

			printf("Termine de realizar el Create\n");

			//TODO habria que hacer que le diga al pool que esta todo ok.

			break;

		case DESCRIBE:

			printf("Se realizara Describe\n");

			metadata_describe = request_describe( (describe_t) request_a_operar->tipo_request );

			printf("Obtuve la lista de metadatas\n");

			if(metadata_describe != NULL){

				printf("No es nula, entonces se la envio a la memoria\n");
				enviar_metadata(metadata_describe, conexion);

				printf("Se la envie correctamente\n");
				list_destroy_and_destroy_elements(metadata_describe, liberar_metadata);

			}else{
				//NOSE!
			}

			printf("TERMINO EL DESCRIBE BRO\n");

			break;

		case DROP:

			printf("Se realizara el DROP\n");
			request_drop((Drop)(request_a_operar->tipo_request));
			printf("Se termino el drop\n");
			break;

		default:

			//no se aca lol

			break;
	}




}

dato_t* request_select(select_t datos_select){ //hay que modificarla para que reciba un select_t

	 dato_t* dato_binarios;
	 dato_t* dato_temporales;
	 dato_t* dato_memtable;

	 char* path_particion_a_buscar;

	 char* nombre_tabla = (char *)datos_select->tabla->buffer;

	 dato_t* dato_mas_nuevo = NULL;

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Existe la tabla en el File System\n");

		 printf("TABLA: %s\n", nombre_tabla);
		 metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

		 printf("Muestro algo de la metadata: %i\n", metadata_tabla->particion);

		 int particion_objetivo = calcular_particion(metadata_tabla->particion , datos_select->key);

		 printf("Calculo la particion en la que se encuentra el dato: %i\n", particion_objetivo);

		 path_particion_a_buscar = obtenerPath_ParticionTabla(nombre_tabla, particion_objetivo);

		 printf("El path para esa particion: %s\n", path_particion_a_buscar);

		 printf("Busco el dato en el binario\n");
		 dato_binarios = buscar_dato_en_particion(path_particion_a_buscar, datos_select->key);
		 printf("Termine de buscar en los binarios de la particion\n");

		 printf("Los busco en los temporales\n");
		 dato_temporales = buscar_dato_en_temporales(nombre_tabla, datos_select->key);
		 printf("Termine de buscar en los temporales\n");

		 printf("Busco en la memtable\n");
         dato_memtable = obtener_dato_con_mayor_timestamp_tabla(nombre_tabla, datos_select->key);
         printf("Termine de buscar en la memtable\n");

         printf("Busco el dato que tiene el timestamp mas grande\n");
		 dato_t* dato_aux = timestamp_mas_grande(dato_temporales, dato_binarios);

		 dato_mas_nuevo = timestamp_mas_grande(dato_memtable, dato_aux);

		 printf("Termine de buscar el que tiene el mas grande\n");

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

		 printf("Libere todo lo administrativo\n");

	 }

	 else{
		 printf("No existe la tabla en el File System\n");
	 }

	 return  dato_mas_nuevo;

 }



 void request_insert(insert datos_insert){ //hay que modificar para que opere con insert
	 //faltaria ver cuando no le pasamos el timestamp
	 dato_t* dato_ingresar;
	 metadata_t* metadata_insert;

	 char* nombre_tabla = (char *) datos_insert->tabla->buffer;

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Existe la tabla en el File System\n");

		 metadata_insert = obtener_metadata(nombre_tabla);

		 printf("Voy a ingresar el siguiente value: %s\n",(char *)datos_insert->value->buffer );

		 dato_ingresar = crear_dato(datos_insert->key, (char *)datos_insert->value->buffer, datos_insert->timestamp);

		 ingresar_a_memtable(dato_ingresar, nombre_tabla);

	 }else{

		 printf("Fallo el insert. \n");
		 //aca deberiamos hacer un log de que fallo.

	 }

 }


 void request_create(create datos_create){
//TODO hay que modificar para que trabaje con la estructura create


	 printf("Entro a la request create!\n");
	 char* nombre_tabla = (char *)datos_create->tabla->buffer;
	 printf("Nombre tabla: %s\n", nombre_tabla);
	 char* criterio = (char *)datos_create->consistencia->buffer;

//	 //log_info(logger_lissandra, "### SOLICITUD DE -- CREATE -- para\n"
//	 			 "TABLA = %s\nNUMERO DE PARTICIONES = %i\nCRITERIO = %s\nTIEMPO_COMPACTACION = %i\n",
//	 			 nombre_tabla, datos_create->numero_particiones, criterio, datos_create->compactacion);		//LOGGER AGREGADO !!!!!!!!!!!!

	 string_to_upper(nombre_tabla);

	 if(existe_la_tabla(nombre_tabla)){
		// log_error(logger_lissandra, "### YA EXISTE LA TABLA ###\n", nombre_tabla);  //LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		 return;

	 }

	 printf("No existe esa tabla!n");

	 char* path_tabla = obtenerPathTabla(nombre_tabla);

	 printf("obtuve su path: %s\n", path_tabla);
//	 printf("path: %s\n" , path_tabla);
	 printf("creo el directorio\n");
	 crear_directorio(path_tabla);

	 printf("creo la metadata\n");
	 crear_metadata(nombre_tabla, criterio , datos_create->numero_particiones, datos_create->compactacion); //MAL?

	 printf("creo los archivos particiones\n");
	 crear_archivos_particiones(nombre_tabla, datos_create->numero_particiones);

	 //log_info(logger_lissandra, "### CREATE REALIZADO CON EXITO ! ####\n");

	 printf("Creo un hilo de compactacion\n");
	 correr_compactacion(datos_create->compactacion, nombre_tabla);
	 printf("Hilo de compactacion creado\n");

	 free(path_tabla);

 }

t_list* request_describe(describe_t request){

	printf("Realizo la request\n");

	t_list* metadatas;

	if( request->global ){

		printf("Realizo request_describe_global\n");
		metadatas = request_describe_global();
		printf("Termine de realizar_describe_global\n");

	}else{

		printf("Realizo request_describe_particular\n");

		Metadata metadata_obtenida = request_describe_particular((char*) request->tabla->buffer);

		printf("Termine de hacer request_describe_particular\n");

		printf("La estructura metadata (que obtuve al realizar el request_describe) tiene:\n");
		printf("tabla: %s\n",(char *) metadata_obtenida->tabla->buffer);
		printf("consistencia: %s\n", (char *)metadata_obtenida->consistencia->buffer);
		printf("particiones: %i\n", metadata_obtenida->particiones);
		printf("compactacion: %i\n",metadata_obtenida->tiempo_compactacion);

		if(metadata_obtenida != NULL){

			printf("Creo la lista de metadatas\n");
			metadatas = list_create();

			printf("Le añado metadata obtenida\n");
			list_add(metadatas, metadata_obtenida);

		}else{
			metadatas = NULL;
		}

	}

	return metadatas;

}


Metadata request_describe_particular(char* nombre_tabla){

	//log_info(logger_lissandra, "### SOLICITUD DE -- DESCRIBE -- para %s\n", nombre_tabla); // LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

	t_config* config_describe;

	Metadata metadata_tabla;

	if(existe_la_tabla(nombre_tabla)){
		printf("Existe la tabla!\n");

		printf("Obtengo el path metadata de la tabla: \n");
		char* path_tabla_metadata = obtener_path_metadata_de_tabla(nombre_tabla);
		printf("path de la metadata: %s\n" , path_tabla_metadata);

		int particiones;
		char* consistencia;
		int tiempo_compactacion;

		printf("Uso el config\n");
		config_describe = config_create(path_tabla_metadata);

		particiones = config_get_int_value(config_describe , "PARTITIONS");
		consistencia = config_get_string_value(config_describe , "CONSISTENCY");
		tiempo_compactacion = config_get_int_value( config_describe , "COMPACTION_TIME");

		printf("Obtuvo los datos con el config: \n");
		printf("particiones: %i\n", particiones);
		printf("consistencia: %s\n", consistencia);
		printf("tiempo_compactacion: %i\n", tiempo_compactacion);
//		log_info(logger_lissandra, "Particiones de %s = %d\n", nombre_tabla , particiones);
//		log_info(logger_lissandra, "Consistencia de %s : %s\n", nombre_tabla, consistencia);
//		log_info(logger_lissandra, "Tiempo de Compactacion de %s : %d\n", nombre_tabla, tiempo_compactacion);

		printf("Creo una estructura metadata para enviarle a la memoria\n");
		metadata_tabla = crear_estructura_metadata(nombre_tabla, consistencia, particiones, tiempo_compactacion);

//		log_info(logger_lissandra, "### DESCRIBE REALIZADO CON EXITO ! ###\n");				// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

		printf("destruyo el config\n");
		config_destroy(config_describe);

		free(path_tabla_metadata);

	 }else{
		printf("no existe la tabla\n");
//		log_error(logger_lissandra, "### NO EXISTE LA TABLA PEDIDA ###\n");					// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

		metadata_tabla = NULL;
	 }

	return metadata_tabla;
}

t_list* request_describe_global(void){

//	log_info(logger_lissandra, "### SOLICITUD DE -- DESCRIBE GLOBAL --\n");					// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

	char* path_directorio_tabla = obtenerPathDirectorio_Tablas();

	DIR *dir1, *dir2;
	struct dirent *tabla, *tabla_particular;

	t_list* metadatas = list_create();

	if((dir1 = opendir(path_directorio_tabla)) != NULL){

		while((tabla = readdir(dir1)) != NULL){

			if(!string_equals_ignore_case(tabla->d_name, ".") && !string_equals_ignore_case(tabla->d_name, "..")){

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

					closedir(dir2); //CHINO? esto nose si estara bien, pero abajo cerrar el dir1, supongo que el dir2 tambien

				}

				free(path_para_tabla_particular);

			}

		}

	}

	closedir(dir1);
	free(path_directorio_tabla);

	//no deberia pasar, pero q c yo
	if(list_size(metadatas) <= 0){
		list_destroy(metadatas);
		metadatas = NULL;
	}

//	log_info(logger_lissandra, "### DESCRIBE GLOBAL REALIZADO CON EXITO ! ###\n");				// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!


	return metadatas;
}




void request_drop(Drop request_drop){

	char* nombre_tabla = (char *)request_drop->tabla->buffer;

//	log_info(logger_lissandra, "### SOLICITUD DE -- DROP -- para %s\n", nombre_tabla);			// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

	DIR *dir1, *dir2;

	struct dirent* tabla, *tabla_particular;

	char* path_directorio_tabla = obtenerPathDirectorio_Tablas();

//	abortar_hilo_compactador(nombre_tabla);//ESTO NO SE SI VA ACA O MAS ABAJO


	if((dir1 = opendir(path_directorio_tabla)) != NULL){
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
								Particion particion = leer_particion(path_para_archivo);
								printf("Se va a eliminar el siguiente archivo: \n");
								printf("%s\n", path_para_archivo);
								mostrar_particion(particion);
								printf("\n\n");
								liberar_particion(particion);
								eliminar_particion(path_para_archivo);
								printf("Estados despues de eliminar el archivo:\n");
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
	}

	closedir(dir1);

	free(path_directorio_tabla);

//	log_info(logger_lissandra, "### DROP REALIZADO CON EXITO ! ###\n");						// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!
}






