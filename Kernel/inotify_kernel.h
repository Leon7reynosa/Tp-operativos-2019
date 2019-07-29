/*
 * inotify_kernel.h
 *
 *  Created on: 28 jul. 2019
 *      Author: utnso
 */

#ifndef INOTIFY_KERNEL_H_
#define INOTIFY_KERNEL_H_

#include<stdio.h>
#include<stdlib.h>

#include<stdbool.h>
#include<errno.h>
#include<sys/inotify.h>
#include<pthread.h>

#include"Config/configuracion.h"

#include<commons/config.h>

struct inotifyEstructura{

	int fd_inotify;
	int fd_watch;
	char* path_config;

};

typedef struct inotifyEstructura* inotify_config;

void* realizar_inotify(inotify_config argumento);
inotify_config crear_inotify(void);
void liberar_inotify(inotify_config a_liberar);

#endif /* INOTIFY_KERNEL_H_ */
