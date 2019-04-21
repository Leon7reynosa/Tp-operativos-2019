/*
 * Lissandra.h
 *
 *  Created on: 9 abr. 2019
 *      Author: utnso
 */

#ifndef LISSANDRA_H_
#define LISSANDRA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

//#include "utils.h"
#include "utilServer.h"

	//Por ahora pongo aca

	/*
	 * typedef struct{
	 * 		key (Tiene q ser uint 16)
	 * 		value
	 * 		timestamp
	 *
	 * 		} dato
	 *
	 */



t_log* iniciar_log_server(void);

#endif /* LISSANDRA_H_ */
