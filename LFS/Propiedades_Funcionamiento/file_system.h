/*
 * file_system.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include"comunity_func.h"
#include"configuracion.h"
#include"Lissandra.h"
#include<stdio.h>
#include<stdlib.h>
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

typedef struct{
	int size;
	int* bloques;
}particionContenido;

typedef struct{
	char* nombreTabla;
	int particion;
}ubicadorBloque;


typedef enum{
	LIBRE,
	OCUPADO
}estado;



int existe_la_tabla(char* tabla);
void crear_directorio(char* path_tabla);
void ingresar_A_Un_binario(char* nombre_tabla, dato_t* dato_ingresar, int particion);
void crear_Binario_tabla(char* nombre_tabla , u_int16_t key , char* valor, time_t timestamp);
void crear_Binario_Bloque(int indice, char* dato);
void crear_archivos_particiones(char* path_tabla, int numero_particiones);
dato_t* buscar_dato_en_particion(char* path_tabla, u_int16_t key);
char* obtenerMapDelBloque_ModoLectura(int indice, int* fichero, struct stat *atributosBloque);
void estadoDelBloque(int numeroBloque);
void mostrarEstadosTotales();
void setEstado(int numeroBloque, estado estado);
void setAllEstados(estado estado);
void getAllEstados();
void llenarBloque(char* dato, int indice);
int buscarPrimerIndiceVacio(void);
void mostrarDato(dato_t* dato);
void leerDatoDelBloque(int indice);
char* datoEnString(dato_t* dato);
char* datoEnFormatoBloque(dato_t* dato);
bool estaLleno(int indice);
dato_t* crearDato(u_int16_t key, time_t timestamp, char* value);
void realizarDump(void);
int tamanioDisponibleDelBloque(int indice);
void agregarBloqueAParticion(int particion, char* tabla);
ubicadorBloque ubicadorDelBloque(int indice);
void llenarTemporal(FILE* fd_tmp, t_list* dato_de_tabla);
void actualizarParticionConBloque(int indice);
int obtenerNumeroParticion(char* array);
bool getEstado(int indice);
char* extensionDelArchivo(char* path);
int cantidadDeBloques(int particion, char* nombreTabla);
void asignarBloqueVacioAParticion(char* nombreTabla, int particion, int bloque);
bool estaDentroDelRangoDeBloquesYEstaOcupado(int bloque);
void leerBloque(int indice);
int tamanioDelBloque(int indice);
bool noEsUnaUbicacionProhibida(char* path);
void leer_Particion(char* nombreTabla, int particion);
void leer_Particiones(char* nombreTabla);
#endif /* FILE_SYSTEM_H_ */
