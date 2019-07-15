/*
 * gossiping.h
 *
 *  Created on: 17 jun. 2019
 *      Author: utnso
 */

#ifndef GOSSIPING_H_
#define GOSSIPING_H_

#include"Mensajeria/Cliente/cliente.h"
#include"Mensajeria/mensajes.h"
#include"Mensajeria/requestGossiping.h"
#include"memoria.h"

#include<commons/collections/list.h>

void* gossiping(void);
void* serializar_gossiping(struct DatoTablaGossiping* dato);
void liberar_dato_memoria(struct MemoriasEstructura* memoria);
void liberar_dato_gossiping(struct DatoTablaGossiping* dato);
void enviar_datos(int memoria2, t_list* memorias );
void intercambiar_datos(tabla_gossip_dto tabla_ajena, int conexion);
Seed pasar_memoria_dto_a_seed(memoria_dto dato_dto);
void actualizar_tabla_gossip(tabla_gossip_dto request_gossip);

#endif /* GOSSIPING_H_ */
