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
 		closedir(dir);
 		free(path_tabla);

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

	//tiene semaforo de bitmap

	Particion base = crear_particion(0);

	pthread_rwlock_wrlock(&semaforo_bitmap);

	int indice_vacio = buscar_primer_indice_vacio();

	aniadir_bloque(base, indice_vacio);

	set_estado(indice_vacio, OCUPADO);

	pthread_rwlock_unlock(&semaforo_bitmap);

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

	free(pathBloque);

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

		if(string_ends_with(path_particion, ".tmp")){
			log_info(logger_dump, "Le actualizo a la particion, el bloque: %i", *bloque_escribir);
		}
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

				free(path_particion); //agregado para memory leak
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

void liberar_dump(void){

	pthread_cancel(hilo_dump);

	realizar_dump();

}

void* ciclo_dump(void* argumentos){

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);


	while(1){

		usleep(tiempo_dump * 1000);
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		log_info(logger_dump, "INICIO DEL DUMP");
		realizar_dump();
		log_info(logger_dump, "TERMINO EL DUMP\n");

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	}

}

void realizar_dump(void){

	t_list* dato_de_tabla;

	void _crear_temporal(char* nombre_tabla, void* elementoDeMemtable){

		dato_de_tabla = (t_list*) elementoDeMemtable;

		if(!existe_la_tabla(nombre_tabla)){

			log_info(logger_dump, "No existe la tabla %s", nombre_tabla);

			return;
		}

		log_info(logger_dump, "Existe la tabla %s\n", nombre_tabla);

		thread_args* argumento_tabla = dictionary_get(diccionario_compactador, nombre_tabla);

		//SEMAFORO TABLA ESPECIFICA
//		printf("[DUMP] VOY A AGARRAR EL SEMAFORO DE LA TABLA ESPECIFICA\n");

		pthread_rwlock_wrlock(&(argumento_tabla->lock_tabla));

		log_info(logger_dump, "BLOQUEE LA TABLA PARA GENERAR TMP");

		char* path_temporal = obtenerPathParaTemporalEnLaTabla(nombre_tabla);

		crear_archivo_particion(path_temporal);

		log_info(logger_dump, "Cree el tmp en %s", path_temporal);

		void _cargar_a_temporal(void* _dato){

			dato_t* dato = (dato_t *)_dato;

			log_info(logger_dump, "Le  cargo el dato %i;%i;%s\n", dato->timestamp, dato->key, dato->value);

			cargar_a_particion(path_temporal, dato);

			log_info(logger_dump, "Cargado correctamente");

		}

		list_iterate(dato_de_tabla, _cargar_a_temporal );

		//DESBLOQUEO LA TABLA ESPECIFICA
		pthread_rwlock_unlock(&(argumento_tabla->lock_tabla));
//		printf("[DUMP] LIBERO EL SEMAFORO DE TABLA ESPECIFICA\n");

		free(path_temporal);

	}

//	printf("[DUMP] VOY A AGARRAR EL SEMAFORO DE MEMTABLE\n");
	//SEMAFORO MEMTABLE

	//SEMAFORO DICCIONARIO COMPACTACION

	pthread_rwlock_rdlock(&(lock_diccionario_compactacion));

	log_info(logger_dump,"[DUMP] AGARRE EL SEMAFORO DEL DICCIONARIO COMPACTADOR");

	pthread_rwlock_wrlock(&(lock_memtable));

	log_info(logger_dump,"[DUMP] AGARRE EL SEMAFORO DEL MEMTABLE");

	dictionary_iterator(memtable, _crear_temporal);

//	printf("[DUMP] LIBERO EL SEMAFORO DE DICCIONARIOA\n");
	pthread_rwlock_unlock(&(lock_diccionario_compactacion));

	log_info(logger_dump, "LIBERE SEMAFORO COMPACTACION");


	log_info(logger_dump, "Se vacia la memtable");

	vaciar_memtable();

	//LIBERO SEMAFORO MEMTABLE
//	printf("[DUMP] LIBERO EL SEMAFORO DE MEMTABLEA\n");
	pthread_rwlock_unlock(&(lock_memtable));

	log_info(logger_dump,"[DUMP] LIBERE MEMTABLE");

}

void vaciar_memtable(){

	void _remover_listas(void* elemento_memtable){

		t_list* lista_de_tabla = (t_list* ) elemento_memtable;

		list_destroy_and_destroy_elements(lista_de_tabla, liberar_dato);

	}

	dictionary_clean_and_destroy_elements(memtable,  _remover_listas);
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
	char delimitador[2] = ".";

	if(string_ends_with(path  , "tmp")){

		aux = string_split(path, delimitador);

		numero = atoi(aux[0]);

		liberar_puntero_doble(aux);

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












