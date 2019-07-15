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

	switch( request_a_operar->cod_op ){

		case SELECT:

			printf(">>SE REALIZARA EL SELECT\n");

			dato_request = request_select( (select_t) request_a_operar->tipo_request );





			//falta hacer mas cosas aca, habria que reenviarlo al pool

			break;

		case INSERT:

			request_insert((insert) request_a_operar->tipo_request );

			break;

		case CREATE:

			request_create( (create) request_a_operar->tipo_request);

			//TODO habria que hacer que le diga al pool que esta todo ok.

			break;

		case DESCRIBE:


			request_describe( (describe_t) request_a_operar->tipo_request );

			printf("TERMINO EL DESCRIBE BRO\n");

			break;

		default:

			//no se aca lol

			break;
	}




}

dato_t* request_select(select_t datos_select){ //hay que modificarla para que reciba un select_t

	 dato_t* dato_binarios;

	 dato_t* dato_memtable;

	 char* path_particion_a_buscar;

	 char* nombre_tabla = (char *)datos_select->tabla->buffer;

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Existe la tabla en el File System\n");

		 metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

		 int particion_objetivo = calcular_particion(metadata_tabla->particion , datos_select->key);

		 path_particion_a_buscar = obtenerPath_ParticionTabla(nombre_tabla, particion_objetivo);

		 dato_binarios = buscar_dato_en_particion(path_particion_a_buscar, datos_select->key);
         dato_memtable = obtener_dato_con_mayor_timestamp_tabla(nombre_tabla, datos_select->key);
		 dato_t* dato_mas_nuevo = timestamp_mas_grande(dato_memtable, dato_binarios);

		 return  dato_mas_nuevo;

	 }

	 else{
		 printf("No existe la tabla en el File System\n");
	 }

 }



 void request_insert(insert datos_insert){ //hay que modificar para que opere con insert
	 //faltaria ver cuando no le pasamos el timestamp
	 dato_t* dato_ingresar;
	 metadata_t* metadata_insert;

	 char* nombre_tabla = (char *) datos_insert->tabla->buffer;

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Existe la tabla en el File System\n");

		 metadata_insert = obtener_metadata(nombre_tabla);

		 dato_ingresar = crear_dato(datos_insert->key, (char *)datos_insert->value, datos_insert->timestamp);

		 ingresar_a_memtable(dato_ingresar, nombre_tabla);

	 }else{

		 printf("Fallo el insert. \n");
		 //aca deberiamos hacer un log de que fallo.

	 }

 }


 void request_create(create datos_create){
//TODO hay que modificar para que trabaje con la estructura create



	 char* nombre_tabla = (char *)datos_create->tabla->buffer;

	 char* criterio = (char *)datos_create->consistencia->buffer;

	 log_info(logger_lissandra, "### SOLICITUD DE -- CREATE -- para\n"
	 			 "TABLA = %s\nNUMERO DE PARTICIONES = %i\nCRITERIO = %s\nTIEMPO_COMPACTACION = %i\n",
	 			 nombre_tabla, datos_create->numero_particiones, criterio, datos_create->compactacion);		//LOGGER AGREGADO !!!!!!!!!!!!

	 string_to_upper(nombre_tabla);

	 if(existe_la_tabla(nombre_tabla)){

		 log_error(logger_lissandra, "### YA EXISTE LA TABLA ###\n", nombre_tabla);  //LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		 return;

	 }

	 char* path_tabla = obtenerPathTabla(nombre_tabla);

//	 printf("path: %s\n" , path_tabla);

	 crear_directorio(path_tabla);

	 crear_metadata(nombre_tabla, criterio , datos_create->numero_particiones, datos_create->compactacion); //MAL?

	 crear_archivos_particiones(nombre_tabla, datos_create->numero_particiones);

	 log_info(logger_lissandra, "### CREATE REALIZADO CON EXITO ! ####\n");

	 correr_compactacion(datos_create->compactacion, nombre_tabla);

 }

void request_describe(describe_t request){

	printf("Realizo la request\n");

	if( request->global ){

		request_describe_global();

	}else{

		request_describe_particular((char*) request->tabla->buffer);

	}

}


void request_describe_particular(char* nombre_tabla){

	log_info(logger_lissandra, "### SOLICITUD DE -- DESCRIBE -- para %s\n", nombre_tabla); // LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

	t_config* config_describe;

	char* path_tabla_metadata = obtener_path_metadata_de_tabla(nombre_tabla);

	printf("path de la metadata: %s\n" , path_tabla_metadata);

	if(existe_la_tabla(nombre_tabla)){

		int particiones;
		char* consistencia;
		int tiempo_compactacion;

		config_describe = config_create(path_tabla_metadata);

		particiones = config_get_int_value(config_describe , "PARTITIONS");
		consistencia = config_get_string_value(config_describe , "CONSISTENCY");
		tiempo_compactacion = config_get_int_value( config_describe , "COMPACTION_TIME");

		log_info(logger_lissandra, "Particiones de %s = %d\n", nombre_tabla , particiones);
		log_info(logger_lissandra, "Consistencia de %s : %s\n", nombre_tabla, consistencia);
		log_info(logger_lissandra, "Tiempo de Compactacion de %s : %d\n", nombre_tabla, tiempo_compactacion);

		printf("Particiones de %s : %d\n", nombre_tabla , particiones);
		printf("Consistencia de %s : %s\n", nombre_tabla, consistencia);
		printf("Tiempo de Compactacion de %s : %d\n", nombre_tabla, tiempo_compactacion );

		log_info(logger_lissandra, "### DESCRIBE REALIZADO CON EXITO ! ###\n");				// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

	 }else{

		log_error(logger_lissandra, "### NO EXISTE LA TABLA PEDIDA ###\n");					// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

	 }

}


void request_describe_global(){

	log_info(logger_lissandra, "### SOLICITUD DE -- DESCRIBE GLOBAL --\n");					// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

	char* path_directorio_tabla = obtenerPathDirectorio_Tablas();

	DIR *dir1, *dir2;
	struct dirent *tabla, *tabla_particular;

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

							request_describe(tabla->d_name);

							break;
						}
					}
				}
			}
		}
	}

	closedir(dir1);

	log_info(logger_lissandra, "### DESCRIBE GLOBAL REALIZADO CON EXITO ! ###\n");				// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

}




void request_drop(char* nombre_tabla){

	log_info(logger_lissandra, "### SOLICITUD DE -- DROP -- para %s\n", nombre_tabla);			// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!

	DIR *dir1, *dir2;

	struct dirent* tabla, *tabla_particular;

	char* path_directorio_tabla = obtenerPathDirectorio_Tablas();

	abortar_hilo_compactador(nombre_tabla);//ESTO NO SE SI VA ACA O MAS ABAJO


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

							unlink(path_para_archivo);
						}
					}
					closedir(dir2);
					rmdir(path_para_tabla_particular);
					break;
				}

			}
		}
	}
	closedir(dir1);

	log_info(logger_lissandra, "### DROP REALIZADO CON EXITO ! ###\n");						// LOGGER AGREGADO !!!!!!!!!!!!!!!!!!!!!!!!!!
}






