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
	char* pathTabla = obtenerPathTabla(nombre_tabla);
	char metadata[] = "Metadata.config";

	char* pathMetadata = malloc(strlen(pathTabla) + strlen(metadata) + 1);

	strcpy(pathMetadata,pathTabla);

	free(pathTabla);
	strcat(pathMetadata,metadata);

	return pathMetadata;
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

	printf("HOLA");

	t_config* metadata_config;

	metadata_config = config_create("Tablas/Tabla_B/Metadata.config");

	config_set_value(metadata_config, "CONSISTENCY", "SC");
	config_set_value(metadata_config, "PARTITIONS", "1");
	config_set_value(metadata_config, "COMPACTION_TIME" , "300");
	config_save(metadata_config);
	config_destroy(metadata_config);

	printf("se configuro el metadata lol\n");
}

char* obtenerPathTabla(char* nombre_Tabla){
	char aux_nombreTabla[strlen(nombre_Tabla) + 1];
	aux_nombreTabla[strlen(nombre_Tabla) + 1] = '\0';
	char prefijo[] = "Tablas/";
	char extension[] = "/";

	int cantidad = strlen(nombre_Tabla) + strlen(prefijo) + strlen(extension) + 1;

	char aux_path[cantidad];
	char* path = malloc(cantidad);

	strcpy(aux_nombreTabla, nombre_Tabla);

	strcpy(aux_path, prefijo);

	strcat(aux_path, aux_nombreTabla);

	strcat(aux_path, extension);

	strcpy(path,aux_path);

	return path;
}
/*
void crear_metadata(char* consistencia, int particion, int tiempo_Compactacion){
	t_config* metadata_config;
	char laconchadetumadreleon[20];
	FILE *aux = fopen("Tablas/Tabla_B/Metadata.config", "w");
	fclose(aux);
	metadata_config = config_create("Tablas/Tabla_B/Metadata.config");
	config_set_value(metadata_config,"CONSISTENCY", consistencia);

	itoa(particion,laconchadetumadreleon,10);

	config_set_value(metadata_config, "PARTITIONS", laconchadetumadreleon);

	itoa(tiempo_Compactacion,laconchadetumadreleon,10);

	config_set_value(metadata_config, "COMPACTION_TIME", laconchadetumadreleon);
	config_save(metadata_config);
	config_destroy(metadata_config);

}
*/
