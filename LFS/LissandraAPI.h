/*
 * LissandraAPI.h
 *
 *  Created on: 16 abr. 2019
 *      Author: utnso
 */

#ifndef LISSANDRAAPI_H_
#define LISSANDRAAPI_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>


	//void select(char* , int);

	void insert(char*, int, int, char*);

	void create(char*, int);

	void describe(char*);

	void drop(char* );



#endif /* LISSANDRAAPI_H_ */
