/*
 * API_Lissandra.c
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */

#include "API_Lissandra.h"

//la hago global aca por ahora, despues vemos como usarla
tabla_memtable *memtable;


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

void insert(char* nombre_tabla , int key , char* valor, time_t timestamp ){

	dato_t *auxiliar;

	int particiones_metadata, tiempo_compactacion;
	char* consistencia;

	if(existe_la_tabla(nombre_tabla)){

		printf("SI EXISTE LA TABLA :D \n\n");
		obtener_metadata (&consistencia , &particiones_metadata, &tiempo_compactacion);

		auxiliar = crear_dato(key, valor, timestamp);

		printf("dato_key: %d\n" , auxiliar->key);
		printf("dato_valor: %s\n\n" , auxiliar->value);

		ingresar_a_memtable(auxiliar, nombre_tabla);

	}else{

		printf("FALLO EL INSERT :D\n");

		//exit(-1);
	}

}


void ingresar_a_memtable(dato_t *dato_ingresar, char* tabla_ingresar){

	bloque_tabla* lista_tabla = existe_en_memtable(tabla_ingresar);

	if(lista_tabla == NULL){
		printf("no existis todavia bro \n\n");
		//le tengo que pasar el lista_tabla?
		//lista_tabla = crear_tabla(tabla_ingresar, dato_ingresar);
	}

	lista_tabla->dato_sig = NULL;
	lista_tabla->dato_t = dato_ingresar;


	//ahi quedaria almacenado en la ultima posicion de memoria
}


/*##########################
bloque_tabla* crear_tabla(char* tabla, dato_t* dato){


	//HAY QUE HACER ESTO
	//me tiene que retornar el bloque de memoria de la tabla donde voy
	// a ingresar el dato (tratemos de que sea en la ultima posicion de memoria
}
############################3*/



bloque_tabla* existe_en_memtable(char* tabla){

	tabla_memtable* aux = memtable->sig_tabla;
	tabla_memtable* siguiente;
	bloque_tabla* encontrado;
	bloque_tabla* aux_bloque;

	if(aux == NULL){

		aux->nombre_tabla = tabla;
		aux->primer_bloque = encontrado;
		aux->sig_tabla = NULL;

		memtable->sig_tabla = aux;
	}



		while( aux != NULL ){

			if(strcmp(aux->nombre_tabla , tabla)){
				//si lo encuentra una tabla que sea con el mismo nombre, asiga la lista a encontrado
				encontrado = aux->primer_bloque;

				encontrado = ultima_posicion(aux->primer_bloque);

				return encontrado;
			}else{
				siguiente = aux->sig_tabla;
				aux = siguiente;
			}
		}

		//si no la encuentra devuelve NULL
		return encontrado;
}

bloque_tabla* ultima_posicion(bloque_tabla* primer_posicion){

	//aca hay que hacer que nos devuelva la ultima posicion donde vamos a almacenar
	bloque_tabla* ultima;

	ultima = primer_posicion->dato_sig;
	while(ultima->dato_sig != NULL){

		ultima = ultima->dato_sig;
	}

	return ultima;
}


//crea un nuevo dato del tipo dato_t
dato_t* crear_dato(int clave, char* valor, time_t tiempo){

	dato_t* nuevo = (dato_t*) malloc(sizeof(dato_t));

	nuevo->key = clave;
	nuevo->value = valor;
	nuevo->timestamp = tiempo;

	return nuevo;
}




//Crea una nueva tabla que va a ser parte de la lista
bloque_tabla* crear_bloque_tabla(dato_t dato_insertar ){

	bloque_tabla* nueva_tabla;

	nueva_tabla = (bloque_tabla* ) malloc(sizeof(bloque_tabla));

	nueva_tabla->dato_t = dato_insertar;
	nueva_tabla->dato_sig = NULL;

	return nueva_tabla;
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




int calcular_particion( int particion_metadata ,int key){
	return key % particion_metadata;
}




void obtener_metadata(char** consistencia , int* particion, int* tiempo_compactacion){

	t_config* metadata_config;

	metadata_config = config_create("Tablas/Tabla_A/Metadata.config");

	*consistencia = config_get_string_value(metadata_config,"CONSISTENCY");

	*particion = config_get_int_value(metadata_config, "PARTITIONS");

	*tiempo_compactacion = config_get_int_value(metadata_config, "COMPACTION_TIME");
}



void setear_metadata(){

	printf("HOLA");

	t_config* metadata_config;

	metadata_config = config_create("Tablas/Tabla_A/Metadata.config");

	config_set_value(metadata_config, "CONSISTENCY", "SC");
	config_set_value(metadata_config, "PARTITIONS", "1");
	config_set_value(metadata_config, "COMPACTION_TIME" , "300");
	config_save(metadata_config);
	config_destroy(metadata_config);

	printf("se configuro el metadata lol\n");
}





