/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"

void realizar_handshake(int conexion){

	//por ahora solo manda tamanio_value
	int bytes = sizeof(int);

	void* buffer = malloc(bytes);

	memcpy(buffer, &tamanio_value_max, sizeof(int));

	send(conexion, buffer, bytes , 0 );

	free(buffer);
}

void creacion_del_config_fileSystem(){

	FILE* f = fopen("fileSystem.config", "w");
	fclose(f);

	g_config = config_create("fileSystem.config");

	config_set_value(g_config, "IP", "127.0.0.1");
	config_set_value(g_config, "PUERTO", "5003");
	config_set_value(g_config, "PUNTO_MONTAJE", "/home/utnso/Escritorio/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/");
	config_set_value(g_config, "RETARDO", "0");
	config_set_value(g_config, "TAMANIO_VALUE", "60");
	config_set_value(g_config, "TIEMPO_DUMP", "20000");


	config_save(g_config);
	config_destroy(g_config);

}


void creacion_del_metadata_fileSystem(){

	FILE* f = fopen("Metadata/Metadata.bin", "w");
	fclose(f);

	g_config = config_create("Metadata/Metadata.bin");


	config_set_value(g_config, "BLOCK_SIZE", "64");
	config_set_value(g_config, "BLOCKS", "5192");
	config_set_value(g_config, "MAGIC_NUMBER", "LISSANDRA");

	config_save(g_config);
	config_destroy(g_config);
}


void creacion_bitmap(){

	int archivo_bitmap;
	char* path = "Metadata/bitmap.bin";
	char* bitmap;
	int trunqueador;


	if(!existe_el_bitmap()){
		archivo_bitmap = open(path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

		if(archivo_bitmap == -1){
			perror("Archivo_Bitmap");
		}


		trunqueador = ftruncate(archivo_bitmap, blocks/8);

		if(trunqueador != 0){
			perror("ftruncate");
		}

		struct stat atributosBitmap;

		if(fstat(archivo_bitmap, &atributosBitmap) == -1){
			perror("No se pudo obtener los atributos del archivo \n");
		}

		bitmap = mmap(NULL, atributosBitmap.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, archivo_bitmap, 0);

		t_bitarray* bitarray = bitarray_create_with_mode(bitmap, atributosBitmap.st_size, MSB_FIRST);

		for(int i = 0; i < atributosBitmap.st_size*8; i++){
			bitarray_clean_bit(bitarray, i);
		}

		msync(bitmap, atributosBitmap.st_size, MS_SYNC);

		munmap(bitmap, atributosBitmap.st_size);
		bitarray_destroy(bitarray);
		close(archivo_bitmap);

	}

	else{
		return;
	}



}


void obtener_puerto_ip(int* puerto,char** ip){

	g_config = config_create("lissandra.config");

	*puerto = config_get_int_value(g_config, "PUERTO");

	*ip = config_get_string_value(g_config,"IP");

}

char* obtener_path_metadata_bin(){

	char* path = string_new();

	char* path_meta = malloc(strlen("Metadata/Metadata.bin") + 1);
	memcpy(path_meta, "Metadata/Metadata.bin", strlen("Metadata/Metadata.bin") + 1);

	string_append(&path, punto_montaje);
	string_append(&path, path_meta);

	free(path_meta);

	return path;

}

void obtener_datos_metadata(){

	char* path_metadata = obtener_path_metadata_bin();

	g_config = config_create(path_metadata);

	free(path_metadata); //agregadp para el memleak del obtener path metadata bin -> Anda bien !

	//BLOCK SIZE, BLOCKS, MAGIC

	char * magic_aux ;

	block_size   = config_get_int_value(g_config, "BLOCK_SIZE");
	magic_aux    = config_get_string_value(g_config, "MAGIC_NUMBER");
	blocks       = config_get_int_value(g_config, "BLOCKS");

	magic_number = malloc(string_length(magic_aux) + 1);
	memcpy(magic_number, magic_aux, string_length(magic_aux) + 1);

	config_destroy(g_config);
}

void obtener_datos_config(){
	g_config = config_create("fileSystem.config");

	char* punto_montaje_aux;
	char* ip_lfs_aux;

	tamanio_value_max   = config_get_int_value(g_config, "TAMANIO_VALUE");
	tiempo_dump         = config_get_int_value(g_config, "TIEMPO_DUMP");
	puerto_lfs          = config_get_int_value(g_config, "PUERTO");
	punto_montaje_aux   = config_get_string_value(g_config, "PUNTO_MONTAJE");
	retardo             = config_get_int_value(g_config, "RETARDO");
	ip_lfs_aux          = config_get_string_value(g_config, "IP");

	ip_lfs = malloc(strlen(ip_lfs_aux) + 1);
	punto_montaje = string_new();//malloc(strlen(punto_montaje_aux) + 1);

	string_append(&punto_montaje, punto_montaje_aux);

	memcpy(ip_lfs, ip_lfs_aux, strlen(ip_lfs_aux) +1 );
	//memcpy(punto_montaje , punto_montaje_aux, strlen(punto_montaje_aux) +1 );

	config_destroy(g_config);
}

void crearYObtenerDatos(){
	creacion_del_config_fileSystem();
	creacion_del_metadata_fileSystem();;
	obtener_datos_config();
	obtener_datos_metadata();
	tamanioDeValue = sizeof(u_int16_t) + tamanio_value_max + sizeof(time_t);
}

void obtenerDatos(){
	obtener_datos_config();
	obtener_datos_metadata();
}

bool existe_el_bitmap(){

	char* path_al_bitmap = string_new();

	string_append(&path_al_bitmap, punto_montaje);
	string_append(&path_al_bitmap, "Metadata/bitmap.bin");

	FILE* file;
	if((file = fopen(path_al_bitmap, "r")) == NULL){

		free(path_al_bitmap);

		return false;
	}
	else{

		fclose(file);
		free(path_al_bitmap);

		return true;
	}

}

void inicializar_loggers(){
	logger_lissandra = 		log_create("lissandra.log", "lissandra", 0, LOG_LEVEL_INFO);
	logger_lfs = 			log_create("lfs.log", "file system", 0, LOG_LEVEL_INFO);
	logger_compactador = 	log_create("compactador.log", "compactador", 0, LOG_LEVEL_TRACE);
	logger_request = 		log_create("requests.log", "requests", 0, LOG_LEVEL_TRACE );
}

void destruir_loggers(){
	log_destroy(logger_lissandra);
	log_destroy(logger_lfs);
	log_destroy(logger_compactador);
	log_destroy(logger_request);
}

void inicializar_compactador(){
	diccionario_compactador = dictionary_create();
	pthread_rwlock_init(&(lock_diccionario_compactacion), NULL);
}

void liberar_globales(void){
	free(magic_number);
	free(punto_montaje);
	free(ip_lfs);
}



