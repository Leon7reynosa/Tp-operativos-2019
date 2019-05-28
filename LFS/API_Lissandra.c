/*
 * API_Lissandra.c
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */

#include "API_Lissandra.h"

tabla_memtable* memtable = NULL;

tabla_memtable* conseguirMemtable(){
	return memtable;
}

////////////////////////////////////////funciones de request////////////////////////////////////////////////

dato_t* realizar_select(char* nombre_tabla , int key ){

	metadata_t metadata;
	char* consistencia;
	dato_t* datoMasReciente;
	dato_t*dato_auxiliar;
	char* path_tabla;

	if(existe_la_tabla(nombre_tabla)){

		metadata = obtener_metadata(nombre_tabla);

		printf("Consistency: %i\n", metadata.consistencia);
		printf("Particiones: %d\n" , metadata.particion);
		printf("Tiempo de compactacion: %d\n" , metadata.compactacion);

		int particion_objetivo = calcular_particion(metadata.particion , key);

		path_tabla = obtenerPath_ParticionTabla(nombre_tabla , particion_objetivo);

		printf("Particion objetivo: %d\n\n" , particion_objetivo);

		datoMasReciente = buscar_dato_en_binario(path_tabla ,key);

		/*
		dato_auxiliar = buscar_dato_en_memtable(key);

		if(datoMasReciente->timestamp > dato_auxiliar->timestamp){
			//con este if comparamos cual tiene el timestamp mas grande
			datoMasReciente = dato_auxiliar;

		}
	*/




		/*
		 * Falta buscar en la memtable. y encontrar el que tenga el timestamp mas grande
		 * y en los temporales
		 */

		printf("\n\nValor encontrado: %s\n\n" , datoMasReciente->value );
		printf("\n\nKey encontrado: %i\n\n" , datoMasReciente->key );

	}else{
		printf("No existe la tabla\n");
	}

	free(path_tabla);

	return datoMasReciente;
}

/*
dato_t* buscar_dato_en_memtable(int key){


}
*/

void create(char* nombre_tabla, char* criterio, int numero_Particiones, int tiempo_Compactacion){
	if(existe_la_tabla(nombre_tabla)){
		printf("Ya existe la tabla \n");
		exit(1);
		//Se guardará el resultado en un .log y se retorna error con dicho resultado
	}
	char* pathDeTabla = obtenerPathTabla(nombre_tabla);
	mkdir(pathDeTabla, 0700); //Anda
	crear_metadata(nombre_tabla, criterio, numero_Particiones, tiempo_Compactacion); //Arreglar esto
	//Crear binario con respecto a las particiones y agregar un bloque a cada uno
}

void describe(){
	struct dirent* de;
	DIR* dr = opendir("Tablas");
	if(dr == NULL){
		printf("No se pudo abrir el directorio mencionado\n");
	}

	while((de = readdir(dr)) != NULL){
		if(de->d_name[0] != '.'){
			//puts(de->d_name);
			printf("%s\n", de->d_name);
			mostrar_metadata_de_tabla(de->d_name);
		}
	}
	closedir(dr);
}

void describe_especifico(char* nombre_tabla){
	DIR* dr = opendir("Tablas");
	printf("---------- Tabla:  %s -----------\n" , nombre_tabla);
	if(existe_la_tabla(nombre_tabla)){
		mostrar_metadata_de_tabla(nombre_tabla);
	}
}


void insert(char* nombre_tabla , int key , char* valor, time_t timestamp ){

	dato_t* dato_ingresar;
	bloque_tabla* mostrar_bloque;
	dato_t* mostrar_dato;
	metadata_t metadata_aux;

	int particiones_metadata, tiempo_compactacion;
	char* consistencia;

	if(existe_la_tabla(nombre_tabla)){

		printf("Existe la tabla. \n");
		metadata_aux = obtener_metadata(nombre_tabla);

		dato_ingresar = crear_dato(key, valor, timestamp);

		ingresar_a_memtable(dato_ingresar, nombre_tabla);

		mostrar_bloque = memtable->primer_bloque;

		printf("Se creo que bloque con key: %s\n" , memtable->primer_bloque->dato_t->value);

		/*tabla_memtable* aux = memtable;
		bloque_tabla* auxbloque = aux->primer_bloque;
		while(aux != NULL){
			printf("Nombre tabla: %s \n", aux->nombre_tabla);
			while(auxbloque != NULL){
				printf("Key: %d \n", (aux->primer_bloque->dato_t)->key);
				auxbloque = aux->primer_bloque->dato_sig;
			}
			printf("\n\n");
			aux = aux->sig_tabla;
			auxbloque= aux->primer_bloque;
		}*/

	}else{
		printf("Fallo el inser.\n");
		//exit(-1);
	}
}

////////////////////////////////////////funciones extras////////////////////////////////////////////////

int existe_la_tabla(char* tabla){

	char* path_tabla = obtenerPathTabla(tabla);

	DIR* dir = opendir(path_tabla);
	if(dir){
		return 1;
	}else {
		printf("El directorio no existe\n");
	}
	closedir(dir);
	return 0;
}


void ingresar_A_Un_binario(char* nombre_tabla, int key){


	FILE* archivo = fopen( "Tablas/Tabla_A/0.bin" , "wb");

	dato_t* dato_ingresar = crear_dato( key, "hakuna matata", 100);

	fwrite(dato_ingresar , sizeof(dato_t) , 1 , archivo );

	fclose(archivo);
}

void ingresar_a_memtable(dato_t* dato_ingresar, char* nombre_tabla){

	bloque_tabla* nuevo_bloque;
	nuevo_bloque = crear_bloque(dato_ingresar);

	if(memtable == NULL){
		memtable = malloc(sizeof(tabla_memtable));
		memtable->nombre_tabla = nombre_tabla;
		memtable->primer_bloque = nuevo_bloque;
		memtable->sig_tabla = NULL;
	}else if(existe_en_memtable(nombre_tabla)){
		poner_bloque_en_tabla(nombre_tabla, nuevo_bloque);

	}else {
		crear_tabla_en_memtable(nombre_tabla);
		poner_bloque_en_tabla(nombre_tabla, nuevo_bloque);
	}

}


void crear_tabla_en_memtable(char* nombre_tabla){

	tabla_memtable* aux;
	aux = memtable;
	tabla_memtable* aux2 = malloc(sizeof(tabla_memtable));

	while(aux->sig_tabla != NULL){
		aux = aux->sig_tabla;
	}
	aux->sig_tabla = aux2;
	aux2->nombre_tabla = nombre_tabla;
	aux2->primer_bloque = NULL;
	aux2->sig_tabla = NULL;

}

void poner_bloque_en_tabla(char* nombre_tabla , bloque_tabla* bloque_ingresar){

	tabla_memtable* aux = memtable;
	printf("nombre memtable: %s\n",aux->nombre_tabla);
	printf("nombre memtable: %s\n",nombre_tabla);

	while(strcmp(aux->nombre_tabla , nombre_tabla) != 0){
		aux = aux->sig_tabla;
	}
	bloque_tabla* ultimo_bloque = aux->primer_bloque;
	printf("algo del bloque: %d \n", ultimo_bloque->dato_t->key);

	while(ultimo_bloque->dato_sig != NULL){
		ultimo_bloque = ultimo_bloque->dato_sig;
	}
	ultimo_bloque->dato_sig = bloque_ingresar;
}



bloque_tabla* crear_bloque(dato_t* dato_ingresar){
	bloque_tabla* nuevo_bloque = malloc(sizeof(bloque_tabla));

	printf("Se creo el bloque con dato: %d\n", dato_ingresar->key);

	nuevo_bloque->dato_sig = NULL;
	nuevo_bloque->dato_t = dato_ingresar;


	return nuevo_bloque;
}

int existe_en_memtable(char* nombre_tabla){

	tabla_memtable* aux;

	aux = memtable;
	while(aux!=NULL){
		if(strcmp(aux->nombre_tabla , nombre_tabla) == 0){
			return 1;
		}
		else{
			aux = aux->sig_tabla;
		}
	}
	return 0;
}


dato_t* crear_dato(int clave, char* valor, time_t tiempo){

	dato_t* nuevo = (dato_t*) malloc(sizeof(dato_t));

	nuevo->key = clave;
	strcpy(nuevo->value, valor);
	nuevo->timestamp = tiempo;

	return nuevo;
}


void crear_Binario(char* nombre_tabla , int key , char* valor, time_t timestamp, int particion){
	dato_t datoAux;
	datoAux.key = key;
	strcpy(datoAux.value, valor);
	datoAux.timestamp = timestamp;
	char* pathTabla = obtenerPath_ParticionTabla(nombre_tabla, particion);

	FILE *f = fopen(pathTabla, "wb");
	fwrite(&datoAux,sizeof(dato_t), 1, f);
	printf("Se guardo con exito (creo xD)\n");
	fclose(f);
	printf("Se cerro el archivo\n\n");
}

dato_t* buscar_dato_en_binario(char* path_tabla, int key){
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


void verificar_Binario(){ //ANDA :D
	dato_t datoAux2;
	FILE *f = fopen("Tablas/Tabla_A/particion.bin", "rb");

	fread(&datoAux2, sizeof(dato_t), 1, f);

	while(!feof(f)) {

		printf("Binario leido con exito\n--Resultados--\n");
		printf("Key leida: %i\n", datoAux2.key);
		printf("Value leido: %s\n", datoAux2.value);
		printf("Timestamp leido: %i\n", datoAux2.timestamp);

		fread(&datoAux2, sizeof(dato_t), 1, f);
	}
	fclose(f);
}
/*
char* obtenerPathTabla(char* nombre_Tabla){
	char aux_nombreTabla[strlen(nombre_Tabla) + 1];
	aux_nombreTabla[strlen(nombre_Tabla) + 1] = '\0';
	char prefijo[] = "Tablas/";
	char extension[] = "/";

	int cantidad = strlen(nombre_Tabla) + strlen(prefijo) + strlen(extension) + 1;

	char aux_path[cantidad];
	char* path = malloc(cantidad);

	strcpy(aux_nombreTabla, nombre_Tabla);
	//printf("1 %s\n",aux_path);

	strcpy(aux_path, prefijo);
	printf("2 %s\n",aux_path);

	strcat(aux_path, aux_nombreTabla);
	printf("3 %s\n",aux_path);



	strcat(aux_path, extension);
	printf("4 %s\n",aux_path);

	strcpy(path,aux_path);
	printf("? %s\n", path);
	return path;


}
*/


/*##################################################################33
void ingresar_a_memtable(dato_t *dato_ingresar, char* tabla_ingresar){


	tabla_memtable* tabla = encontrar_memtable(tabla_ingresar);
	bloque_tabla* bloque_nuevo = malloc(sizeof(bloque_tabla));

	bloque_nuevo->dato_sig = NULL;
	bloque_nuevo->dato_t = *dato_ingresar;

	ingresar_bloque( tabla , bloque_nuevo);

	printf("se ingreso un bloque bro\n");

}






void ingresar_bloque(tabla_memtable* tabla, bloque_tabla* bloque){

	bloque_tabla* aux ;
	bloque_tabla* posicion_previa;
	printf("chino cochino\n");
	aux = tabla->primer_bloque;

	if(aux == NULL){
		tabla->primer_bloque = bloque;
		return;
	}

	posicion_previa = aux;

	while(aux != NULL){
	//	printf("lalo malalo\n");
		posicion_previa = aux;
		aux = aux->dato_sig;
	}

	posicion_previa->dato_sig = bloque;

	printf("funciono lol\n");
return;
}



//nos crea una tabla y la pone al final de la memtable, nos devuelve el puntero a esa tabla
tabla_memtable* crear_tabla(char* tabla){

	tabla_memtable* ultima_posicion;
	tabla_memtable* nueva_tabla =  malloc(sizeof(tabla_memtable));

	ultima_posicion = ultima_posicion_memtable();

	nueva_tabla->nombre_tabla = tabla;
	nueva_tabla->primer_bloque = NULL;
	nueva_tabla->sig_tabla = NULL;

	ultima_posicion->sig_tabla = nueva_tabla;

	return nueva_tabla;
}




tabla_memtable* ultima_posicion_memtable(){

	tabla_memtable* aux;
	tabla_memtable* ultima_posicion;

	//inicializacion
	if(memtable == NULL){
		memtable = malloc(sizeof(tabla_memtable));
		return memtable;
	}

	aux = memtable;
	ultima_posicion = aux;

	while(aux !=  NULL){
		ultima_posicion = aux;
		aux = aux->sig_tabla;
		printf("alan \n");
	}
	printf("no entre al while hdps\n");
	return ultima_posicion;
}



//nos devuelve null si no existe la tabla en la memtable
tabla_memtable* encontrar_memtable(char* tabla){

	tabla_memtable* aux;

	aux = memtable;

	while( aux != NULL ){
		if(strcmp(aux->nombre_tabla , tabla)){
				printf("se encontroooo la memtable\n\n");
				return aux;
		}else{
			aux = aux->sig_tabla;
		}
	}

	printf("No se encontro la tabla\n");
	aux = crear_tabla(tabla);

	return aux;
}

*/


int calcular_particion( int particion_metadata ,int key){
	return key % particion_metadata;
}








