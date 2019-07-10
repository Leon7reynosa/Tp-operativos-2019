/*
 * particion_contenido.c
 *
 *  Created on: 7 jul. 2019
 *      Author: utnso
 */


#include"particion_contenido.h"

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

	FILE* particion = fopen(path_particion, "r");

	fread(&size, sizeof(int), 1, particion);

	particion_contenido = crear_particion(size);

	while(!feof(particion)){ //ASUMO QUE ESTO LO TIRA BIEN OK? si rompe posiblemente sea esto

		int* bloque = malloc(sizeof(int));

		fread(bloque, sizeof(int), 1, particion);

		if(bloque_valido(*bloque)){

			list_add(particion_contenido->bloques, bloque);
		}
	}

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


	list_iterate(particion_lectura->bloques, _tamanio_por_bloque);

	particion_lectura->size = size_actualizado;

	reescribir_bloques_en_particion(path_particion, particion_lectura);
}

void mostrar_particion(Particion particion){

	printf("Bytes = %i\n", particion->size);

	void _printear(void* _bloque){

		int* bloque = (int *)_bloque;

		printf("---BLOQUE: %i---\n ", *bloque);

		mostrar_bloque(*bloque);

		printf("Termine de leer el bloque!\n");

	}

	list_iterate(particion->bloques, _printear);


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

	char** aux = string_split(dato, ";");

	char* key_en_string = string_itoa(key);
	bool nani = string_equals_ignore_case(aux[1], key_en_string);

	free(key_en_string);
	liberar_puntero_doble(aux);

	return nani;



}


//bloque_siguiente, si no hay siguiente, tiene que ser menor que 0 ( < 0)
char* buscar_dato_bloque(u_int16_t key, int bloque_a_analizar, int indice_bloque_siguiente){

	char* path_bloque = obtenerPath_Bloque(bloque_a_analizar);

	int fd_bloque_anal = open(path_bloque, O_RDONLY , S_IRUSR);

	struct stat* atributos = malloc(sizeof(struct stat));

	fstat(fd_bloque_anal, atributos);

	char* datos = mmap(NULL, atributos->st_size, PROT_READ, MAP_SHARED, fd_bloque_anal, 0);

	Bloque bloque = crear_bloque(bloque_a_analizar, datos);

	munmap(datos, atributos->st_size);

	int index_dato = 0;

	char* dato_cortado = NULL;

	bool _tenes_la_key(void* _dato_a_analizar){


		//TODO PASO 2: analizo el dato de la lista
		char* dato_a_analizar = (char *) _dato_a_analizar;

		//TODO SI NO ES EL ULTIMO DATO
		if(index_dato < (list_size(bloque->datos)) - 1){ //lion

			//TODO LO ANALIZO NORMAL
			index_dato++; //lion
			return analizar_dato(key, dato_a_analizar);

		}else{

				//TODO SI ES EL ULTIMO DATO, PRIMERO ME FIJO SI EL SIGUIENTE ES VALIDO
				if(indice_bloque_siguiente >= 0){
					//SI ES VALIDO, OSEA ME PASARON UN BLOQUE SIGUIENTE

					path_bloque = obtenerPath_Bloque(indice_bloque_siguiente); //lion

					printf("path: %s\n" , path_bloque);

					//ABRO EL BLOQUE SIGUIENTE
					int fd_bloque_siguiente = open(path_bloque, O_RDONLY , S_IRUSR);

					fstat(fd_bloque_siguiente, atributos);


					datos = mmap(NULL, atributos->st_size, PROT_READ, MAP_SHARED, fd_bloque_siguiente, 0); //lion

					printf("datos map : %s\n" , datos);

					Bloque bloque_siguiente = crear_bloque(indice_bloque_siguiente, datos);


					munmap(fd_bloque_siguiente, atributos->st_size);

					if(es_dato_cortado((char *) list_get(bloque_siguiente->datos, 0)) || es_dato_cortado((char * ) list_get(bloque->datos, bloque->datos->elements_count - 1))){

						//OBTENGO EL PRIMER DATO DEL BLOQUE SIGUIENTE (QUE SERIA EL CORTADO)
						dato_cortado = string_duplicate((char *) list_get(bloque_siguiente->datos, 0));

						char* dato_cortado_aux = string_new();

						string_append(&dato_cortado_aux, dato_a_analizar);
						string_append(&dato_cortado_aux, dato_cortado);

						printf("dato cortado : %s\n" , dato_cortado);

						liberar_bloque(bloque_siguiente);

						bool nani = analizar_dato(key, dato_cortado_aux);

						free(dato_cortado_aux);

						return nani;  //lo que pasa aca es que retornamos un bool true o false, y no retornamos el bloque_cor
					}

					close(fd_bloque_siguiente);

				}else{

					return false;

					}

				}

				index_dato++;
			}




	//TODO PASO 1: itero la lista de datos del bloque a analizar buscando el dato
	char* dato_encontrado = list_find(bloque->datos, _tenes_la_key);

	char* dato_completo = NULL;

	if(dato_encontrado != NULL){

		dato_completo = string_new();

		string_append(&dato_completo, dato_encontrado);

		if(dato_cortado != NULL){
			string_append(&dato_completo, dato_cortado);

		}

	}


	printf("LLEGUE HASTA ACA!\n");
	//free(dato_cortado);

	free(atributos);

	close(fd_bloque_anal);

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

//ESTA HAY QUE HACERLA BIEN
dato_t* buscar_dato_en_particion(char* path_particion_a_buscar , int key){
//asumimos que hay una sola key en la particion

	Particion particion = leer_particion(path_particion_a_buscar);

	int index_bloque;

	dato_t* dato_encontrado = NULL;

	char* dato;

	int* bloque;

	int *bloque_siguiente;

	for( index_bloque = 0 ; index_bloque < list_size(particion->bloques) ; index_bloque++){

		bloque = list_get(particion->bloques, index_bloque);

		char* dato_encontrado;

		if( index_bloque < (list_size(particion->bloques) -1 )){

			int *bloque_siguiente = list_get( particion->bloques , index_bloque + 1 ) ;

			dato  = buscar_dato_bloque(key, *bloque, *bloque_siguiente);

			printf("dato: %s\n" , dato);

		}else{

			dato = buscar_dato_bloque(key , *bloque  , -1);
		}

		if(dato != NULL){


			dato_encontrado = convertir_a_dato(dato);
			printf("entre aca\n");

			return dato_encontrado;

		}

	}

	printf("No se pudo encontrar el dato\n");
	return dato_encontrado;
}


void aniadir_bloque(Particion particion, int bloque){

	int* bloque_aux = malloc(sizeof(int));

	*bloque_aux = bloque;

	list_add(particion->bloques, bloque_aux);
}

