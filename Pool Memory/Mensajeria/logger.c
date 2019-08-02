/*
 * logger.c
 *
 *  Created on: 22 jul. 2019
 *      Author: utnso
 */

#include"logger.h"

char* obtener_path_log(void){

	char* path_log = string_new();

	string_append(&path_log, "../Pool_Memory.log");

	return path_log;
}

char* obtener_path_log_gossip(void){

	char* path_log = string_new();

	string_append(&path_log, "../Gossip.log");

	return path_log;
}

char* obtener_path_log_journal(void){

	char* path_log = string_new();

	string_append(&path_log, "../Journal.log");

	return path_log;
}

void inicializar_logger(void){

	char* path_log = obtener_path_log();
	char* path_gossip = obtener_path_log_gossip();
	char* path_journal = obtener_path_log_journal();

	logger = log_create(path_log , "Pool Memory", 0 ,LOG_LEVEL_INFO);

	logger_gossip = log_create(path_gossip, "Pool Memory", 0, LOG_LEVEL_INFO);

	logger_journal = log_create(path_journal, "Pool_memory", 0, LOG_LEVEL_INFO);

	free(path_log);
	free(path_gossip);
	free(path_journal);
}


void liberar_logger(void){

	log_destroy(logger);
	log_destroy(logger_gossip);
	log_destroy(logger_journal);

}
