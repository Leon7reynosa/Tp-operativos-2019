/*
 * API_Pool.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Pool.h"

segmento* memoria_principal = NULL;

void select(char* nombre_tabla, int key){

	if(existe_segmento(nombre_tabla)){


	}else{


	}



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

}


void insert(char* nombre_tabla, int key, char* value){



}

int existe_segmento(char* nombre_tabla){

	segmento* segmento_tabla;
	segmento_tabla = memoria_principal;

	while(segmento_tabla != NULL){
		if(Strcmp(segmento_tabla->tabla , nombre_tabla) == 0){

			return 1;
		}else{
			segmento_tabla = segmento_tabla->siguiente_segmento;
		}
	}

	return 0;
}
