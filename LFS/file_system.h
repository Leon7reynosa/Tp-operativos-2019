/*
 * file_system.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include"comunity_func.h"
#include"Config/configuracion.h"

typedef struct {
	u_int16_t key;
	char* value;
	time_t timestamp;
}bloque_t;

typedef struct{
	u_int16_t key;
	char value[50];
	time_t timestamp;
}datoAux;

typedef enum{
	LIBRE,
	OCUPADO
}estado;

int existe_la_tabla(char* tabla);
void crear_directorio(char* path_tabla);
void ingresar_A_Un_binario(char* nombre_tabla, dato_t* dato_ingresar, int particion);
void crear_Binario_tabla(char* nombre_tabla , u_int16_t key , char* valor, time_t timestamp);
void crear_Binario_Bloque(int indiceBloque, dato_t dato);
void crear_archivos_particiones(char* path_tabla, int numero_particiones);
dato_t* buscar_dato_en_binario(char* path_tabla, u_int16_t key);
void estadoDelBloque(int numeroBloque);
void mostrarEstadosTotales();
void setearEstado(int numeroBloque, estado estado);
void getEstado();
void llenarBloque(dato_t dato);
int indicePrimerBloqueVacio(void);
void mostrarDato(dato_t);

#endif /* FILE_SYSTEM_H_ */
