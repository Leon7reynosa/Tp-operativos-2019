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
#include"semaforos.h"

t_log* logger_kernel;

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


int ejecutar_request(char* request_lql);
int identificar_request(char* request_lql);
int obtener_parametros_select(char* linea_request, char* nombre_tabla, u_int16_t* key);
int obtener_parametros_insert(char* linea_request, char* nombre_tabla, u_int16_t* key, char** value, time_t* timestamp);
int obtener_parametros_add(char* linea_request, int* numero_memoria, char* consistencia);
int obtener_parametros_insert_sin_timestamp(char* linea_request, char* nombre_tabla, u_int16_t* key, char* value);
int obtener_parametros_create(char* linea_request, char* nombre_tabla, char* criterio, int* numero_particiones, int* tiempo_compactacion);
void obtener_parametros_describe_de_una_tabla(char* linea_request, char* nombre_tabla);
int obtener_parametros_describe(char* linea_request, char* nombre_tabla);
void obtener_parametros_drop(char* linea_request, char* nombre_tabla);
char* obtener_parametros_run(char* linea_request);


#endif /* API_KERNEL_H_ */
