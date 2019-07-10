/*
 * api_memory.c
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#include "requests.h"

dato_t* request_select(char* nombre_tabla , int key){

	 dato_t* dato_binarios;

	 dato_t* dato_memtable;

	 char* path_particion_a_buscar;

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Existe la tabla en el File System\n");

		 metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);
		 int particion_objetivo = calcular_particion(metadata_tabla->particion , key);
		 path_particion_a_buscar = obtenerPath_ParticionTabla(nombre_tabla, particion_objetivo);

		 dato_binarios = buscar_dato_en_particion(path_particion_a_buscar, key);
         dato_memtable = obtener_dato_con_mayor_timestamp_tabla(nombre_tabla, key);
		 dato_t* dato_mas_nuevo = timestamp_mas_grande(dato_memtable, dato_binarios);

		 return  dato_mas_nuevo;

	 }

	 else{
		 printf("No existe la tabla en el File System\n");
	 }

 }



 void request_insert(char* nombre_tabla, int key, char* valor, time_t timestamp){
	 //faltaria ver cuando no le pasamos el timestamp
	 dato_t* dato_ingresar;
	 metadata_t* metadata_insert;

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Existe la tabla en el File System\n");

		 metadata_insert = obtener_metadata(nombre_tabla);

		 dato_ingresar = crear_dato(key, valor, timestamp);

		 ingresar_a_memtable(dato_ingresar, nombre_tabla);

	 }else{

		 printf("Fallo el insert. \n");
		 //aca deberiamos hacer un log de que fallo.

	 }

 }


 void request_create(char* nombre_tabla, char* criterio, int numero_particiones, int tiempo_compactacion){

	 string_to_upper(nombre_tabla);

	 if(existe_la_tabla(nombre_tabla)){

		 printf("Ya existe la tabla \n");
		 //guardar resultado en un archivo de log y retornar error de dicho resultado
		 exit(1);

	 }

	 char* path_tabla = obtenerPathTabla(nombre_tabla);

	 printf("path: %s\n" , path_tabla);

	 crear_directorio(path_tabla);

	 crear_metadata(nombre_tabla, criterio, numero_particiones, tiempo_compactacion); //MAL?

	 crear_archivos_particiones(nombre_tabla, numero_particiones);
 }


void request_describe(char* nombre_tabla){

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

		printf("Particiones de %s : %d\n", nombre_tabla , particiones);
		printf("Consistencia de %s : %s\n", nombre_tabla, consistencia);
		printf("Tiempo de Compactacion de %s : %d\n", nombre_tabla, tiempo_compactacion );

	 }else{

		 printf("No existe la tabla\n");

	 }

}


void request_describe_global(){

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

				dir2 = opendir(path_para_tabla_particular);

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




void request_drop(char* nombre_tabla){
	DIR *dir1, *dir2;
	struct dirent* tabla, *tabla_particular;
	char* path_directorio_tabla = obtenerPathDirectorio_Tablas();


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
}






