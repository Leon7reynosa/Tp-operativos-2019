/*
 * logger.h
 *
 *  Created on: 22 jul. 2019
 *      Author: utnso
 */

#ifndef MENSAJERIA_LOGGER_H_
#define MENSAJERIA_LOGGER_H_

#include<stdio.h>
#include<stdlib.h>

#include<commons/log.h>
#include<commons/string.h>

t_log* logger;

t_log* logger_gossip;

t_log* logger_journal;

char* obtener_path_log(void);
void inicializar_logger(void);
void liberar_logger(void);

#endif /* MENSAJERIA_LOGGER_H_ */
