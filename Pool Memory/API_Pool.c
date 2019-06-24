/*
 * API_Pool.c
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#include "API_Pool.h"

void trabajar_request(request nueva_request , int conexion){

	Dato dato_select;
	t_dato* dato_a_enviar;

	switch(nueva_request->cod_op){

		case SELECT:

			dato_select = request_select( (select_t) nueva_request->tipo_request);

			dato_a_enviar = crear_t_dato(dato_select->key, dato_select->timestamp , dato_select->value);

			enviar_dato(dato_a_enviar, conexion);

			liberar_t_dato(dato_a_enviar);

			break;
		case INSERT:



			break;

		case CREATE:



			break;

		case GOSSIP:



			break;

		default:

			break;
	}

}



Dato request_select(select_t dato){

	Segmento segmento_tabla;
	Pagina pagina_encontrada;
	Dato dato_encontrado;

	if(existe_segmento(dato->tabla->buffer,&segmento_tabla)){
		if(existe_pagina(segmento_tabla, dato->key, &pagina_encontrada)){
			mostrar_datos(pagina_encontrada);
			dato_encontrado = decodificar_dato_de_memoria(pagina_encontrada->referencia_memoria);

		}else{

			//VER ESTO, si se puede hacer una funcion para no repetir logica
			printf("Le pido las cosas al LFS \n");

			Dato dato_lfs = pedir_dato_al_LFS(dato->tabla->buffer, dato->key);
			/*pagina_encontrada = solicitar_pagina();
			agregar_pagina(segmento_tabla, pagina_encontrada);
			mostrar_dato(pagina_encontrada);

			liberar_dato(dato_lfs);
			*/
			printf("Le mande el mensaje a lissandra\n");
		}

	}else{
		printf("No existe el segmento, lo tenes que crear y pedirle el dato al LFS! \n");
		//VER ESTO
		/*
		agregar_segmento(dato->tabla->buffer, memoria->tabla_segmentos);

		Dato dato_lfs = pedir_dato_al_LFS(dato->tabla->buffer, dato->key);

		printf("No Existe el segmento con dicha tabla \n\n");
		*/
	}

	return dato_encontrado;
}

void request_insert(insert dato){

	Segmento segmento_tabla;
	Pagina pagina_encontrada;
	Dato dato_insert;

	if(string_length(dato->value->buffer) > tamanio_value){
		printf("SEGMENTATION FAULT! Te pasaste en el tamaño del value \n");
		return;
	}

	time_t timestamp = time(NULL);

	if(timestamp < 0){
		//se pudre todo, el timestamp no esta disponible
		exit(1);
	}
	////// HAY QUE GUARDAR ESTO BIEN CON EL TIEMPO ACTUAL

	dato_insert = crear_dato(dato->key, dato->value->buffer, timestamp );

	if(existe_segmento(dato->tabla->buffer ,&segmento_tabla)){
		printf("Existe el segmento!\n");
		if(existe_pagina(segmento_tabla, dato->value->buffer, &pagina_encontrada)){

			printf("Existe la pagina!\n");
			actualizar_pagina(pagina_encontrada, dato_insert);
			mostrar_datos(pagina_encontrada);

		}else{
			printf("No existe la pagina!\n");
			pagina_encontrada = solicitar_pagina(); //SEGUIR Y VER
			actualizar_pagina(pagina_encontrada, dato_insert);
			agregar_pagina(segmento_tabla, pagina_encontrada);
			printf("LA TABLA A AHORA TIENE %i SEGMENTOS\n", segmento_tabla->Tabla_paginas->elements_count);
			mostrar_datos(pagina_encontrada);

		}


	}else{
		printf("No existe el segmento, deberia crearlo!\n");
	}

	liberar_dato(dato_insert);
}


void request_create(create dato_create){

	enviar_request(CREATE, dato_create);

	//seguir!

}
