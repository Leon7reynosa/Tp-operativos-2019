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

		dato_a_buscar = buscar_key(key,segmento_tabla);

	}else{
		printf("No existe el segmento! \n");
	}

	free(dato_a_buscar);
}

/////////////////////////////////////////////////////////////////////////////

void insert(char* nombre_tabla, int key, char* value){



}

////////////////////////////////////////////////////////////////////////////

dato_t* buscar_key(int key, segmento* segmento_tabla){

	pagina* pagina_con_dato;
	dato_t* dato_encontrado;

	if(existe_pagina(key,segmento_tabla,&pagina_con_dato)){

		printf("existe la pagina numero: %d que tiene la key numero: %d\n",pagina_con_dato->numero_pagina,pagina_con_dato->dato_en_Memoria->key);
		dato_encontrado = pagina_con_dato->dato_en_Memoria;

	}
	else{
		printf("Se lo tenes que pedir al File System \n");
		//dato_encontrado = pedir_key_a_LFS(key, segmento_tabla->tabla);
		//y si no existe en el fileSystem?
	}

	return dato_encontrado;
}



////////////////////////////////////////////////////////////////////////////

int existe_pagina(int key, segmento* segmento_tabla, pagina** pagina_con_dato){

	pagina* pagina_aux = segmento_tabla->primera_pagina;

	while(pagina_aux != NULL){

		if(pagina_aux->dato_en_Memoria->key == key){
			printf("existe la pagina con la referencia a memoria de esa key! \n");
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
	segmento_tabla = tabla_segmentos;
	//sincronizar ?
	int tablas_restantes = cant_tablas;

	while(tablas_restantes > 0){
		if(strcmp(segmento_tabla->tabla , nombre_tabla) == 0){

			*segmento_encontrado = segmento_tabla;
			return 1;
		}else{
			segmento_tabla = segmento_tabla->siguiente_segmento;
			tablas_restantes --;
		}
	}

	*segmento_encontrado = NULL;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////

//arreglar
dato_t* pedir_key_a_LFS(int key, char* nombre_tabla){

	dato_t* dato_return;

	t_dato_recibido* dato_recibido = malloc(sizeof(t_dato_recibido));

	enviar_request_select(conexion_lissandra, nombre_tabla , key);

	dato_recibido = recibir_request_LFS(conexion_lissandra);

	dato_return = convertir_a_dato_t(dato_recibido);

	liberar_dato_recibido(dato_recibido);


	return dato_return;
}

////////////////////////////////////////////////////////////////////////////////

dato_t* convertir_a_dato_t(t_dato_recibido* dato_recibido){

	dato_t* dato_return = malloc(sizeof(dato_t));

	dato_return->key = dato_recibido->key;
	dato_return->value = dato_recibido->value->value;
	dato_return->timestamp = dato_recibido->timestamp;

	return dato_return;

}

/////////////////////////////////////////////////////////////////////////////

void liberar_dato_recibido(t_dato_recibido* dato_recibido){

	free(dato_recibido->value->value);
	free(dato_recibido->value);
	free(dato_recibido);

}

//////////////////////////////////////////////////////////////////////////////

void crear_segmento(char* tabla){

	segmento* nuevo_segmento = malloc(sizeof(segmento));
	int tamanio_nombre = strlen(tabla) + 1;

	nuevo_segmento->tabla = malloc(tamanio_nombre);
	strcpy(nuevo_segmento->tabla, tabla);

	nuevo_segmento->primera_pagina = NULL;

	nuevo_segmento->siguiente_segmento = NULL;

	agregar_segmento_tabla(nuevo_segmento);


}

//pasarle un segmento ya creado (allocado en memoria con malloc)
void agregar_segmento_tabla(segmento* nuevo_segmento){
	/*si la tabla de segmentos no esta inicializada
	  la inicializo*/
	if(tabla_segmentos == NULL){

		tabla_segmentos = nuevo_segmento;

	}else{
		segmento* segmento_aux;

		segmento_aux = ultimo_segmento_tabla();

		segmento_aux->siguiente_segmento = nuevo_segmento;
	}

}

// no usar si la tabla de segmentos no esta inicializada
segmento* ultimo_segmento_tabla(){
	segmento* ultimo_segmento;

	ultimo_segmento = tabla_segmentos;

	while(ultimo_segmento->siguiente_segmento != NULL ){

		ultimo_segmento = ultimo_segmento->siguiente_segmento;
	}

	return ultimo_segmento;
}

/////////////////////////////////////////////////////////////////////////

//no usar si el dato_t no esta previamente cargado en memoria!
void crear_pagina(segmento* tabla, dato_t* datos_pagina){

	pagina* nueva_pagina = malloc(sizeof(pagina));

	nueva_pagina->flag_modificado = FALSE;
	nueva_pagina->siguiente_pagina = NULL;

	nueva_pagina->dato_en_Memoria = datos_pagina;

	//si no hay ninguna pagina, meto esta como primera
	if(tabla->primera_pagina == NULL){
		tabla->primera_pagina = nueva_pagina;

	}else{
		pagina* pagina_aux;
		pagina_aux = ultima_pagina(tabla);
		nueva_pagina->numero_pagina = pagina_aux->numero_pagina + 1;
		pagina_aux->siguiente_pagina = nueva_pagina;
	}



}
//no usar si no tiene ninguna pagina
pagina* ultima_pagina(segmento* segmento_tabla){

	pagina* pagina_encontrada;

	pagina_encontrada = segmento_tabla->primera_pagina;

	while(pagina_encontrada->siguiente_pagina != NULL){
		pagina_encontrada = pagina_encontrada->siguiente_pagina;
	}

	return pagina_encontrada;
}

/////////////////////////////////////////////////////////////////////////
