/*
 * inotify.h
 *
 *  Created on: 13 jul. 2019
 *      Author: utnso
 */

#ifndef INOTIFY_PRUEBA_H_
#define INOTIFY_PRUEBA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/string.h>
#include<pthread.h>
#include<stdbool.h>

#include"Propiedades_Funcionamiento/metadata.h"

#include<errno.h>
#include<poll.h>
#include<sys/inotify.h>
#include<unistd.h>

void* notificar_cambio_config(void* argumento);

#endif /* INOTIFY_PRUEBA_H_ */
