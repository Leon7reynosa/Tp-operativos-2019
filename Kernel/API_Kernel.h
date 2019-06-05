#ifndef API_KERNEL_H_
#define API_KERNEL_H_

#include<time.h>
#include"Mensajeria/mensajes.h"

typedef struct{
	int key;
	char* value;
	time_t timestamp;
}dato_t;

typedef struct{
	dato_t* dato_t;
	struct bloque_tabla *dato_sig;
}bloque_tabla;

typedef struct{
	char* nombre_tabla; //uso un string para ver el nombre de la tabla, si quieren despues cambienlo
	bloque_tabla* primer_bloque;
	struct tabla_memtable* sig_tabla;
}tabla_memtable;


void enviar_select(int conexion, char* nombre_tabla, u_int16_t key);
void enviar_insert(int conexion, char* nombre_tabla, u_int16_t key, char* value, time_t timestamp);
void enviar_create(int conexion, char* nombre_tabla, criterio_t criterio, int numero_particiones, int tiempo_compactacion);
void enviar_describe(int conexion, char* nombre_tabla);

/*
insert(char* nombre_tabla, int key, char* value, time_t timestamp);
insertSinTimestamp(char* nombre_tabla, int key, char* value);
create(char* nombre_tabla, criterio_t tipoConsistencia, int numeroParticiones, tiempoCompactacion); //ver tipo tiempoCompactacion
describeSinParametro();
describe(char* nombre_tabla);
journal();
metrics();
add(int cantidad, criterio_t criterio);
*/
#endif /* API_KERNEL_H_ */
