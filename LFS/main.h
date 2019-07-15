/*
 * main.h
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */

#ifndef MAIN_H_
#define MAIN_H_

#include<pthread.h>
#include"Propiedades_Funcionamiento/comunity_func.h"
#include"Propiedades_Funcionamiento/Mensajeria/request.h"
//#include"Propiedades_Funcionamiento/file_system.h"
#include "Propiedades_Funcionamiento/configuracion.h"
#include"Propiedades_Funcionamiento/file_system_aux.h"
#include"Propiedades_Funcionamiento/consola.h"

#include<commons/config.h>
#include"Propiedades_Funcionamiento/loggers.h"
#include"Propiedades_Funcionamiento/conexiones.h"
/*
t_log* logger_lissandra;
t_log* logger_lfs;
t_log* logger_compactador;
*/
void* conectar_memoria(void);
void pruebas(void);
void prueba_dump(void);

#endif /* MAIN_H_ */
