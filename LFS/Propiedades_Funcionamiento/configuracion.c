/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"


void creacion_del_config_fileSystem(){

	FILE* f = fopen("fileSystem.config", "w");
	fclose(f);

	g_config = config_create("fileSystem.config");

	config_set_value(g_config, "IP", "127.0.0.1");
	config_set_value(g_config, "PUERTO", "4445");
	config_set_value(g_config, "PUNTO_MONTAJE", "/home/utnso/Escritorio/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/");
	config_set_value(g_config, "RETARDO", "500");
	config_set_value(g_config, "TAMANIO_VALUE", "20");
	config_set_value(g_config, "TIEMPO_DUMP", "5000");


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


void obtener_puerto_ip(int* puerto,char** ip){

	g_config = config_create("lissandra.config");

	*puerto = config_get_int_value(g_config, "PUERTO");

	*ip = config_get_string_value(g_config,"IP");

}

void obtener_datos_metadata(){
	g_config = config_create("Metadata/Metadata.bin");
	//BLOCK SIZE, BLOCKS, MAGIC

	char * magic_aux;

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



