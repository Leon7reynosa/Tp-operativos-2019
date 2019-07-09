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

	printf("\nVoy a leer la particion\n");

	Particion particion_contenido;
	int size;

	FILE* particion = fopen(path_particion, "r");

	fread(&size, sizeof(int), 1, particion);

	particion_contenido = crear_particion(size);

	while(!feof(particion)){ //ASUMO QUE ESTO LO TIRA BIEN OK? si rompe posiblemente sea esto

		printf("Entre a leer un bloque\n");

		int* bloque = malloc(sizeof(int));

		fread(bloque, sizeof(int), 1, particion);

		if(bloque_valido(*bloque)){
			printf("Bloque valido!\n");
			list_add(particion_contenido->bloques, bloque);
		}
	}

	fclose(particion);

	return particion_contenido;
	printf("Finalizo lectura de particion\n\n\n");
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


bool analizar_dato(u_int16_t key, char* dato){

	char** aux = string_split(dato, ";");

	char* key_en_string = string_itoa(key);
	bool nani = string_equals_ignore_case(aux[1], key_en_string);

	free(key_en_string);
	liberar_puntero_doble(aux);

	return nani;



}


//bloque_siguiente, si no hay siguiente, tiene que ser menor que 0 ( < 0)
char* buscar_dato_bloque(u_int16_t key, int bloque_a_analizar, int bloque_siguiente){

	char* path_bloque = obtenerPath_Bloque(bloque_a_analizar);

	int fd_bloque_anal = open(path_bloque, O_RDONLY , S_IRUSR);

	struct stat* atributos = malloc(sizeof(struct stat));

	fstat(fd_bloque_anal, atributos);

	char* datos = mmap(NULL, atributos->st_size, PROT_READ, MAP_SHARED, fd_bloque_anal, 0);

	Bloque bloque = crear_bloque(bloque_a_analizar, datos);

	munmap(datos, atributos->st_size);

	int index_dato = 0;

	char* dato_cortado;

	bool _tenes_la_key(void* _dato_a_analizar){
		//TODO PASO 2: analizo el dato de la lista
		char* dato_a_analizar = (char *) _dato_a_analizar;

		//TODO SI NO ES EL ULTIMO DATO
		if(index_dato < list_size(bloque->datos)){

			//TODO LO ANALIZO NORMAL
			return analizar_dato(key, dato_a_analizar);

		}else{

			//ME FIJO SI EL ULTIMO TERMINA CON BARRITA N, entonces esta completo (entra al else)
			if(!string_ends_with(dato_a_analizar, "\n")){

				//TODO SI ES EL ULTIMO DATO, PRIMERO ME FIJO SI EL SIGUIENTE ES VALIDO
				if(bloque_siguiente >= 0){
					//SI ES VALIDO, OSEA ME PASARON UN BLOQUE SIGUIENTE

					path_bloque = obtenerPath_Bloque(bloque_a_analizar);

					//ABRO EL BLOQUE SIGUIENTE
					int fd_bloque_siguiente = open(path_bloque, O_RDONLY , S_IRUSR);

					fstat(fd_bloque_siguiente, path_bloque);

					datos = mmap(NULL, atributos->st_size, PROT_READ, MAP_SHARED, fd_bloque_anal, 0);

					Bloque bloque_siguiente = crear_bloque(bloque_siguiente, datos);

					munmap(fd_bloque_siguiente, atributos->st_size);

					//OBTENGO EL PRIMER DATO DEL BLOQUE SIGUIENTE (QUE SERIA EL CORTADO)
					dato_cortado = list_get(bloque_siguiente, 0);

					char* dato_cortado_aux = string_new();

					string_append(&dato_cortado_aux, dato_a_analizar);
					string_append(&dato_cortado_aux, dato_cortado);

					liberar_bloque(bloque_siguiente);

					bool nani = analizar_dato(key, dato_cortado_aux);

					free(dato_cortado_aux);

					close(fd_bloque_siguiente);

					return nani;


				}else{

					return false;

					}

				}else{

					return analizar_dato(key, dato_a_analizar);

				}
			}

			index_dato++;
		}

	//TODO PASO 1: itero la lista de datos del bloque a analizar buscando el dato
	char* dato_encontrado = list_find(bloque->datos, _tenes_la_key);

	char* dato_completo = NULL;

	if(dato_encontrado != NULL){

		dato_completo = string_new();

		string_append(dato_completo, dato_encontrado);

		if(dato_cortado != NULL){

			string_append(&dato_completo, dato_encontrado);

		}

	}

	free(dato_cortado);

	free(atributos);

	close(fd_bloque_anal);

	liberar_bloque(bloque);

	return dato_completo;

}

//ESTA HAY QUE HACERLA BIEN
dato_t* buscar_dato_en_particion(char* path_particion_a_buscar , int key){
//asumimos que hay una sola key en la particion

	Particion particion = leer_particion(path_particion_a_buscar);

	int index_siguiente = 1;

	dato_t* dato_encontrado = NULL;

	bool _tiene_key(void* _bloque){

		int* bloque = (int *)_bloque;



		index_siguiente++;
	}

	list_any_satisfy(particion->bloques, _tiene_key);

}



void aniadir_bloque(Particion particion, int bloque){

	int* bloque_aux = malloc(sizeof(int));

	*bloque_aux = bloque;

	list_add(particion->bloques, bloque_aux);

}
