/*
 * API_Lissandra.c
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */

#include "API_Lissandra.h"


void realizar_select(char* nombre_tabla , int key ){


	int particion_objetivo; //en que particion de la tabla tengo que buscar
	int particiones_metadata, tiempo_compactacion;
	char* consistencia;

	if(existe_la_tabla(nombre_tabla)){

		printf("SI EXISTE YUPII :D\n\n");

		obtener_metadata (&consistencia , &particiones_metadata, &tiempo_compactacion);

		printf("CONSISTENCY = %s\n", consistencia);
		printf("PARTICIONES = %d\n" , particiones_metadata);
		printf("TIEMPO DE COMPACTACION = %d\n" , tiempo_compactacion);

		particion_objetivo = calcular_particion(particiones_metadata , key);

		printf("PARTICION OBJETIVO = %d\n\n" , particion_objetivo);

	}else{

		printf("FALLOO NOOO D:\n");

	}


}

void insert(char* nombre_tabla , int key , char* valor, unsigned timestamp ){

	int particiones_metadata, tiempo_compactacion;
	char* consistencia;

	if(existe_la_tabla(nombre_tabla)){

		obtener_metadata (&consistencia , &particiones_metadata, &tiempo_compactacion);



	}else{

		printf("FALLO EL INSERT :D\n");

		//exit(-1);
	}

}



int existe_la_tabla(char* tabla){

	DIR* dir = opendir(tabla);

	if(dir){
		return dir;
	}else {
		printf("El directorio no existe");
		return dir;
	}

	closedir(dir);
}

int calcular_particion( int particion_metadata ,int key){

	return key % particion_metadata;

}

void obtener_metadata(char** consistencia , int* particion, int* tiempo_compactacion){

	t_config* metadata_config;

	metadata_config = config_create("Tablas/Tabla_A/Metadata.config");

	*consistencia = config_get_string_value(metadata_config,"CONSISTENCY");

	*particion = config_get_int_value(metadata_config, "PARTITIONS");

	*tiempo_compactacion = config_get_int_value(metadata_config, "COMPACTION_TIME");

}


void setear_metadata(){

	printf("HOLA");

	t_config* metadata_config;

	metadata_config = config_create("Tablas/Tabla_A/Metadata.config");

	config_set_value(metadata_config, "CONSISTENCY", "SC");
	config_set_value(metadata_config, "PARTITIONS", "1");
	config_set_value(metadata_config, "COMPACTION_TIME" , "300");
	config_save(metadata_config);
	config_destroy(metadata_config);

	printf("se configuro el metadata lol\n");


}





