
/*
 * API_Lissandra.h
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */

#ifndef API_LISSANDRA_H_
#define API_LISSANDRA_H_



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<time.h>
#include<commons/config.h>
#include<string.h>
#include<sys/stat.h>
#include"Metadata/metadata.h"
#include"Mensajeria/recibir.h"
#include"Mensajeria/mensajes.h"

/*
#define TAMANIO_MAX_VALUE 50


typedef struct{

	int key;
	char value[TAMANIO_MAX_VALUE];
	time_t timestamp;

}dato_t;
*/

typedef struct{

	dato_t* dato_t;
	struct bloque_tabla* dato_sig;

}bloque_tabla;

typedef struct{

	char* nombre_tabla; //uso un string para ver el nombre de la tabla, si quieren despues cambienlo
	bloque_tabla* primer_bloque;
	struct tabla_memtable* sig_tabla;

}tabla_memtable;

//////////////////////Request del Lissandra/////////////////////////////////////////

tabla_memtable* conseguirMemtable();
void insert(char* ,int , char*,time_t);
void create(char* nombre_tabla, char* criterio, int numero_Particiones, int tiempo_Compactacion);
dato_t* realizar_select(char*, int);
void describe();
void describe_especifico(char*);

////////////////////////////////////////////////////////////////////////////////////

//int encontrar_tabla(char*);
int calcular_particion(int, int);
//dato_t *crear_dato(int , char* , time_t );
//bloque_tabla *existe_en_memtable(char* );
//void ingresar_a_memtable(dato_t*, char* );
//tabla_memtable* crear_tabla(char* tabla);
//tabla_memtable* ultima_posicion_memtable(void);
//tabla_memtable* encontrar_memtable(char* tabla);
bloque_tabla* crear_bloque(dato_t* dato);
dato_t* crear_dato(int clave, char* valor, time_t tiempo);
void poner_bloque_en_tabla(char* nombre_tabla , bloque_tabla* bloque_ingresar);
void crear_tabla_en_memtable(char* nombre_tabla);
void crear_Binario(char* ,int , char*,time_t);
void verificar_Binario();
dato_t* buscar_dato_en_binario(int key);

//char* obtenerPathTabla(char* nombre_Tabla);

#endif /* API_LISSANDRA_H_ */
