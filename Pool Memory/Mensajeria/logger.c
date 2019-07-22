/*
 * logger.c
 *
 *  Created on: 22 jul. 2019
 *      Author: utnso
 */

#include"logger.h"

void inicializar_logger(void){

	logger = log_create("Pool_Memory.log", "Pool Memory", 0 ,LOG_LEVEL_ERROR);


}


void liberar_logger(void){

	log_destroy(logger);

}
