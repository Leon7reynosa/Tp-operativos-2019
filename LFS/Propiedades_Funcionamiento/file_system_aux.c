/*
 * file_system_aux.c
 *
 *  Created on: 7 jul. 2019
 *      Author: utnso
 */

#include"file_system_aux.h"

int existe_la_tabla(char* tabla){

	char* path_tabla = obtenerPathTabla(tabla);
	DIR* dir = opendir(path_tabla);

 	if(dir){
 		return 1;
 	}

 	closedir(dir);
 	return 0;

}


void crear_directorio(char* path){

	mkdir(path, 0700);

}


void crear_archivos_particiones(char* nombre_tabla, int numero_particiones){


	FILE* archivo;
	int indice_vacio;

	char* path_particion;

	for (int i = 0; i < numero_particiones ; i++ ){

		Particion base = crear_particion(0);

		path_particion = obtenerPath_ParticionTabla(nombre_tabla, i);

		indice_vacio = buscar_primer_indice_vacio();

		aniadir_bloque(base, indice_vacio);

		set_estado(indice_vacio, OCUPADO);

		crear_bloque_vacio( indice_vacio );

		archivo = fopen(path_particion, "wb");

		fwrite(&(base->size), sizeof(int), 1, archivo);

		int* primer_bloque = list_get(base->bloques, 0);

		printf("bloques: %i\n", base->bloques->elements_count);

		fwrite(primer_bloque, list_size(base->bloques) * sizeof(int), 1, archivo);

		fclose(archivo);

		free(path_particion);

		liberar_particion(base);

	}

}

void crear_bloque_vacio(int bloque){

	char* pathBloque =  obtenerPath_Bloque(bloque);

	int ficheroBloque = open(pathBloque, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	close(ficheroBloque);

}


void asignar_bloque_vacio_a_particion(char* path_particion, int bloque){


	crear_bloque_vacio(bloque);

	Particion particion_contenido = leer_particion(path_particion);

	aniadir_bloque(particion_contenido, bloque);

	set_estado(bloque, OCUPADO);

	reescribir_bloques_en_particion(path_particion, particion_contenido);

	liberar_particion(particion_contenido);

}


void reescribir_bloques_en_particion( char* path_particion , Particion particion_a_reescribir ){


	FILE* archivo = fopen(path_particion , "w");

	fwrite( &(particion_a_reescribir->size) , sizeof(int) , 1, archivo);


	void _escribir_bloques(void* _bloque){
		int* bloque_escribir = (int *)_bloque;
		fwrite( bloque_escribir  , sizeof(int) , 1, archivo );

	}


	list_iterate(particion_a_reescribir->bloques, _escribir_bloques);

	fclose(archivo);

}


void mostrar_tabla_y_particiones( char* nombre_tabla ){

	DIR* directorio_tabla;
	struct dirent *entrada;

	directorio_tabla = opendir(obtenerPathTabla(nombre_tabla));

	if( directorio_tabla != NULL ){

		while( (entrada = readdir(directorio_tabla)) != NULL ) {


			if( no_es_ubicacion_prohibida(entrada->d_name) ){

				printf("PARTICION: %s\n\n" , entrada->d_name);

				char* path_particion = obtenerPath_ParticionTabla(nombre_tabla, obtener_numero_particion(entrada->d_name));

				Particion particion = leer_particion(path_particion);

				mostrar_bloques(particion->bloques);

				liberar_particion(particion);
			}

		}

	}

}

bool no_es_ubicacion_prohibida(char* path){

	char *ubicacionesProhibidas[3];

	ubicacionesProhibidas[0] = ".";
	ubicacionesProhibidas[1] = "..";
	ubicacionesProhibidas[2] = "Metadata.config";

	if(strcmp(path, ubicacionesProhibidas[0]) && strcmp(path, ubicacionesProhibidas[1]) && strcmp(path, ubicacionesProhibidas[2])){
		return true;
	}
	else{
		return false;
	}

}

int obtenerNumeroTemporal(char* path){
	int numero;
	char** aux;
	char* numeroString;
	char delimitador[2] = ".";

	if(string_equals_ignore_case("tmp", extension_del_archivo(path))){
		aux = string_split(path, delimitador);
		numeroString = string_substring_from(aux[0], 1);
		numero = atoi(numeroString);
		return numero;
	}
	else{
		return -1;
	}
}


char* extension_del_archivo(char* path){

	char* delimitador = ".";
	char** aux;
	aux = string_split(path, delimitador);
	return aux[1];
}












