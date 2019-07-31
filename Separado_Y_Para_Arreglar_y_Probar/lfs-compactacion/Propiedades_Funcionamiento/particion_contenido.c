/*
 * particion_contenido.c
 *
 *  Created on: 7 jul. 2019
 *      Author: utnso
 */


#include"particion_contenido.h"

void eliminar_particion(char* path_particion){


	printf("\n>Se ELIMINARÁ la siguiente Particion:\n");

	Particion particion = leer_particion(path_particion);

	void _eliminar_bloque(void* _nro_bloque){

		int* nro_bloque = (int *)_nro_bloque;

		eliminar_bloque(*nro_bloque);

	}

	list_iterate(particion->bloques, _eliminar_bloque);

	liberar_particion(particion);

}

Particion crear_particion(int size){

	Particion nueva_particion = malloc(sizeof(struct particionContenido));

	nueva_particion->size = size;

	nueva_particion->bloques = list_create();

	return nueva_particion;

}

void liberar_particion(Particion particion){



	void _destruir_bloques(void* bloque){//este nada mas es un int

		free(bloque);

	}

	list_destroy_and_destroy_elements(particion->bloques, _destruir_bloques);

	free(particion);

}

Particion leer_particion(char* path_particion){

	Particion particion_contenido;
	int size;

	FILE* particion = fopen(path_particion, "rb");

	if(!particion){
		printf("\nNO se pudo abrir la particion\n");
		exit(1);
	}

	fseek(particion, 0, SEEK_SET);

	fread(&size, sizeof(int), 1, particion);

	particion_contenido = crear_particion(size);

	int* bloque = malloc(sizeof(int));

	fread(bloque, sizeof(int), 1, particion);

	while(!feof(particion)){ //ASUMO QUE ESTO LO TIRA BIEN OK? si rompe posiblemente sea esto

		list_add(particion_contenido->bloques, bloque);

		bloque = malloc(sizeof(int));

		fread(bloque, sizeof(int), 1, particion);

	}

	free(bloque);

	fclose(particion);

	return particion_contenido;

}

bool bloque_valido(int bloque){

	return (bloque < blocks) && (bloque >= 0);

}

void actualiar_particion(char* path_particion){

	Particion particion_lectura = leer_particion( path_particion );
	int size_actualizado = 0;

	//mostrar_particion(particion_lectura);

	void _tamanio_por_bloque(void* bloque_lista){

		int* bloque_para_tamanio = (int*) bloque_lista;

		size_actualizado += tamanio_del_bloque( *bloque_para_tamanio );


	}

	if(string_ends_with(path_particion, ".tmp")){
		log_info(logger_dump, "Actualize la particion. Antes %i bytes ---- Despues %i bytes", particion_lectura->size, size_actualizado );
	}

	list_iterate(particion_lectura->bloques, _tamanio_por_bloque);

	particion_lectura->size = size_actualizado;

	reescribir_bloques_en_particion(path_particion, particion_lectura);

	liberar_particion(particion_lectura); //agregado por memory leak

}

void mostrar_particion(Particion particion){

	printf("\n>[LECTURA PARTICION] CANTIDAD DE BLOQUES: %i\n", list_size(particion->bloques));
	int* aux = list_get(particion->bloques, 1);

	void _printear(void* _bloque){

		int* bloque = (int *)_bloque;

		mostrar_bloque(*bloque);

	}

	list_iterate(particion->bloques, _printear);

	printf("[LECTURA PARTICION] Termine de mostrar la particion\n");
}

int obtener_numero_particion(char* path){
	int numero;
	char** aux;
	char delimitador[2] = ".";

	if(string_equals_ignore_case("bin", extension_del_archivo(path))){
		aux = string_split(path, delimitador);
		numero = atoi(aux[0]);

		return numero;
	}
	else{
		return -1;
	}
}

bool analizar_dato(u_int16_t key, char* dato){

//	printf("DATO ANALIZADO: %s\n", dato);
	char** aux = string_split(dato, ";");

//	printf("KEY antes de pasarlo a string: %i\n", key);

	char* key_en_string = string_itoa(key);
	bool nani = string_equals_ignore_case(aux[1], key_en_string);

//	printf("KEY EN STRING: %s\n", key_en_string);
//	printf("KEY DEL BLOQUE: %s\n", aux[1]);

	free(key_en_string);
	liberar_puntero_doble(aux);

	return nani;



}

dato_t* buscar_dato_en_temporales(char* nombre_tabla, u_int16_t key){

	char* path_tabla = obtenerPathTabla(nombre_tabla);

	DIR* directorio_tabla = opendir(path_tabla);
	struct dirent* entrada_directorio;

	dato_t* dato_mayor = NULL;

	if(directorio_tabla != NULL){

		while((entrada_directorio = readdir(directorio_tabla)) !=NULL){

			if(string_ends_with(entrada_directorio->d_name, ".tmp")){

				char* path_temporal = obtenerPathTabla(nombre_tabla);

				string_append(&path_temporal, "/");
				string_append(&path_temporal, entrada_directorio->d_name);

				dato_t* dato_encontrado = buscar_dato_en_particion(path_temporal, key);

				if(dato_encontrado != NULL){

					if(dato_mayor != NULL){

						dato_t* dato_aux = dato_mayor;

						dato_mayor = timestamp_mas_grande(dato_encontrado, dato_aux);

						liberar_dato(dato_aux);

						liberar_dato(dato_encontrado);

					}

					else{

						dato_mayor = dato_encontrado;

					}

				}

				free(path_temporal);

			}

		}


	}

	closedir(directorio_tabla);
	free(path_tabla);

	return dato_mayor;

}


//bloque_siguiente, si no hay siguiente, tiene que ser menor que 0 ( < 0)
char* buscar_dato_bloque(u_int16_t key, int bloque_a_analizar, int indice_bloque_siguiente){

	Bloque bloque = leer_bloque(bloque_a_analizar);

//	if(list_size(bloque->datos) > 0){
//		printf("[BUSQUEDA] Leo un dato aunquesea: %s\n", list_get(bloque->datos, 0));
//	}

	int index_dato = 0;

	char* dato_cortado = NULL;

	bool _tenes_la_key(void* _dato_a_analizar){


		//TODO PASO 2: analizo el dato de la lista
		char* dato_a_analizar = (char *) _dato_a_analizar;

		//TODO SI NO ES EL ULTIMO DATO
		if(index_dato < (list_size(bloque->datos)) - 1){

			//TODO LO ANALIZO NORMAL
			index_dato++;

			if(es_dato_cortado(dato_a_analizar)){
				return false;
			}

			return analizar_dato(key, dato_a_analizar);

		}else{
				//TODO SI ES EL ULTIMO DATO, PRIMERO ME FIJO SI EL SIGUIENTE ES VALIDO
			if(indice_bloque_siguiente >= 0){

				//SI ES VALIDO, OSEA ME PASARON UN BLOQUE SIGUIENTE
				Bloque bloque_siguiente = leer_bloque(indice_bloque_siguiente);

				if(es_dato_cortado((char *) list_get(bloque_siguiente->datos, 0)) || es_dato_cortado(dato_a_analizar)){
					//OBTENGO EL PRIMER DATO DEL BLOQUE SIGUIENTE (QUE SERIA EL CORTADO)
					dato_cortado = string_duplicate((char *) list_get(bloque_siguiente->datos, 0));

					char* dato_cortado_aux = string_new();

					string_append(&dato_cortado_aux, dato_a_analizar);
					string_append(&dato_cortado_aux, dato_cortado);

					liberar_bloque(bloque_siguiente);

					bool nani = analizar_dato(key, dato_cortado_aux);

					free(dato_cortado_aux);

					return nani;  //lo que pasa aca es que retornamos un bool true o false, y no retornamos el bloque_cor
				}

				liberar_bloque(bloque_siguiente);

			}

			else{

				if(es_dato_cortado(dato_a_analizar)){

					return false;

				}

				return analizar_dato(key, dato_a_analizar);

			}

		}

		index_dato++;
	}

//	printf("Busco entre los datos del bloque, el que estoy buscando\n");
	//TODO PASO 1: itero la lista de datos del bloque a analizar buscando el dato
	char* dato_encontrado = list_find(bloque->datos, _tenes_la_key);

	char* dato_completo = NULL;

	if(dato_encontrado != NULL){

		dato_completo = string_new();

		string_append(&dato_completo, dato_encontrado);

		if(dato_cortado != NULL){

			string_append(&dato_completo, dato_cortado);

			free(dato_cortado);

		}

	}else{
		printf("[BUSQUEDA] No esta el dato en el bloque indicado\n");
	}

	liberar_bloque(bloque);

	return dato_completo;

}

bool es_dato_cortado(char* dato_a_analizar){

	char** dato_dividido = string_split(dato_a_analizar, ";");

	for( int i = 0 ; i < 3 ; i++){

		if( dato_dividido[i] == NULL){

			liberar_puntero_doble(dato_dividido);
			return true;

		}

	}

	liberar_puntero_doble(dato_dividido);
	return false;

}

dato_t* buscar_dato_en_particion(char* path_particion_a_buscar , int key){
//asumimos que hay una sola key en la particion

	Particion particion = leer_particion(path_particion_a_buscar);
	int index_bloque;

//	mostrar_particion(particion);

	dato_t* dato_encontrado = NULL;
	char* dato;

	int* bloque;
	int *bloque_siguiente;

	//por cada bloque de la particion
	for( index_bloque = 0 ; index_bloque < list_size(particion->bloques) ; index_bloque++){

		bloque = list_get(particion->bloques, index_bloque);

		//si el bloque tiene un bloque siguiente
		if( index_bloque < (list_size(particion->bloques) - 1)){

			int *bloque_siguiente = list_get( particion->bloques , index_bloque + 1 ) ;

			dato  = buscar_dato_bloque(key, *bloque, *bloque_siguiente);


		}
		//si no tiene un bloque siguiente
		else{

			dato = buscar_dato_bloque(key , *bloque  , -1);
		}

		//si se encontro el dato!
		if(dato != NULL){

//			printf("[RESULTADO] Encontre el dato!\n");
			dato_encontrado = convertir_a_dato(dato);
//			printf("[RESULTADO] key: %i\n",dato_encontrado->key);
//			printf("[RESULTADO] timestamp: %i\n", dato_encontrado->timestamp);
//			printf("[RESULTADO] value: %s\n", dato_encontrado->value);
			free(dato);

			break;

		}

	}

	if(dato_encontrado == NULL){

		printf("[RESULTADO] No encontre el dato en la particion!\n");

	}

	liberar_particion(particion);

	return dato_encontrado;
}


void aniadir_bloque(Particion particion, int bloque){

	int* bloque_aux = malloc(sizeof(int));

	*bloque_aux = bloque;

	list_add(particion->bloques, bloque_aux);
}


t_list* obtener_datos_particiones(char* nombre_tabla){

	metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

	printf("PRUEBITA\n");

	t_list* lista_datos_particion = list_create();

	printf("PRUEBITA\n");

	char* datos_totales = string_new();

	printf("PRUEBITA\n");

	for (int i = 0; i < metadata_tabla->particion ; i++){

		char* path_particion = obtenerPath_ParticionTabla(nombre_tabla, i);

		printf("PRUEBITA1\n");

		Particion particion_a_obtener = leer_particion(path_particion);

		printf("PRUEBITA1\n");

		free(path_particion); //agregado para mem leak

		printf("PRUEBITA1\n");

	//	int i = 0;

		int fd_bloque;

		void _generar_lista_datos(void* dato_bloque){

			int * bloque = (int* ) dato_bloque;

			char* path_bloque = obtenerPath_Bloque(*bloque);

			printf(">>PATH BLOQUE : %s\n" , path_bloque);

			int fd_bloque = open(path_bloque, O_RDONLY , S_IRUSR);

			if(fd_bloque <= -1){

				perror("Open bloque");
				exit(1);
			}

			struct stat* atributos = malloc(sizeof(struct stat));

			fstat(fd_bloque, atributos);


			char* datos = mmap(NULL, atributos->st_size, PROT_READ, MAP_SHARED, fd_bloque, 0);

			if(datos == MAP_FAILED){
				if(atributos->st_size == 0){

					log_info(logger_compactador, "Particion %i -> bloque: %i, esta vacio", i, *bloque);

					close(fd_bloque);

					free(path_bloque);

					free(atributos);

					return;

				}else{
					perror("No funciono el mmap");
					exit(1);
				}

			}else{

				printf("[MMAP PARTICION] dato append %s\n", datos);

				log_info(logger_compactador, "Particion %i -> bloque: %i, tiene = %s", i, *bloque, datos);

				string_append(&datos_totales, datos);

				munmap(datos, atributos->st_size);

			}

			close(fd_bloque);

			free(path_bloque);

			free(atributos);

		}

		printf("PRUEBITA1\n");

		list_iterate(particion_a_obtener->bloques , _generar_lista_datos);

		printf("PRUEBITA1\n");

		liberar_particion(particion_a_obtener); //agregado por memory leak

		printf("PRUEBITA1\n");

	}

	printf("PRUEBITA\n");

	int j = 0;

	printf("\nDATOS TOTALES: %s\n\n", datos_totales);

	char** cadena_datos = string_split(datos_totales , "\n");

	while( cadena_datos[j] != NULL ){

		list_add(lista_datos_particion , cadena_datos[j]);
		j++;
	}

	free(cadena_datos);

	free(metadata_tabla);

	free(datos_totales);

	return lista_datos_particion;
}


t_list* obtener_datos_temporales(char* nombre_tabla){

	metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

	printf("PRUEBITA TMPC\n");

	t_list* lista_datos_particion = list_create();

	printf("PRUEBITA TMPC\n");

	int cantidad_temporales = obtener_cantidad_de_archivos_tmpc(nombre_tabla);

	printf("PRUEBITA TMPC\n");

	char* datos_totales = string_new();

	printf("PRUEBITA TMPC\n");

	for (int i = 0; i < cantidad_temporales ; i++){

			char* path_particion = obtener_path_de_temporal_para_compactar_de_tabla(nombre_tabla, i);

			printf("PRUEBITA TMPC1\n");

			Particion particion_a_obtener = leer_particion(path_particion);

			printf("PRUEBITA TMPC1\n");

			int fd_bloque;

			void _generar_lista_datos(void* dato_bloque){

				int * bloque = (int* ) dato_bloque;

				char* path_bloque = obtenerPath_Bloque(*bloque);

				printf("BLOQUE: %i\n", *bloque);

				int fd_bloque = open(path_bloque, O_RDONLY , S_IRUSR);

				struct stat* atributos = malloc(sizeof(struct stat));

				fstat(fd_bloque, atributos);

				char* datos = mmap(NULL, atributos->st_size, PROT_READ, MAP_SHARED, fd_bloque, 0);

				if(datos == MAP_FAILED){

					printf("datos == map faiiled\n");
					if(atributos->st_size == 0){

						log_info(logger_compactador, "Particion %i -> bloque: %i, esta vacio", i, *bloque);

						printf("atributos zise = 0\n");

						close(fd_bloque);

						free(path_bloque);
						free(atributos);

						return;

					}else{
						perror("mmap");
						exit(1);
					}

				}

				log_info(logger_compactador, "Particion %i -> bloque: %i, tiene = %s", i, *bloque, datos);

				string_append(&datos_totales,datos);

				munmap(datos, atributos->st_size);

				close(fd_bloque);

				free(path_bloque);

				free(atributos);
			}

			printf("PRUEBITA TMPC1\n");

			list_iterate(particion_a_obtener->bloques , _generar_lista_datos);

			printf("PRUEBITA TMPC1\n");

			liberar_particion(particion_a_obtener);

			printf("PRUEBITA TMPC1\n");

			free(path_particion);

		}

		printf("PRUEBITA TMPC\n");

		int j = 0;

		char** cadena_datos = string_split(datos_totales , "\n");

		printf("PRUEBITA TMPC\n");

		while( cadena_datos[j] != NULL ){

			list_add(lista_datos_particion , cadena_datos[j]);
			j++;
		}

		printf("PRUEBITA TMPC\n");

		free(metadata_tabla);

		printf("PRUEBITA TMPC\n");

		free(datos_totales);

		printf("PRUEBITA TMPC\n");

		free(cadena_datos);

		printf("PRUEBITA TMPC\n");

		return lista_datos_particion;

}



