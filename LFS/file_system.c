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
//	char* pathBloque = obtenerPath_Bloque(indice);
	char* pathHard = "2.bin";

	printf("El path es: %s\n", pathHard);
	printf("Datos previos a carga: \n");
	mostrarDato(dato);

	datoAux datoPasaje;
	datoPasaje.key = dato.key;
	strcpy(datoPasaje.value, dato.value);
	datoPasaje.timestamp = dato.timestamp;


	char valueAux[strlen(dato.value) + 1];

	FILE* f = fopen(pathHard, "wb");

	fwrite(&(dato.key), sizeof(dato.key), 1, f);
	fwrite(valueAux, strlen(dato.value) + 1 , 1, f);
	fwrite(&(dato.timestamp), sizeof(dato.timestamp), 1, f);

	fwrite(&(datoPasaje), sizeof(bloque_t), 1, f);

	printf("#########TERMINADO DE CARGAR#############\n");
	fclose(f);
//	setearEstado(2, OCUPADO);
}

void mostrarDato(dato_t dato){
	printf("Key: %d\n", dato.key);
	printf("Value: %s\n", dato.value);
	printf("Timestamp: %d\n", dato.timestamp);
}


void setearEstado(int indiceBloque, estado estado){

	struct stat myStat;

	int fichero = open("bitmap.bin", O_WRONLY, S_IRUSR | S_IWUSR);

	char* bitmap = mmap(0, myStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fichero, 0);

	if(bitmap == MAP_FAILED){
		perror("mmap");
		close(fichero);
		exit(1);
	}

	*(bitmap + indiceBloque) = estado;

	close(fichero);


}

void getEstado(){
	int fichero = open("./bitmap.bin", O_RDONLY, S_IRUSR);
	struct stat myStat;
	fstat(&myStat.st_size, fichero);

	char* bitmap = mmap(NULL, myStat.st_size, PROT_READ, MAP_SHARED, fichero, 0);

	for(int i = 0; i < 5192; i++){
		printf("Indice %i = %i\n", i, *(bitmap + i));
	}

	munmap(bitmap, myStat.st_size);

	close(fichero);
}


