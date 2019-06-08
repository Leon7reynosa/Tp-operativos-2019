/*
 * metadata.c
 *
 *  Created on: 19 may. 2019
 *      Author: utnso
 */

#include"metadata.h"

metadata_t obtener_metadata(char* nombre_tabla){

	metadata_t obtener_metadata;
	t_config* metadata_config;
	char* path_metadata_tabla = obtener_path_metadata_de_tabla(nombre_tabla);
	char* aux_consistencia;
	metadata_config = config_create(path_metadata_tabla);

	aux_consistencia = config_get_string_value(metadata_config,"CONSISTENCY");
	obtener_metadata.particion= config_get_int_value(metadata_config, "PARTITIONS");
	obtener_metadata.compactacion = config_get_int_value(metadata_config, "COMPACTION_TIME");

	if(strcmp(aux_consistencia,"SC") == 0){
	obtener_metadata.consistencia = SC;
	}
	if(strcmp(aux_consistencia,"SHC") == 0){
	obtener_metadata.consistencia = SHC;
	}
	if(strcmp(aux_consistencia, "EC") == 0){
	obtener_metadata.consistencia = EC;
	}

	return obtener_metadata;
}


char* obtener_path_metadata_de_tabla(char* nombre_tabla){
	char* path = obtenerPathTabla(nombre_tabla);
	string_append(&path, "Metadata.config");


	return path;
}

void mostrar_metadata_de_tabla(char* nombre_Tabla){
	char* pathMetadata = obtener_path_metadata_de_tabla(nombre_Tabla);
	char* consistencia;
	int particiones, compactacion;

	t_config* configMetadata = config_create(pathMetadata);
	particiones = config_get_int_value(configMetadata, "PARTITIONS");
	compactacion = config_get_int_value(configMetadata, "COMPACTION_TIME");
	consistencia = config_get_string_value(configMetadata, "CONSISTENCY");


	printf("CONSISTENCY = %s\n", consistencia);
	printf("PARTITIONS = %i\n", particiones);
	printf("COMPACTION_TIME = %i\n", compactacion);

	config_destroy(configMetadata);
	free(pathMetadata);


}

void setear_metadata(){

	t_config* metadata_config;

	metadata_config = config_create("Tablas/Tabla_B/Metadata.config");

	config_set_value(metadata_config, "CONSISTENCY", "SC");
	config_set_value(metadata_config, "PARTITIONS", "1");
	config_set_value(metadata_config, "COMPACTION_TIME" , "300");
	config_save(metadata_config);
	config_destroy(metadata_config);

	printf("se configuro el metadata lol\n");

}

char* obtenerPathTabla(char* nombre_tabla){

	char* path = string_new();

	char *prefijo = "Tablas/";

	string_append(&path, prefijo);
	string_append(&path, nombre_tabla);
	string_append(&path, "/");

	return path;
}

char* obtenerPath_ParticionTabla(char* nombre_tabla, int particion){
	char* path = string_new();
	path = obtenerPathTabla(nombre_tabla);

	string_append(&path, string_itoa(particion));
	string_append(&path, ".bin");


	return path;

}

char* obtenerPath_Bloque(int indice){
	t_config* g_config = config_create("lissandra.config");
	char* puntoMontaje = config_get_string_value(g_config, "PUNTO_MONTAJE");
	char* path = string_new();

	string_append(&path, puntoMontaje);
	string_append(&path, "/Bloques/");
	string_append(&path, string_itoa(indice));
	string_append(&path, ".bin");

	config_destroy(g_config);

	return path;


}

void crear_metadata(char* nombre_tabla, char* consistencia, int particion, int tiempo_Compactacion){
	t_config* metadata_config;
	char* particionAux = string_new();
	char* tiempo_compactacionAux = string_new();
	char* pathMetadata = obtener_path_metadata_de_tabla(nombre_tabla);

	particionAux = string_itoa(particion);
	tiempo_compactacionAux = string_itoa(tiempo_Compactacion);


	metadata_config = config_create(pathMetadata);
	config_save_in_file(metadata_config, pathMetadata);

	config_set_value(metadata_config,"CONSISTENCY", consistencia);
	config_set_value(metadata_config, "PARTITIONS", particionAux);
	config_set_value(metadata_config, "COMPACTION_TIME", tiempo_compactacionAux);

	config_save(metadata_config);
	config_destroy(metadata_config);

}

