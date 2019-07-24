/*
 * consistencia.h
 *
 *  Created on: 14 jun. 2019
 *      Author: utnso
 */

#ifndef CONSISTENCIA_H_
#define CONSISTENCIA_H_

#include<stdio.h>
#include<stdlib.h>
#include"gossiping.h"
#include<commons/collections/list.h>
#include"Mensajeria/requestDescribe.h"
#include"Mensajeria/estructuras.h"

typedef enum{
	SC,
	EC,
	SHC
}cod_consistencia;

typedef struct{

	cod_consistencia codigo_consistencia;

}metadata_t;



t_dictionary* registro_tabla; //va a tener char* nombre_tabla y los elementos van a ser metadata_t

//van a ser listas de distintas memorias, es decir, sus sockets


t_list* Eventual_C; //van a tener memoria_t*
t_list* Strong_Hash_C;

void inicializar_registro_tabla(void);
void inicializar_consistencias(void);
void agregar_a_consistencia(cod_consistencia codigo, memoria_t* memoria_a_guardar);
cod_consistencia identificar_consistencia(char* consistencia);
memoria_t* tomar_memoria_segun_codigo_consistencia(cod_consistencia codigo_consistencia , u_int16_t key);
cod_consistencia identificar_consitencia_para_request(int cod_request, void* tipo_request);
memoria_t* tomar_memoria_al_azar(void);
memoria_t* memoria_al_azar_consistencia(t_list* lista_consistencia);
memoria_t* seleccionar_memoria_consistencia(cod_operacion cod_op , void* tipo_request);
u_int16_t determinar_key(cod_operacion cod_op , void* tipo_request);
int funcion_hash(u_int16_t key);
void mostrar_memoria_utilizada(memoria_t* memoria_utilizada);
void guardar_tabla_consistencia(char* tabla, metadata_t* metadata_tabla);
int obtener_index_memoria(int key);
void actualizar_metadata(t_list* datos_describe);



#endif /* CONSISTENCIA_H_ */


