/*
 * API_Pool.h
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#ifndef API_POOL_H_
#define API_POOL_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<time.h>
#include<commons/config.h>
#include<string.h>
#include<sys/stat.h>

#define TAMANIO_MAX_VALUE 50

typedef enum{

	FALSE,
	TRUE

}bool;

typedef struct{

	int key;
	char value[TAMANIO_MAX_VALUE];
	time_t timestamp;

}dato_t;

typedef struct{

	int numero_pagina;
	dato_t dato;
	bool flag_modificado;
	struct pagina* siguiente_pagina;

}pagina;

typedef struct{

	char* tabla; //ESTO DESPUES TIENE QUE SER EL PATH
	pagina* primera_pagina;
	struct segmento* siguiente_segmento;

}segmento;



void select(char* nombre_tabla, int key);
/*
 * 1)Verificar si existe el segmento de la tabla solicitada y buscar en las paginas
 * del mismo si contiene la key solicitada. Si la contiene, devuelve su valor y
 * finaliza el proceso
 * 2)Si no la contiene, enviar la solicitud a FileSystem para obtener el valor
 * solicitado y almacenarlo
 * 3)Una vez obtenido, se debe solicitar una nueva pagina libre para almacenar
 * el mismo. En caso de no disponer de una pagina libre, se debe ejecutar el
 * algoritmo de reemplazo y, en caso de no poder efectuarlo por estar la memoria
 * full, ejecutar el Journal de la memoria
 */

void insert(char* nombre_tabla, int key, char* value);


#endif /* API_POOL_H_ */
