/*
 * metadata.h
 *
 *  Created on: 22 jul. 2019
 *      Author: utnso
 */

#ifndef METADATA_H_
#define METADATA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>
#include"Mensajeria/request.h"
#include"Mensajeria/requestDescribe.h"
#include"Config/configuracion.h"
#include<pthread.h>

void* refrescar_metadata(void);

#endif /* METADATA_H_ */
