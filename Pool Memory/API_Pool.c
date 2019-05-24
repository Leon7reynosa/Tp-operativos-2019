/*
 * API_Pool.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Pool.h"


void realizar_select(char* nombre_tabla, int key){

	dato_t* dato_a_buscar;
	segmento* segmento_tabla;

	if(existe_segmento(nombre_tabla, &segmento_tabla)){

		printf("existe el segmento vieja! : %s\n", segmento_tabla->tabla);
		printf("busco la key! \n");

		dato_a_buscar = buscar_key(key,segmento_tabla);

		printf("Value : %s\n" , dato_a_buscar->value);

	}else{
		printf("No existe el segmento! \n");

	}

	free(dato_a_buscar);

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

/////////////////////////////////////////////////////////////////////////////

void insert(char* nombre_tabla, int key, char* value){



}

////////////////////////////////////////////////////////////////////////////

dato_t* buscar_key(int key, segmento* segmento_tabla){

	pagina* pagina_con_dato;
	dato_t* dato_encontrado = malloc(sizeof(dato_t));

	if(existe_pagina(key,segmento_tabla,&pagina_con_dato)){

		printf("existe la pagina numero: %d que tiene la key numero: %d\n",pagina_con_dato->numero_pagina,pagina_con_dato->dato.key);
		*dato_encontrado = pagina_con_dato->dato;

	}
	else{

		dato_encontrado = pedir_key_a_LFS(key, segmento_tabla->tabla);
	}

	return dato_encontrado;
}



////////////////////////////////////////////////////////////////////////////

/*Si existe la pagina que tenga esta key, en este segmento de tabla; se la asigno a
pagina_con_dato y y la funcion devuelve 1 (verdadero)
*/
int existe_pagina(int key, segmento* segmento_tabla,pagina** pagina_con_dato){

	pagina* pagina_aux = segmento_tabla->primera_pagina;

	while(pagina_aux != NULL){

		if(pagina_aux->dato.key == key){
			printf("existe la pagina con esa key! \n");
			*pagina_con_dato = pagina_aux;
			return 1;
		}
		else{
			pagina_aux = pagina_aux->siguiente_pagina;
		}

	}

	*pagina_con_dato = NULL;

	return 0;

}



///////////////////////////////////////////////////////////////////////////

/*Si existe un segmento en la memoria principal, que tenga asociado el nombre de esta
 * tabla, se la asigno a segmento_encontrado y devuelvo 1 (verdadero)
 *
 * */
int existe_segmento(char* nombre_tabla, segmento** segmento_encontrado){

	segmento* segmento_tabla;
	segmento_tabla = memoria_principal;

	while(segmento_tabla != NULL){
		if(strcmp(segmento_tabla->tabla , nombre_tabla) == 0){

			*segmento_encontrado = segmento_tabla;
			return 1;
		}else{
			segmento_tabla = segmento_tabla->siguiente_segmento;
		}
	}

	*segmento_encontrado = NULL;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////

dato_t* pedir_key_a_LFS(int key, char* nombre_tabla){

	dato_t* dato_recibido = malloc(sizeof(dato_t));

	//enviar_request_select(conexion_lissandra, nombre_tabla , key);

	//dato_recibido = recibir_request_LFS(conexion_lissandra);




	/*proximamente
	 * aca deberia de, a la vez que le pedimos la key, recibirla y agregar la pagina
	 * a la memoria en dicha tabla
	 * Saludos!
	 * */

	return dato_recibido;
}

//////////////////////////////////////////////////////////////////////////////
