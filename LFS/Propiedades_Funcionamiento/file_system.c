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

void asignarBloqueAParticion(char* nombreTabla, int particion, int bloque){
	FILE* archivo = fopen(obtenerPath_ParticionTabla(nombreTabla, particion), "wb+");
	particionContenido auxiliar;
	auxiliar.bloques = malloc(sizeof(int));
	int cantidadDeBloquesNumero = cantidadDeBloques(particion, nombreTabla);
	int contador = 0;

	fread(&(auxiliar.size), sizeof(int), 1, archivo);
	fread(auxiliar.bloques, sizeof(int), 1, archivo);

	printf("Hola :D\n");
	while(estaDentroDelRangoDeBloquesYEstaOcupado(*(auxiliar.bloques + contador))){
		printf("Bloque %i\n", *(auxiliar.bloques + contador));
		contador++;
	}

	contador = 0;

	while(estaDentroDelRangoDeBloquesYEstaOcupado(*(auxiliar.bloques + contador))){
		if(*(auxiliar.bloques + contador) == bloque){
			printf("Ya esta ese bloque aca !");
			return;
		}
			contador++;
	}

	auxiliar.size = cantidadDeBloquesNumero * block_size;
	*(auxiliar.bloques + contador) = bloque;

	fwrite(&(auxiliar.size), sizeof(int), 1, archivo);
	fwrite(auxiliar.bloques, sizeof(int), 1, archivo);

	fclose(archivo);

}

void leer_Particiones(char* nombreTabla){
	DIR* dir;
	struct dirent *ent;
	FILE* f;
	particionContenido auxiliar;
	auxiliar.bloques = malloc(sizeof(int));
	char* mapBloque;
	char* pathParaParticion;

	int *fichero = malloc(sizeof(int));
	struct stat *atributosBloque = malloc(sizeof(struct stat));
	int i = 0;
	int j = 0;

	if((dir = opendir(obtenerPathTabla(nombreTabla))) != NULL){
		while((ent = readdir(dir)) != NULL){
			if(noEsUnaUbicacionProhibida(ent->d_name)){
				pathParaParticion = string_new();
				printf("Estamos en el archivo %s\n", ent->d_name);
				string_append(&pathParaParticion, obtenerPathTabla(nombreTabla));
				string_append(&pathParaParticion, "/");
				string_append(&pathParaParticion, ent->d_name);
				f = fopen(pathParaParticion, "rb");
					fread(&(auxiliar.size), sizeof(int), 1, f);
					fread(auxiliar.bloques, sizeof(int), 1, f);
					printf("Size = %i\n", auxiliar.size);

					printf("Bloques que contiene = \n ");

					while(estaDentroDelRangoDeBloquesYEstaOcupado(*(auxiliar.bloques + i))){
						printf("####Bloque %i#######\n", *(auxiliar.bloques + i));
						mapBloque = obtenerMapDelBloque_ModoLectura(*(auxiliar.bloques + i), fichero, atributosBloque);
						if(mapBloque == NULL){
							printf("No hay nada todavia :(\n\n");
							close(*fichero);
						}
						else{
							for(j = 0; j < atributosBloque->st_size; j++){
								printf("%c", *(mapBloque + j));
							}
							printf("\n");
							cerrarMapDelBloque(fichero, mapBloque, atributosBloque);
						}
						i++;
					}
					i = 0;
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
	*fichero = open(path, O_RDONLY, S_IRUSR);
	if(*fichero < 0){
		return NULL;
	}

	fstat(*fichero, atributosBloque);
	char* mapBloque = mmap(NULL, atributosBloque->st_size, PROT_READ, MAP_SHARED, *fichero, 0);

	return mapBloque;
}

void cerrarMapDelBloque(int* fichero, char* map, struct stat *atributosBloque){
	munmap(map, atributosBloque->st_size);
	close(*fichero);
}

//usar con el ultimo bloque de la particion!
void llenarBloque(char* dato, int bloque){
	ubicadorBloque auxiliar;
	char** arrayValue = string_split(dato, ";");

	if((strlen(*(arrayValue) + 2)) > tamanio_value_max){
		//HACER LOG DE ERROR
		printf("Superas el tamanio maximo\n");
		return;
	}

	if(estaLleno(bloque)){
		printf("\n busco otro bloque para: %s\n", dato);
		int nuevoBloque = buscarPrimerIndiceVacio();
		//asignar_bloque(nuevoIndice)
		setEstado(nuevoBloque, OCUPADO);
		llenarBloque(dato, nuevoBloque);
//		actualizarParticion(nuevoBloque);

	}
	else{
		printf("\n dato a escribir : %s\n", dato);
		crear_Binario_Bloque(bloque, dato);
//		actualizarParticion(bloque);
	}
}

void crear_Binario_Bloque(int indice, char* dato){

	int caracteresTotales = strlen(dato);
	int recorridoDato = 0;
	char* pathBloque =  obtenerPath_Bloque(indice);

	int ficheroBloque = open(pathBloque, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	int punteroBloque;
	int indiceDatos = 0;

	printf("Buscando posicion de inicio de escritura...\n");
	//El seek_end nos sirve para continuar escribiendo, si es que  hay espacio todavia
	punteroBloque = lseek(ficheroBloque, 0, SEEK_END);
	printf("Posicion encontrada !!! Puntero de bloque = %i\n", punteroBloque);

	for(caracteresTotales; caracteresTotales > 0; caracteresTotales--){
		if(punteroBloque < block_size){
			write(ficheroBloque, (dato + indiceDatos), sizeof(char));
			*(dato + recorridoDato) = ' ';
			punteroBloque++;
			recorridoDato++;
			indiceDatos++;
		}
		else{
			printf("Entraste al else\n");
			printf("DATO: %s\n", dato);
			string_trim(&dato); //Remueve todos los caracter vacios de la izquierda
			printf("\n parte del dato a escribir en otro bloque: %s\n", dato);
			llenarBloque(dato, indice);
			break;
		}
	}

	printf("#########TERMINADO DE CARGAR#############\n");
	printf("Se cargo exitosamente !\n");
	close(ficheroBloque);
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
	for(i = 0; i < blocks; i++){
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

bool getEstado(int indice){
	int fichero = open("Metadata/bitmap.bin", O_RDONLY, S_IRUSR);

	struct stat myStat;
	fstat(fichero, &myStat);

	char* bitmap = mmap(NULL, myStat.st_size, PROT_READ, MAP_SHARED, fichero, 0);
	t_bitarray* bitarray = bitarray_create_with_mode(bitmap, myStat.st_size, MSB_FIRST);

	bool i = bitarray_test_bit(bitarray, indice);

	munmap(bitmap, myStat.st_size);

	bitarray_destroy(bitarray);

	close(fichero);

	return i;
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
	int bytes;
	char* pathBloque;
	pathBloque = obtenerPath_Bloque(indice);
	int archivoBloque = open(pathBloque, O_RDONLY, S_IRUSR);
	if(archivoBloque < 0){
		printf("\nNo existe el bloque %i\n", indice);
		return;
	}
	printf("######Iniciando lectura del bloque %i#######\n", indice);

	int i = 0;
	char* auxiliar = malloc(block_size);
//	read(archivoBloque,(auxiliar + i), sizeof(char));
//	printf("%c", *(auxiliar + i));
//	while(*(auxiliar + i) != '\0'){
//		i++;
//		read(archivoBloque,(auxiliar + i), sizeof(char));
//		printf("%c", *(auxiliar + i));
//	}

	while((bytes = read(archivoBloque,(auxiliar + i), sizeof(char)) ) > 0){
		printf("%c", *(auxiliar + i));
		i++;
	}

	//La fila vacia es por el \n
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
		printf("El bloque esta vacio\n");
		return EXIT_SUCCESS;
	}

	else{
		printf("El bloque no esta vacio\n");
		return EXIT_FAILURE;
	}
}

int noExisteBloque(int indice){
	char* path = obtenerPath_Bloque(indice);
	int ficheroBloque = open(path, O_RDONLY, S_IRUSR);

	if(ficheroBloque == -1){
		close(ficheroBloque);
		printf("No existe el bloque\n");
		return EXIT_SUCCESS;
	}
	else{
		close(ficheroBloque);
		printf("Existe el bloque\n");
		return EXIT_FAILURE;
	}



}

bool estaLleno(int indice){
	char* path = obtenerPath_Bloque(indice);
	int ficheroBloque = open(path, O_RDONLY, S_IRUSR);
	int puntero = lseek(ficheroBloque, 0, SEEK_END);
	close(ficheroBloque);

	return (puntero == (block_size));

}

dato_t* crearDato(u_int16_t key, time_t timestamp, char* value){
	dato_t* dato = malloc(sizeof(dato_t));
	dato->key = key;
	dato->timestamp = timestamp;
	dato->value = malloc(strlen(value)+1);
	memcpy(dato->value, value, strlen(value)+1);

	return dato;
}

FILE* crearTemporal(char* nombreTabla){
	FILE* archivo;
	particionContenido* base = malloc(sizeof(particionContenido));
	base->size = 0;
	base->bloques = malloc(sizeof(int));


	*(base->bloques) = buscarPrimerIndiceVacio();

	archivo = fopen(obtenerPathParaTemporalEnLaTabla(nombreTabla), "wb");
	fwrite(&(base->size), sizeof(int), 1, archivo);
	fwrite(base->bloques, sizeof(int), 1, archivo);
	printf("Se le asigno el bloque %i al temporal\n",*(base->bloques));
	//no se cierra el archivo
	setEstado(*(base->bloques), OCUPADO);

	free(base->bloques);
	free(base);

	return archivo;

}

void realizarDump(){

	t_list* dato_de_tabla;


	void _crear_temporal(char* tabla, void* elementoDeMemtable){

		dato_de_tabla = (t_list*) elementoDeMemtable;

		FILE* archivo_temporal;

		archivo_temporal = crearTemporal(tabla);

		llenarTemporal(archivo_temporal, dato_de_tabla);

		close(archivo_temporal);

//		list_destroy_and_destroy_elements(dato_de_tabla, eliminar_dato_t);

	}

	dictionary_iterator(memtable, _crear_temporal);

}

void llenarTemporal(FILE* temporal, t_list* dato_de_tabla){

	void _cargar_a_temporal(dato_t* dato){
		char* string_dato = datoEnFormatoBloque(dato);
		particionContenido auxiliar;
		int recorredorBloques = 0;

		fseek(temporal, 0, SEEK_SET);
		fread(&(auxiliar.size), sizeof(int), 1, temporal);
		fread(auxiliar.bloques, sizeof(int), 1, temporal);

		while(*(auxiliar.bloques + recorredorBloques) >= 0 && *(auxiliar.bloques + recorredorBloques) < 5192 && getEstado(*(auxiliar.bloques + recorredorBloques)) == 1){
			if(tamanioDelBloque(*(auxiliar.bloques + recorredorBloques)) == block_size){
				continue;
			}
			else{
				llenarBloque(string_dato, *(auxiliar.bloques + recorredorBloques));
			}
		}
	}

	list_iterate(dato_de_tabla, _cargar_a_temporal);


}

int tamanioDelBloque(int indice){
	char* path = obtenerPath_Bloque(indice);
	int descriptorBloque = open(path, O_RDONLY, S_IRUSR);

	int puntero = lseek(descriptorBloque, 0, SEEK_END);

	return puntero;
}

int cantidadDeBloques(int particion, char* nombreTabla){
	particionContenido auxiliar;
	auxiliar.bloques = malloc(sizeof(int));
	char* path = obtenerPath_ParticionTabla(nombreTabla, particion);
	FILE* archivoParticion = fopen(path, "rb");
	fread(&auxiliar.size, sizeof(int), 1, archivoParticion);
	fread(auxiliar.bloques, sizeof(int), 1, archivoParticion);

	int contador = 0;

	while(estaDentroDelRangoDeBloquesYEstaOcupado(*(auxiliar.bloques + contador))){
		contador++;
	}

	free(auxiliar.bloques);
	fclose(archivoParticion);
	return contador;

}

void actualizarParticion(int indice){
	printf("FUNCION ACTUALIZAR PARTICION\n");

	ubicadorBloque ubicador = ubicadorDelBloque(indice);
	printf("tabla = %s\n", ubicador.nombreTabla);
	printf("particion = %i\n", ubicador.particion);

	char* path = obtenerPath_ParticionTabla(ubicador.nombreTabla, ubicador.particion);
	printf("PATH = %s\n", path);

	FILE* archivoParticion = fopen(path, "wb+");

	particionContenido auxiliar;
	auxiliar.bloques = malloc(sizeof(int));

	int i = 0;

	printf("NANo?\n");

	fread(&auxiliar.size, sizeof(int), 1, archivoParticion);
	fread(auxiliar.bloques, sizeof(int), 1, archivoParticion);

	printf("NANI\n");

	int tamanioBloqueNuevo = tamanioDelBloque(indice);
	int cantidadDeBloquesNumero = cantidadDeBloques(ubicador.particion, ubicador.nombreTabla);

	printf("#####DATOS ANTERIORES#####\n");

	while(estaDentroDelRangoDeBloquesYEstaOcupado(*(auxiliar.bloques + i))){
		printf("bloque %i\n", *(auxiliar.bloques + i));
		i++;
	}

	printf("Size = %i\n", auxiliar.size);

	printf("/n################\n");
	i = 0;

	while(estaDentroDelRangoDeBloquesYEstaOcupado(*(auxiliar.bloques + i))){
		if(*(auxiliar.bloques + i) == indice){
			break;
		}
		else{
			i++;
		}
	}

	auxiliar.size = (cantidadDeBloquesNumero - 1) * block_size + tamanioBloqueNuevo;
	*(auxiliar.bloques + i) = indice;




	printf("\n#####DATOS NUEVOS#####\n");
	printf("Size = %i\n", auxiliar.size);
	i = 0;

	while(estaDentroDelRangoDeBloquesYEstaOcupado(*(auxiliar.bloques + i))){
		printf("bloque %i\n", *(auxiliar.bloques + i));
		i++;
	}

	fwrite(&auxiliar.size, sizeof(int), 1, archivoParticion);
	fwrite(auxiliar.bloques, sizeof(int), 1, archivoParticion);

	fclose(archivoParticion);
}

ubicadorBloque ubicadorDelBloque(int indice){
	DIR* dir, * dir2;
	struct dirent *ent, *ent2;
	FILE* f;
	particionContenido auxiliar;
	auxiliar.bloques = malloc(sizeof(int));
	ubicadorBloque ubicador;
	ubicador.nombreTabla = malloc(7);
	char *pathParaParticion;
/*
	int ficheroMap;
	ficheroMap = open("Metadata/bitmap.bin", O_RDONLY, S_IRUSR);
	struct stat atributosMap;
	fstat(ficheroMap, &atributosMap);

	char* bitmap = mmap(NULL, atributosMap.st_size, PROT_READ, MAP_SHARED, ficheroMap, 0);
	t_bitarray* bitarray = bitarray_create_with_mode(bitmap, atributosMap.st_size, MSB_FIRST);
*/
	int i = 0;

	if((dir = opendir(obtenerPathDirectorio_Tablas())) != NULL){
		while((ent = readdir(dir)) != NULL){

			if(noEsUnaUbicacionProhibida(ent->d_name)){
				pathParaParticion = string_new();

				string_append(&pathParaParticion, obtenerPathDirectorio_Tablas());
				string_append(&pathParaParticion, "/");
				string_append(&pathParaParticion, ent->d_name);
				dir2 = opendir(pathParaParticion);
				while((ent2 = readdir(dir2)) != NULL){

					if(noEsUnaUbicacionProhibida(ent2->d_name)){

						pathParaParticion = string_new();

						string_append(&pathParaParticion, obtenerPathDirectorio_Tablas());
						string_append(&pathParaParticion, "/");
						string_append(&pathParaParticion, ent->d_name);
						string_append(&pathParaParticion, "/");
						string_append(&pathParaParticion, ent2->d_name);

						f = fopen(pathParaParticion, "rb");

						fread(&auxiliar.size, sizeof(int), 1, f);
						fread(auxiliar.bloques, sizeof(int), 1, f);

						while(estaDentroDelRangoDeBloquesYEstaOcupado(*(auxiliar.bloques + i))){
							if(*(auxiliar.bloques + i) == indice){
								ubicador.nombreTabla = ent->d_name;
								ubicador.particion = obtenerNumeroParticion(ent2->d_name);

								return ubicador;

							}
							i++;
						}
						i = 0;
						fclose(f);
					}
				}
				closedir(dir2);
			}
		}
		closedir(dir);
	}
	else{
		perror("");
		exit(-1);
	}
}

int obtenerNumeroParticion(char* path){
	int numero;
	char** aux;
	char delimitador[2] = ".";

	if(string_equals_ignore_case("bin", extensionDelArchivo(path))){
		aux = string_split(path, delimitador);
		numero = atoi(aux[0]);

		return numero;
	}
	else{
		return -1;
	}

}

int obtenerNumeroTemporal(char* path){
	int numero;
	char** aux;
	char* numeroString;
	char delimitador[2] = ".";

	if(string_equals_ignore_case("tmp", extensionDelArchivo(path))){
		aux = string_split(path, delimitador);
		numeroString = string_substring_from(aux[0], 1);
		numero = atoi(numeroString);
		return numero;
	}
	else{
		return -1;
	}
}

char* extensionDelArchivo(char* path){

	char* delimitador = ".";
	char** aux;
	aux = string_split(path, delimitador);
	return aux[1];
}

bool noEsUnaUbicacionProhibida(char* path){
	char *ubicacionesProhibidas[3];

	ubicacionesProhibidas[0] = ".";
	ubicacionesProhibidas[1] = "..";
	ubicacionesProhibidas[2] = "Metadata.config";

	if(strcmp(path, ubicacionesProhibidas[0]) && strcmp(path, ubicacionesProhibidas[1]) && strcmp(path, ubicacionesProhibidas[2])){
		return true;
	}
	else{
		return false;
	}
}

bool estaDentroDelRangoDeBloquesYEstaOcupado(int bloque){
	if(bloque >= 0 && bloque < blocks && getEstado(bloque) == OCUPADO){
		return true;
	}
	else{
		return false;
	}
}
