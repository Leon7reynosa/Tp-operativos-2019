/*
 * gossiping.h
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#ifndef GOSSIPING_H_
#define GOSSIPING_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include<commons/collections/list.h>
#include"Config/configuracion.h"
#include"Mensajeria/request.h"
#include"Mensajeria/Cliente/cliente.h"
#include"consistencia.h"
#include"Mensajeria/estructuras.h"



#include"API_Kernel.h"



struct MemoriasEstructura{
	int numero_memoria;
	t_stream* ip;
	int puerto;

};
 //PARA LA TABLA_GOSSIPING


t_list* tabla_gossiping;


memoria_t* Strong_C;

void* realizar_gossiping(void);
void remover_memoria_de_consistencia(memoria_t* memoria);
memoria_t* remover_conexion( int conexion , t_list* lista_a_remover );
void eliminar_memoria_t( memoria_t* dato_memoria );
int tomar_socket_memoria_aleatorio(t_list* lista_memorias);
bool actualizar_gossiping(int conexion);
bool recibir_actualizacion_gossiping(int conexion);
void inicializar_tabla_gossiping();
void ingresar_a_tabla_gossiping(memoria_t* dato_memoria_ingresar);
struct MemoriasEstructura* crear_memoria_estructura(char* ip , int puerto, int numero_memoria);
memoria_t* crear_memoria_t(char* ip , int puerto , int numero_memoria);
memoria_t* convertir_a_memoria_t(struct MemoriasEstructura* dato_memoria);
bool existe_en_tabla_gossiping(struct MemoriasEstructura* dato_memoria);
void liberar_dato_memoria(struct MemoriasEstructura* memoria);
void liberar_memoria_t(memoria_t* dato_a_borrar);



#endif /* GOSSIPING_H_ */
