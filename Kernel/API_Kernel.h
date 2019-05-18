#ifndef API_KERNEL_H_
#define API_KERNEL_H_

#include<time.h>

typedef enum criterio{
	SC, //Strong Consistency
	SHC, // Strong Hash Consistency
	EC //Eventual Consistency
}criterio_t;

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


select(char* nombre_tabla, int key);
insert(char* nombre_tabla, int key, char* value, time_t timestamp);
insert(char* nombre_tabla, int key, char* value);
create(char* nombre_tabla, criterio_t tipoConsistencia, int numeroParticiones, tiempoCompactacion); //ver tipo tiempoCompactacion
describe();
describe(char* nombre_tabla);
journal();
metrics();
add(int cantidad, criterio_t criterio);

#endif /* API_KERNEL_H_ */
