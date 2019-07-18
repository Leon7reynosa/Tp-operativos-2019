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

#include<commons/collections/list.h>
#include"Config/configuracion.h"
#include"Mensajeria/request.h"
#include"Mensajeria/Cliente/cliente.h"



#include"API_Kernel.h"

struct MemoriasEstructura{
	int numero_memoria;
	t_stream* ip;
	int puerto;

};
 //PARA LA TABLA_GOSSIPING

typedef struct{

	int numero_memoria;
	char* ip;
	int puerto;
	int socket;

}memoria_t;

t_list* tabla_gossiping;


void actualizar_gossiping(void);
void recibir_actualizacion_gossiping(void);
void ingresar_a_tabla_gossiping(memoria_t* dato_memoria_ingresar);
memoria_t* convertir_a_memoria_t(struct MemoriasEstructura* dato_memoria);
bool existe_en_tabla_gossiping(struct MemoriasEstructura* dato_memoria);
void liberar_dato_memoria(struct MemoriasEstructura* memoria);
void liberar_memoria_t(memoria_t* dato_a_borrar);


#endif /* GOSSIPING_H_ */
