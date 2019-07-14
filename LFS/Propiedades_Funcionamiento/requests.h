/*
 * api_memory.h
 *
 *  Created on: 1 jun. 2019
 *      Author: utnso
 */

#ifndef REQUESTS_H_
#define REQUESTS_H_

#include"comunity_func.h"
#include"Lissandra.h"
#include"file_system_aux.h"
#include"compactador.h"
#include"metadata.h"

dato_t* request_select(select_t dato_select);
void request_insert(insert dato_insert);
void request_create(create dato_create);


#endif /* REQUESTS_H_ */
