/*
 * requestInsert.c
 *
 *  Created on: 6 jun. 2019
 *      Author: utnso
 */

#include"requestInsert.h"

struct insertEstruct{

	t_stream* tabla;
	u_int16_t key;
	t_stream* value;
	time_t timestamp;

};

