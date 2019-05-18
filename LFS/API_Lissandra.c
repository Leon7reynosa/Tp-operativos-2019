/*
 * API_Lissandra.c
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */

#include "API_Lissandra.h"


//la hago global aca por ahora, despues vemos como usarla
tabla_memtable* memtable = NULL;

tabla_memtable* conseguirMemtable(){
	return memtable;
}

void realizar_select(char* nombre_tabla , int key ){

	int particion_objetivo; //en que particion de la tabla tengo que buscar
	int particiones_metadata, tiempo_compactacion;
	char* consistencia;

	if(existe_la_tabla(nombre_tabla)){

		printf("SI EXISTE YUPII :D\n\n");

		obtener_metadata (&consistencia , &particiones_metadata, &tiempo_compactacion);

		printf("CONSISTENCY = %s\n", consistencia);
		printf("PARTICIONES = %d\n" , particiones_metadata);
		printf("TIEMPO DE COMPACTACION = %d\n" , tiempo_compactacion);

		particion_objetivo = calcular_particion(particiones_metadata , key);

		printf("PARTICION OBJETIVO = %d\n\n" , particion_objetivo);


	}else{

		printf("FALLOO NOOO D:\n");

	}


}

void create(char* nombre_tabla, char* criterio, int numero_Particiones, int tiempo_Compactacion){
	if(existe_la_tabla(nombre_tabla)){
		//Se guardará el resultado en un .log y se retorna error con dicho resultado
	}
	mkdir(nombre_tabla, 0700); //Anda
//	crear_metadata(criterio, numero_Particiones, tiempo_Compactacion); //Arreglar esto
	//Crear binario con respecto a las particiones y agregar un bloque a cada uno
}

void describe(){
	struct dirent* de;
	DIR* dr = opendir("Tablas");
	if(dr == NULL){
		printf("No se pudo abrir el directorio mencionado\n");
	}

	while((de = readdir(dr)) != NULL){
		printf("%s\n", de->d_name);
//		obtener_metadata_de_tabla(de->d_name);
	}

	closedir(dr);
}

int existe_la_tabla(char* tabla){
	DIR* dir = opendir(tabla);
	if(dir){
		return dir;
	}else {
		printf("El directorio no existe");
		return dir;
	}
	closedir(dir);
}
/*
void obtener_metadata_de_tabla(char* nombre_tabla){
	char prefijo[7] = "Tablas/";
	char path[14];

	t_config* metadata = config_create()

}
*/


//tabla 1   tabla 2   ...
//bloques n bloques n ..

//memtable
void insert(char* nombre_tabla , int key , char* valor, time_t timestamp ){

	dato_t* dato_ingresar;
	bloque_tabla* mostrar_bloque;
	dato_t* mostrar_dato;

	int particiones_metadata, tiempo_compactacion;
	char* consistencia;

	if(existe_la_tabla(nombre_tabla)){

		printf("SI EXISTE LA TABLA :D \n\n");
		obtener_metadata (&consistencia , &particiones_metadata, &tiempo_compactacion);

		dato_ingresar = crear_dato(key, valor, timestamp);

		printf("dato_key: %d\n" , dato_ingresar->key);
		printf("dato_valor: %s\n\n" , dato_ingresar->value);

		ingresar_a_memtable(dato_ingresar, nombre_tabla);
		printf("FUNCIONO CORRECTAMENTE\n\n");

		mostrar_bloque = memtable->primer_bloque;

		printf("Se creo que bloque con key: %s\n" , memtable->primer_bloque->dato_t->value);

		/*tabla_memtable* aux = memtable;
		bloque_tabla* auxbloque = aux->primer_bloque;
		while(aux != NULL){
			printf("Nombre tabla: %s \n", aux->nombre_tabla);
			while(auxbloque != NULL){
				printf("Key: %d \n", (aux->primer_bloque->dato_t).key);
				auxbloque = aux->primer_bloque->dato_sig;
			}
			printf("\n\n");
			aux = aux->sig_tabla;
			auxbloque= aux->primer_bloque;
		}*/

	}else{

		printf("FALLO EL INSERT :D\n");
		//exit(-1);
	}



}


void ingresar_a_memtable(dato_t* dato_ingresar, char* nombre_tabla){
	allocar_memoria(nombre_tabla, dato_ingresar);

}

void allocar_memoria(char* nombre_tabla , dato_t* dato_ingresar){

	bloque_tabla* nuevo_bloque;
	nuevo_bloque = crear_bloque(dato_ingresar);

	if(memtable == NULL){
		memtable = malloc(sizeof(tabla_memtable));
		memtable->nombre_tabla = nombre_tabla;
		memtable->primer_bloque = nuevo_bloque;
		memtable->sig_tabla = NULL;
	}else if(existe_en_memtable(nombre_tabla)){
		printf("entro en el else if\n");
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
		printf("entro en el while 1");
		aux = aux->sig_tabla;
	}
	printf("mitad\n");
	bloque_tabla* ultimo_bloque = aux->primer_bloque;
	printf("algo del bloque: %d \n", ultimo_bloque->dato_t->key);

	while(ultimo_bloque->dato_sig != NULL){
		printf("entro en el while 2");
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


void obtener_metadata(char** consistencia , int* particion, int* tiempo_compactacion){

	t_config* metadata_config;

	metadata_config = config_create("Tablas/Tabla_A/Metadata.config");

	*consistencia = config_get_string_value(metadata_config,"CONSISTENCY");

	*particion = config_get_int_value(metadata_config, "PARTITIONS");

	*tiempo_compactacion = config_get_int_value(metadata_config, "COMPACTION_TIME");
}
/*
void crear_metadata(char* consistencia, int particion, int tiempo_Compactacion){
	t_config* metadata_config;
	char laconchadetumadreleon[20];
	FILE *aux = fopen("Tablas/Tabla_B/Metadata.config", "w");
	fclose(aux);
	metadata_config = config_create("Tablas/Tabla_B/Metadata.config");
	config_set_value(metadata_config,"CONSISTENCY", consistencia);

	itoa(particion,laconchadetumadreleon,10);

	config_set_value(metadata_config, "PARTITIONS", laconchadetumadreleon);

	itoa(tiempo_Compactacion,laconchadetumadreleon,10);

	config_set_value(metadata_config, "COMPACTION_TIME", laconchadetumadreleon);
	config_save(metadata_config);
	config_destroy(metadata_config);

}
*/


dato_t* crear_dato(int clave, char* valor, time_t tiempo){

	dato_t* nuevo = (dato_t*) malloc(sizeof(dato_t));

	nuevo->key = clave;
	nuevo->value = valor;
	nuevo->timestamp = tiempo;

	return nuevo;
}





void crear_Binario(char* nombre_tabla , int key , char* valor, time_t timestamp/*, int particion*/){
	dato_t datoAux;
	datoAux.key = key;
	datoAux.value = valor;
	datoAux.timestamp = timestamp;

	FILE *f = fopen("Tablas/Tabla_A/particion.bin", "wb");
	fwrite(&datoAux,sizeof(dato_t), 1, f);
	printf("Se guardo con exito (creo xD)\n");
	fclose(f);
	printf("Se cerro el archivo\n\n");
}
/*
void verificar_Binario(){ //ANDA :D
	dato_t datoAux;
	FILE *f = fopen("Tablas/Tabla_A/particion.bin", "rb");
	fread(&datoAux, sizeof(dato_t), 1, f);
	printf("Binario leido con exito\n--Resultados--\n");
	printf("Key leida: %i\n", datoAux.key);
	printf("Value leido: %s\n", datoAux.value);
	printf("Timestamp leido: %i\n", datoAux.timestamp);
	fclose(f);
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



//Crea una nueva tabla que va a ser parte de la lista
bloque_tabla* crear_bloque_tabla(dato_t dato_insertar ){

	bloque_tabla* nueva_tabla;

	nueva_tabla = (bloque_tabla* ) malloc(sizeof(bloque_tabla));

	nueva_tabla->dato_t = dato_insertar;
	nueva_tabla->dato_sig = NULL;

	return nueva_tabla;
}
*/


int calcular_particion( int particion_metadata ,int key){
	return key % particion_metadata;
}



void setear_metadata(){

	printf("HOLA");

	t_config* metadata_config;

	metadata_config = config_create("Tablas/Tabla_B/Metadata.config");

	config_set_value(metadata_config, "CONSISTENCY", "SC");
	config_set_value(metadata_config, "PARTITIONS", "1");
	config_set_value(metadata_config, "COMPACTION_TIME" , "300");
	config_save(metadata_config);
	config_destroy(metadata_config);

	printf("se configuro el metadata lol\n");
}




