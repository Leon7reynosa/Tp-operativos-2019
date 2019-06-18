#ifndef API_KERNEL_H_
#define API_KERNEL_H_

#include<time.h>
#include<commons/log.h>
#include<commons/string.h>
#include<readline/readline.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include"Mensajeria/mensajes.h"
#include"Config/configuracion.h"
#include"planificador.h"


t_queue* cola_new;

typedef struct{
	u_int16_t key;
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


void mandar_request(char* request_lql);
void obtener_parametros_select(char* linea_request, char* nombre_tabla, u_int16_t* key);
void obtener_parametros_insert(char* linea_request, char* nombre_tabla, u_int16_t* key, char* value, time_t* timestamp);
void obtener_parametros_add(char* linea_request, int numero_memoria, char* consistencia);
void obtener_parametros_insert_sin_timestamp(char* linea_request, char* nombre_tabla, u_int16_t* key, char* value);
void obtener_parametros_create(char* linea_request, char* nombre_tabla, char* criterio, int* numero_particiones, int* tiempo_compactacion);
void obtener_parametros_describe_de_una_tabla(char* linea_request, char* nombre_tabla);
void obtener_parametros_describe(char* linea_request);
void obtener_parametros_drop(char* linea_request, char* nombre_tabla);


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
