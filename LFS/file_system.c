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

void llenarBloque(dato_t* dato){
	printf("hola :D \n");
	if(sizeof(dato) > tamanio_value_max){
		//HACER LOG DE ERROR
		printf("Superas el tamanio maximo\n");
		exit(-1);
	}
	printf("hola :D \n");
	int indice = buscarPrimerIndiceVacio();

	printf("INDICE ENCONTRADO !!!\n El indice libre es: %i\n", indice);
	crear_Binario_Bloque(indice, dato);
	printf("SE CARGO CORRECTAMENTE EL BLOQUE AL INDICE !!!\n");
	setEstado(indice, OCUPADO);
	printf("Estado del indice %i seteado a %i\n", indice, OCUPADO);
}

void crear_Binario_Bloque(int indice, dato_t* dato){
	char* pathBloque = string_new();
	pathBloque = obtenerPath_Bloque(indice);


	printf("El path es: %s\n", pathBloque);
	printf("Datos previos a carga: \n");
	mostrarDato(dato);

	dato_t* datoPasaje = malloc(sizeof(dato_t));
	datoPasaje->key = dato->key;
	datoPasaje->value = malloc(strlen(dato->value) + 1);
	memcpy(datoPasaje->value, dato->value, strlen(dato->value) + 1);
	datoPasaje->timestamp = dato->timestamp;

	FILE* f = fopen(pathBloque, "wb");

	fwrite(&(datoPasaje->key), sizeof(datoPasaje->key), 1, f);
	fwrite(&(datoPasaje->timestamp), sizeof(datoPasaje->timestamp), 1, f);
	fwrite(datoPasaje->value, strlen(datoPasaje->value) + 1 , 1, f);
	//fwrite(&(datoPasaje), sizeof(bloque_t), 1, f);

	printf("#########TERMINADO DE CARGAR#############\n");
	fclose(f);

	free(datoPasaje->value);
	free(datoPasaje);
	printf("Se cargo exitosamente !\n");
}

void mostrarDato(dato_t* dato){
	printf("Key: %d\n", dato->key);
	printf("Timestamp: %d\n", dato->timestamp);
	printf("Value: %s\n", dato->value);
}


void setEstado(int indiceBloque, estado estado){

	struct stat myStat;

	int fichero = open("bitmap.bin", O_RDWR, S_IRUSR | S_IWUSR);

	fstat(fichero, &myStat);

	char* bitmap = mmap(0, myStat.st_size, PROT_WRITE, MAP_SHARED, fichero, 0);

	if(bitmap == MAP_FAILED){
		perror("mmap");
		close(fichero);
		exit(1);
	}

	*(bitmap + indiceBloque) = estado;

	close(fichero);

	munmap(bitmap, myStat.st_size);

}

void getAllEstados(){
	int fichero = open("./bitmap.bin", O_RDONLY, S_IRUSR);
	struct stat myStat;
	fstat(fichero, &myStat);

	char* bitmap = mmap(NULL, myStat.st_size, PROT_READ, MAP_SHARED, fichero, 0);

	for(int i = 0; i < myStat.st_size; i++){
		printf("Indice %i = %i\n", i, *(bitmap + i));
	}

	munmap(bitmap, myStat.st_size);

	close(fichero);
}

void setAllEstados(estado estado){
	int fichero = open("./bitmap.bin", O_RDWR, S_IRUSR | S_IWUSR);
	struct stat myStat;
	fstat(fichero, &myStat);

	char* bitmap = mmap(0, myStat.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fichero, 0);

	for(int i = 0; i < myStat.st_size; i++){
		*(bitmap + i) = estado;
	}

	munmap(bitmap, myStat.st_size);

	close(fichero);
}


int buscarPrimerIndiceVacio(){
	int fichero = open("./bitmap.bin", O_RDONLY, S_IRUSR);
	struct stat myStat;

	fstat(fichero, &myStat);

	char* bitmap = mmap(NULL, myStat.st_size, PROT_READ, MAP_SHARED, fichero, 0);

	int i = 0;
	printf("%i\n", EOF);
	while(*(bitmap + i) != EOF){
		if(*(bitmap + i) == OCUPADO){
			i++;
			printf("%i", i);
		}
		else{
			printf("Dentro del else\n");
			break;
		}
	}

	if(*(bitmap) == EOF){
		printf("Esta todo lleno\n");
		exit(-1);
	}

	munmap(bitmap, myStat.st_size);
	return i;
}

void leerDatoDelBloque(int indice){
	char* path = obtenerPath_Bloque(indice);
	dato_t* datoObtenido = malloc(sizeof(dato_t));
	datoObtenido->value = malloc(51);
	FILE* f = fopen(path, "r");


	fread(&(datoObtenido->key), sizeof(datoObtenido->key), 1, f);
	fread(&(datoObtenido->timestamp), sizeof(datoObtenido->timestamp), 1, f);

	int i = 0;
	int ch;
	ch = fgetc(f);
	*(datoObtenido->value + i) = ch;
	i++;
	while(ch != '\0'){
		ch = fgetc(f);
		*(datoObtenido->value + i) = ch;
		i++;
	}


	datoObtenido->value = realloc(datoObtenido->value, i);


	printf("########## DATOS OBTENIDOS DEL BLOQUE %i ############\n", indice);
	mostrarDato(datoObtenido);

	free(datoObtenido->value);
	free(datoObtenido);

	fclose(f);
}




