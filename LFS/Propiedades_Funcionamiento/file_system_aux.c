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
 	free(path_tabla);

 	return 0;

}


void crear_directorio(char* path){

	mkdir(path, 0700);

}


void crear_archivos_particiones(char* nombre_tabla, int numero_particiones){

	char* path_particion;

	for (int i = 0; i < numero_particiones ; i++ ){

		path_particion = obtenerPath_ParticionTabla(nombre_tabla, i);

		crear_archivo_particion(path_particion);

		free(path_particion);

	}

}

void crear_archivo_particion(char* path){

	Particion base = crear_particion(0);

	int indice_vacio = buscar_primer_indice_vacio();

	aniadir_bloque(base, indice_vacio);

	set_estado(indice_vacio, OCUPADO);

	crear_bloque_vacio( indice_vacio );

	FILE* archivo = fopen(path, "wb");

	fwrite(&(base->size), sizeof(int), 1, archivo);

	int* primer_bloque = list_get(base->bloques, 0);

	fwrite(primer_bloque, list_size(base->bloques) * sizeof(int), 1, archivo);

	fclose(archivo);

	liberar_particion(base);


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

void inicializar_dump(void){

	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&hilo_dump, &attr, ciclo_dump, NULL);

	pthread_attr_destroy(&attr);

}

void* ciclo_dump(void* argumentos){


	while(1){


		usleep(tiempo_dump * 1000);
		log_info(logger_lissandra, "INICIO DEL DUMP");
		realizar_dump();
		log_info(logger_lissandra, "TERMINO EL DUMP\n");

	}

}

void realizar_dump(){
	t_list* dato_de_tabla;

	void _crear_temporal(char* nombre_tabla, void* elementoDeMemtable){

		printf("\nEmpiezo a iterar la memtable\n");
		dato_de_tabla = (t_list*) elementoDeMemtable;
		printf("Tabla agarrada de la memtable: %s\n", nombre_tabla);

		char* path_temporal = obtenerPathParaTemporalEnLaTabla(nombre_tabla);

		printf("Path para temporal dentro de esa tabla: %s\n", path_temporal);

		thread_args* argumento_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

		//SEMAFORO TABLA ESPECIFICA
		pthread_rwlock_wrlock(&(argumento_tabla->lock_tabla));

		crear_archivo_particion(path_temporal);

		printf("Se creo el temporal!\n");

		void _cargar_a_temporal(void* _dato){

			printf("-- %s\n\n", path_temporal);
			dato_t* dato = (dato_t *)_dato;
			cargar_a_particion(path_temporal, dato);

		}

		printf("Ahora voy a cargar el temporal en el path: \n");
		list_iterate(dato_de_tabla, _cargar_a_temporal );

		//DESBLOQUEO LA TABLA ESPECIFICA
		pthread_rwlock_unlock(&(argumento_tabla->lock_tabla));

		free(path_temporal);

	}

	//SEMAFORO MEMTABLE
	pthread_rwlock_wrlock(&(lock_memtable));
	//SEMAFORO DICCIONARIO COMPACTACION
	pthread_rwlock_rdlock(&(lock_diccionario_compactacion));

	dictionary_iterator(memtable, _crear_temporal);

	//aaaaaaaaaaaaaaaaaaaaaaaa x si llegara a pasar algo en medio de la iteracion, osea, no se, ponele que alguien quiere insertar algo y podría hacerlo en el medio de
	//alguna iteraacion,ESO
	pthread_rwlock_unlock(&(lock_diccionario_compactacion));


	vaciar_memtable();

	//LIBERO SEMAFORO MEMTABLE
	pthread_rwlock_unlock(&(lock_memtable));

}

void vaciar_memtable(){


	void _remover_listas(char* nombre_tabla , void* elemento_memtable){

		t_list* lista_de_tabla = (t_list* ) elemento_memtable;

		list_clean_and_destroy_elements(lista_de_tabla, liberar_dato);

	}

	dictionary_iterator(memtable,  _remover_listas);
}

bool no_es_ubicacion_prohibida(char* path){

	char *ubicacionesProhibidas[3];

	ubicacionesProhibidas[0] = ".";
	ubicacionesProhibidas[1] = "..";
	ubicacionesProhibidas[2] = "Metadata.config";

	if(!string_equals_ignore_case(path, ubicacionesProhibidas[0]) && !string_equals_ignore_case(path, ubicacionesProhibidas[1]) && !string_equals_ignore_case(path, ubicacionesProhibidas[2])){
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

	char* extension = extension_del_archivo(path);

	if(string_equals_ignore_case("tmp", extension)){

		aux = string_split(path, delimitador);
		numeroString = string_substring_from(aux[0], 1);

		liberar_puntero_doble(aux);

		numero = atoi(numeroString);

		free(numeroString);
		free(extension);
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

	char* extension = malloc(string_length(aux[1]));

	memcpy(extension, aux[1], string_length(aux[1]));

	liberar_puntero_doble(aux);

	return extension;
}












