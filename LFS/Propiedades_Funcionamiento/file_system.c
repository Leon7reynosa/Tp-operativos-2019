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

void crear_directorio(char* path){
	mkdir(path, 0700);
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
	particionContenido* base = malloc(sizeof(particionContenido));
	base->size = 0;
	base->bloques = malloc(sizeof(int));


	*(base->bloques) = buscarPrimerIndiceVacio();
	for (int i = 0; i < numero_particiones ; i++ ){

		archivo = fopen(obtenerPath_ParticionTabla(nombre_tabla, i), "wb");
		fwrite(&(base->size), sizeof(int), 1, archivo);
		fwrite(base->bloques, sizeof(int), 1, archivo);
		printf("Se le asigno el bloque %i a la particion %i\n",*(base->bloques) ,i);
		fclose(archivo);
		setEstado(*(base->bloques), OCUPADO);
		*(base->bloques) = buscarPrimerIndiceVacio();

	}

	free(base->bloques);
	free(base);
}

void leer_Particiones(char* nombreTabla){
	DIR* dir;
	struct dirent *ent;
	FILE* f;
	particionContenido auxiliar;
	auxiliar.bloques = malloc(sizeof(int));
	char* mapBloque;
	char* pathParaParticion;
	char* ubicacionesProhibidas[3];
	ubicacionesProhibidas[0] = ".";
	ubicacionesProhibidas[1] = "..";
	ubicacionesProhibidas[2] = "Metadata.config";
	int *fichero;
	struct stat *atributosBloque = malloc(sizeof(struct stat));
	int i = 0;
	int j = 0;

	if((dir = opendir(obtenerPathTabla(nombreTabla))) != NULL){
		while((ent = readdir(dir)) != NULL){
			if(strcmp(ent->d_name, ubicacionesProhibidas[0]) && strcmp(ent->d_name, ubicacionesProhibidas[1]) && strcmp(ent->d_name, ubicacionesProhibidas[2])){
				pathParaParticion = string_new();
				printf("Estamos en el archivo %s\n", ent->d_name);
				string_append(&pathParaParticion, obtenerPathTabla(nombreTabla));
				string_append(&pathParaParticion, "/");
				string_append(&pathParaParticion, ent->d_name);
				f = fopen(pathParaParticion, "rb");
				printf("%s\n" , ent->d_name);
				printf("hola beby\n");
				fread(&(auxiliar.size), sizeof(int), 1, f);
				fread(auxiliar.bloques, sizeof(int), 1, f);
				printf("Size = %i\n", auxiliar.size);
				printf("Bloques que contiene = %i\n ", *(auxiliar.bloques));
				while(*(auxiliar.bloques + i) != '\0'){
					printf("####Bloque %i\n####### ", *(auxiliar.bloques + i));
					mapBloque = obtenerMapDelBloque_ModoLectura(*(auxiliar.bloques + i), fichero, atributosBloque);
					for(j; j < atributosBloque->st_size; j++){
						printf("%c", *(mapBloque + j));
					}
					printf("\n");
					cerrarMapDelBloque(fichero, mapBloque, atributosBloque);
					i++;

				}
				printf("\n");

				free(pathParaParticion);
			}

		}
		closedir(dir);
	}
	else{
		perror("");
		exit(-1);
	}
}


dato_t* buscar_dato_en_particion(char* path_tabla, u_int16_t keyABuscar){
	FILE *archivo;
	dato_t *datoAuxiliar = malloc(sizeof(dato_t));;
	char* dato_A_Analizar, *c; //ver si hay que hacerle malloc
	int recorredorArray = 0;
	int contador_bloque = 0;
	int contador_particion = 0;
	char* keyEnString = string_itoa(keyABuscar); //ver si hay que hacer malloc
	struct stat *atributosBloque = malloc(sizeof(struct stat));
	int fichero;
	particionContenido* datoAux = malloc(sizeof(particionContenido));

	archivo = fopen(path_tabla, "rb");

	fread(datoAux, sizeof(dato_t), 1, archivo);

	char* mapBloque = obtenerMapDelBloque_ModoLectura(*(datoAux->bloques + recorredorArray), &fichero, atributosBloque);

	while( contador_bloque < atributosBloque->st_size && contador_particion < datoAux->size){

		if(*(mapBloque + contador_bloque) != '\n' ){

			c = *(mapBloque + contador_bloque);
			string_append(&dato_A_Analizar, c);
			contador_bloque++;
			contador_particion++;

		}else{

			if(string_contains(dato_A_Analizar, keyEnString)){

				sscanf(dato_A_Analizar , "%i;%i;%s" , &datoAuxiliar->timestamp ,  &datoAuxiliar->key, datoAuxiliar->value);

				fclose(archivo);

				return datoAuxiliar;
			}

			else{

				dato_A_Analizar = "";

				contador_bloque++;

				contador_particion++;
			}
		}

		if(contador_bloque == (atributosBloque->st_size)){

			cerrarMapDelBloque(fichero, mapBloque, atributosBloque);

			recorredorArray++;

			contador_bloque = 0;

			mapBloque = obtenerMapDelBloque_ModoLectura(*(datoAux->bloques + recorredorArray), &fichero, atributosBloque);

			break;

		}
	}

	fclose(archivo);

	return NULL;
}


char* obtenerMapDelBloque_ModoLectura(int indice, int* fichero, struct stat *atributosBloque){

	char *path = obtenerPath_Bloque(indice);
	int ficheroBloque = open(path, O_RDONLY, S_IRUSR);
	fstat(ficheroBloque, atributosBloque);
	char* mapBloque = mmap(NULL, atributosBloque->st_size, PROT_READ, MAP_SHARED, ficheroBloque, 0);

	return mapBloque;
}

void cerrarMapDelBloque(int fichero, char* map, struct stat *atributosBloque){
	munmap(map, atributosBloque->st_size);
	close(fichero);
}

void llenarBloque(char* dato, int indice){

	char** arrayValue = string_split(dato, ";");

	if((strlen(*(arrayValue) + 2)) > tamanio_value_max){
		//HACER LOG DE ERROR
		printf("Superas el tamanio maximo\n");
		exit(-1);
	}

	if(estaBloqueVacio(indice) || noExisteBloque(indice)){ //ver si anda bien
		crear_Binario_Bloque(indice, dato);
		printf("SE CARGO CORRECTAMENTE EL BLOQUE AL INDICE !!!\n");
		setEstado(indice, OCUPADO);
		printf("Estado del indice %i seteado a %i\n", indice, OCUPADO);
	}
	else{
		crear_Binario_Bloque(indice, dato);
	}


}

void crear_Binario_Bloque(int indice, char* dato){
	int caracteresTotales = strlen(dato);
	int recorridoDato = 0;
	char* pathBloque =  obtenerPath_Bloque(indice);

	int ficheroBloque = open(pathBloque, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

	int punteroBloque = 0;
	int indiceDatos = 0;

	printf("Buscando posicion de inicio de escritura...\n");
	punteroBloque = lseek(ficheroBloque, 0, SEEK_END);
	printf("Posicion encontrada !!! Puntero de bloque = %i\n", punteroBloque);
	printf("Tamanio del bloque = %i\n", block_size);

	for(caracteresTotales; caracteresTotales >= 0; caracteresTotales--){
		if(punteroBloque < block_size){
			write(ficheroBloque, (dato + indiceDatos), sizeof(char));
			*(dato + recorridoDato) = ' ';
			punteroBloque++;
			recorridoDato++;
			indiceDatos++;
		}

		else{
			printf("Entraste al else\n");
			string_trim_left(&dato); //Remueve todos los caracter vacios de la izquierda

			llenarBloque(dato, indice);
		}


	}

	printf("#########TERMINADO DE CARGAR#############\n");
	/*
	fclose(f);

	free(datoPasaje->value);
	free(datoPasaje);
	*/
	printf("Se cargo exitosamente !\n");
	close(ficheroBloque);
//	msync(archivoBloque, atributosBloque.st_size, MS_SYNC);
//	munmap(archivoBloque, atributosBloque.st_size);
}

void mostrarDato(dato_t* dato){
	printf("Key: %d\n", dato->key);
	printf("Timestamp: %d\n", dato->timestamp);
	printf("Value: %s\n", dato->value);

	int i = obtenerCantidadTotalDeBytes(dato);

	printf("Cantidad de bytes = %i\n", i);

}


void setEstado(int indiceBloque, estado estado){

	struct stat myStat;

	int fichero = open("Metadata/bitmap.bin", O_RDWR, S_IRUSR | S_IWUSR);

	fstat(fichero, &myStat);

	char* bitmap = mmap(0, myStat.st_size, PROT_WRITE, MAP_SHARED, fichero, 0);

	if(bitmap == MAP_FAILED){
		perror("mmap");
		close(fichero);
		exit(1);
	}
	t_bitarray* bitarray = bitarray_create_with_mode(bitmap, myStat.st_size, MSB_FIRST);
	if(estado == LIBRE){
		bitarray_clean_bit(bitarray, indiceBloque);
	}
	else{
		bitarray_set_bit(bitarray, indiceBloque);
	}


	munmap(bitmap, myStat.st_size);
	bitarray_destroy(bitarray);

	close(fichero);

}

void getAllEstados(){
	int fichero = open("Metadata/bitmap.bin", O_RDONLY, S_IRUSR);

	struct stat myStat;
	fstat(fichero, &myStat);

	char* bitmap = mmap(NULL, myStat.st_size, PROT_READ, MAP_SHARED, fichero, 0);
	t_bitarray* bitarray = bitarray_create_with_mode(bitmap, myStat.st_size, MSB_FIRST);

	int i;

	printf("######Lector de estados del bitmap######\n");
	for(i = 0; i < myStat.st_size * 8; i++){
		printf("Indice %i = %i\n", i, bitarray_test_bit(bitarray, i));
	}

	munmap(bitmap, myStat.st_size);
	bitarray_destroy(bitarray);

	close(fichero);
}

void setAllEstados(estado estado){
	int fichero = open("Metadata/bitmap.bin", O_RDWR, S_IRUSR | S_IWUSR);
	struct stat myStat;
	fstat(fichero, &myStat);

	char* bitmap = mmap(0, myStat.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fichero, 0);
	t_bitarray* bitarray = bitarray_create_with_mode(bitmap, myStat.st_size, MSB_FIRST);

	if(estado == LIBRE){
		for(int i = 0; i < myStat.st_size*8; i++){
			bitarray_clean_bit(bitarray, i);
		}
	}
	else{
		for(int i = 0; i < myStat.st_size*8; i++){
			bitarray_set_bit(bitarray, i);
		}
	}



	munmap(bitmap, myStat.st_size);
	bitarray_destroy(bitarray);
	close(fichero);
}


int buscarPrimerIndiceVacio(){
	int fichero = open("Metadata/bitmap.bin", O_RDONLY, S_IRUSR);
	struct stat myStat;

	fstat(fichero, &myStat);

	char* bitmap = mmap(NULL, myStat.st_size, PROT_READ, MAP_SHARED, fichero, 0);

	t_bitarray* bitarray = bitarray_create_with_mode(bitmap, myStat.st_size, MSB_FIRST);


	int i = 0;
	bool bit = bitarray_test_bit(bitarray, i);

	while(i < blocks && (bit == OCUPADO)){
		i++;
		bit = bitarray_test_bit(bitarray, i);
	}

	if(i == blocks){
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

int obtenerCantidadTotalDeBytes(dato_t* dato){
	int total = 0;
	char* stringKey, *stringTimestamp;

	stringKey = string_from_format("%i", dato->key);
	stringTimestamp = string_from_format("%i", dato->timestamp);

	total += strlen(stringKey);
	total += strlen(stringTimestamp);
	total += strlen(dato->value);
	total += 3; //; ; \n

	return total;
}

char* datoEnFormatoBloque(dato_t* dato){
	char* stringKey, *stringTimestamp;
	char* stringDato = string_new();

	stringKey = string_from_format("%i", dato->key);
	stringTimestamp = string_from_format("%i", dato->timestamp);

	string_append(&stringDato, stringTimestamp);
	string_append(&stringDato, ";");
	string_append(&stringDato, stringKey);
	string_append(&stringDato, ";");
	string_append(&stringDato, dato->value);
	string_append(&stringDato, "\n");

	return stringDato;
}

void leerBloque(int indice){
	printf("\n######Iniciando lectura del bloque %i#######\n", indice);
	char* pathBloque = malloc(strlen(obtenerPath_Bloque(indice)+1));
	pathBloque = obtenerPath_Bloque(indice);
	printf("PATTH : %s\n", pathBloque);
	int archivoBloque = open(pathBloque, O_RDONLY, S_IRUSR);

	int i = 0;
	char* auxiliar = malloc(100);
	read(archivoBloque,(auxiliar + i), sizeof(char));
	printf("%c", *(auxiliar + i));
	while(*(auxiliar + i) != '\0'){
		i++;
		read(archivoBloque,(auxiliar + i), sizeof(char));
		printf("%c", *(auxiliar + i));
	}
	//La fila vacia es por el \n
	//printf("%s\n", auxiliar);
	close(archivoBloque);
	free(auxiliar);
	free(pathBloque);

	printf("\n#####Finalizado lectura del Bloque %i ########\n", indice);
}

int estaBloqueVacio(int indice){
	char* path = obtenerPath_Bloque(indice);
	int ficheroBloque = open(path, O_RDONLY,  S_IRUSR);
	int posicion = lseek(ficheroBloque, 0, SEEK_END);

	if(posicion == 0){
		return EXIT_SUCCESS;
	}

	else{
		return EXIT_FAILURE;
	}
}

int noExisteBloque(int indice){
	char* path = obtenerPath_Bloque(indice);
	int ficheroBloque = open(path, O_RDONLY, S_IRUSR);

	if(ficheroBloque == -1){
		return EXIT_SUCCESS;
	}
	else{
		return EXIT_FAILURE;
	}

}


