/*
 * file_system.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#include"file_system.h"

int existe_la_tabla(char* tabla){

	char* path_tabla = obtenerPathTabla(tabla);

 	DIR* dir = opendir(path_tabla);

 	if(dir){

 		return 1;

 	}

 	closedir(dir);

 	return 0;
}

void crear_directorio(char* path_tabla){

	mkdir(path_tabla, 0700);

}

void ingresar_A_Un_binario(char* nombre_tabla, dato_t* dato_ingresar, int particion){

	char* pathTabla = obtenerPath_ParticionTabla(nombre_tabla, particion);


	FILE* archivo = fopen( pathTabla , "wb");

	fwrite(dato_ingresar , sizeof(dato_t) , 1 , archivo );

	fclose(archivo);
}


void crear_Binario_tabla(char* nombre_tabla , u_int16_t key , char* valor, time_t timestamp){
	dato_t datoAux;
	metadata_t metadata;

	datoAux.key = key;
	strcpy(datoAux.value, valor);
	datoAux.timestamp = timestamp;

	metadata = obtener_metadata(nombre_tabla);

	int particionObjetivo = calcular_particion(metadata.particion, key);

	char* pathTabla = obtenerPath_ParticionTabla(nombre_tabla, particionObjetivo);

	printf("El path destino es: %s\n", pathTabla);

	FILE *f = fopen(pathTabla, "wb");
	fwrite(&datoAux,sizeof(dato_t), 1, f);
	printf("ESCRITO CORRECTAMENTE\n");
	fclose(f);

}

void crear_archivos_particiones(char* nombre_tabla, int numero_particiones){

	FILE* archivo;

	for (int i = 0; i < numero_particiones ; i++ ){

		archivo = fopen(obtenerPath_ParticionTabla(nombre_tabla, i), "wb");

		fclose(archivo);

	}
}


dato_t* buscar_dato_en_binario(char* path_tabla, u_int16_t key){
	FILE *archivo;

	dato_t* datoAux = malloc(sizeof(dato_t));

	archivo = fopen(path_tabla, "rb");

	fread(datoAux, sizeof(dato_t), 1, archivo);
	while(!feof(archivo)){
		//tengo en cuenta que en las particiones hay keys diferentes
		if(datoAux->key == key){
			break;
		}
		fread(datoAux, sizeof(dato_t), 1, archivo);
	}

	fclose(archivo);

	return datoAux;
}

void llenarBloque(dato_t dato){
//	int indice = indicePrimerBloqueVacio();
	int indice = 2;
	crear_Binario_Bloque(indice, dato);
}

void crear_Binario_Bloque(int indice, dato_t dato){
	char valueAux[strlen(dato.value)];
	strcpy(valueAux, dato.value);
//	char* pathBloque = obtenerPath_Bloque(indice);
	char* pathHard = "2.bin";

	printf("El path es: %s\n", pathHard);

	mostrarDato(dato);
	printf("El valueAux es: %s\n", valueAux);

	FILE* f = fopen(pathHard, "wb");

	fwrite(&(dato.key), sizeof(dato.key), 1, f);
	fwrite(&(valueAux), sizeof(dato.value), 1, f);
	fwrite(&(dato.timestamp), sizeof(dato.timestamp), 1, f);

	fclose(f);
}

void mostrarDato(dato_t dato){
	printf("Key: %d\n", dato.key);
	printf("Value: %s\n", dato.value);
	printf("Timestamp: %d\n", dato.timestamp);
}


