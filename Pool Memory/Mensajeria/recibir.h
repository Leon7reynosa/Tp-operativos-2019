/*
 * recibir.h
 *
 *  Created on: 19 abr. 2019
 *      Author: utnso
 */

#ifndef RECIBIR_H_
#define RECIBIR_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/time.h>
#include<netdb.h>
#include<signal.h>
#include<ctype.h>
#include"commons/collections/list.h"

#include"requestSelect.h"
#include"requestInsert.h"
#include"requestCreate.h"
#include"requestDescribe.h"
#include"requestDrop.h"
#include"requestGossiping.h"

#include"configuracion.h"
#include"request.h"
#include"t_dato.h"

/*
typedef enum{
	DESCONEXION,
	MENSAJE
}cod_op;
*/

estado_request recibir_estado_request(int conexion);
request recibir_request(int conexion);
void recibir_mensaje(int conexion);
t_dato* recibir_dato_LFS(int conexion );
void* recibir_buffer(int* size,int conexion);
//cod_op determinar_operacion(char* buffer);
void desconectar_cliente(int conexion);
t_list* recibir_describe(int conexion);

#endif /* RECIBIR_H_ */
