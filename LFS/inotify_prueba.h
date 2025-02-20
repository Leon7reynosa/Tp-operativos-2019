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

#include"Propiedades_Funcionamiento/configuracion.h"

struct inotifyEstructura{

	int fd_inotify;
	int fd_watch;
	char* path_config;

};

typedef struct inotifyEstructura* inotify_config;

inotify_config arg_inotify;

pthread_t inotify_thread;

void* realizar_inotify(inotify_config argumento);
inotify_config crear_inotify(void);
void liberar_inotify(inotify_config a_liberar);

#endif /* INOTIFY_PRUEBA_H_ */
