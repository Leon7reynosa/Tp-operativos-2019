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

	printf("Voy a leer la particion\n");

	Particion particion_contenido;
	int size;

	FILE* particion = fopen(path_particion, "r");

	fread(&size, sizeof(int), 1, particion);

	particion_contenido = crear_particion(size);

	while(!feof(particion)){ //ASUMO QUE ESTO LO TIRA BIEN OK? si rompe posiblemente sea esto

		printf("Entre a leer un bloque\n");

		int* bloque = malloc(sizeof(int));

		fread(bloque, sizeof(int), 1, particion);

		list_add(particion_contenido->bloques, bloque);

	}

	fclose(particion);

	return particion_contenido;

}

void actualiar_particion(char* path_particion){

	printf("path : %s\n" , path_particion);
	Particion particion_lectura = leer_particion( path_particion );
	int size_actualizado = 0;
	printf("Leyo la particion\n");

	mostrar_particion(particion_lectura);

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

//ESTA HAY QUE HACERLA BIEN
dato_t* buscar_dato_en_particion(char*	 path , int key){

	return crear_dato(7,"hola",33125);


}

void aniadir_bloque(Particion particion, int bloque){

	int* bloque_aux = malloc(sizeof(int));

	*bloque_aux = bloque;

	list_add(particion->bloques, bloque_aux);

}
