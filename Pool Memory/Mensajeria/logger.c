/*
 * logger.c
 *
 *  Created on: 22 jul. 2019
 *      Author: utnso
 */

#include"logger.h"

char* obtener_path_log(void){

	char* path_log = string_new();

	string_append(&path_log, "/home/utnso/Escritorio/TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/Pool Memory/Pool_Memory.log");

	return path_log;
}

void inicializar_logger(void){

	char* path_log = obtener_path_log();

	logger = log_create(path_log , "Pool Memory", 0 ,LOG_LEVEL_INFO);


}


void liberar_logger(void){

	log_destroy(logger);

}
