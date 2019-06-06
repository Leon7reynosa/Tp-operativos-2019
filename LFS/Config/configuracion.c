/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"


void creacion_del_config_lissandra(){


	g_config = config_create("lissandra.config");

	config_set_value(g_config, "IP", "127.0.0.1");
	config_set_value(g_config, "PUERTO", "4445");
	config_save(g_config);
	config_destroy(g_config);

}

void creacion_del_config_file_system(){
	//FALTA EL PUNTO DE MONTAJE
	g_config = config_create("fileSystem.config");
	config_save_in_file(g_config, "LFS/fileSystem.config"); //anda cuando lo creo el archivo aparte, sino no xD

	config_set_value(g_config, "BLOCK_SIZE", "64");
	config_set_value(g_config, "BLOCKS", "5192");
	config_set_value(g_config, "MAGIC_NUMBER", "LISSANDRA");

	config_save(g_config);
	config_destroy(g_config);
}

void creacion_bitmap(){

//	log_info(); hace falta
	int archivo;
	int cantidadDeBloques;
	char* path = "bitmap.bin";
	char* bitmap; //al guardarlo en un chat*, obtengo la mas minima relacion de
				  // tamanio de todas las demas variables

	g_config = config_create("fileSystem.config");

	cantidadDeBloques = config_get_int_value(g_config, "BLOCKS");

	printf("Cantidad de Bloques = %d\n", cantidadDeBloques);

	//LO CREA PERO NO PASA AL HOLA
	archivo = open(path, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);


	bitmap = mmap(NULL, cantidadDeBloques, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_SHARED, archivo, 0);
	//after the mmap() call has returned, the file descriptor, "archivo", can be closed
	//without invalidating the mapping
	close(archivo);

	t_bitarray* bitArray = bitarray_create_with_mode(bitmap, cantidadDeBloques, LSB_FIRST);

	for(int i = 0; i < 3; i++){
		bitarray_set_bit(bitArray, i);
		bool result = bitarray_test_bit(bitArray, i);
		printf("%d\n", result);
	}


}


void obtener_puerto_ip(int* puerto,char** ip){

	g_config = config_create("lissandra.config");

	*puerto = config_get_int_value(g_config, "PUERTO");

	*ip = config_get_string_value(g_config,"IP");

}

