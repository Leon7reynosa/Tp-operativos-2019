/*
 * inotify.h
 *
 *  Created on: 1 ago. 2019
 *      Author: utnso
 */

#ifndef INOTIFY_MEMORIA_H_
#define INOTIFY_MEMORIA_H_


#include<stdio.h>
#include<stdlib.h>

#include<sys/inotify.h>
#include<errno.h>
#include<unistd.h>

#include"Mensajeria/configuracion.h"

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

#endif /* INOTIFY_MEMORIA_H_ */
