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
	config_set_value(g_config, "PUNTO_MONTAJE", "./LFS");
	config_save(g_config);
	config_destroy(g_config);

}

void creacion_del_config_file_system(){
	//FALTA EL PUNTO DE MONTAJE
	g_config = config_create("fileSystem.metadata");
	config_save_in_file(g_config, "LFS/fileSystem.metadata"); //anda cuando lo creo el archivo aparte, sino no xD

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
	char* path = "./bitmap.bin";
	char* archivo_en_memoria; //al guardarlo en un chat*, obtengo la mas minima relacion de
				  // tamanio de todas las demas variables



	g_config = config_create("fileSystem.metadata");

	cantidadDeBloques = config_get_int_value(g_config, "BLOCKS");

	printf("Cantidad de Bloques = %d\n", cantidadDeBloques);


	archivo = open(path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	ftruncate(archivo, cantidadDeBloques);
	struct stat myStat;

	if(fstat(archivo, &myStat) == -1){
		perror("No se pudo obtener el tamanio del archivo \n");
	}
	printf("Bitmap size = %ld\n", myStat.st_size);


	archivo_en_memoria = mmap(NULL, myStat.st_size, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_SHARED, archivo, 0);
	//after the mmap() call has returned, the file descriptor, "archivo", can be closed
	//without invalidating the mapping

	for(int i = 0; i < myStat.st_size; i++){
		write(archivo, 0, sizeof(char));
	}

	printf("\n");

	munmap(archivo_en_memoria, myStat.st_size);

	close(archivo);


}


void obtener_puerto_ip(int* puerto,char** ip){

	g_config = config_create("lissandra.config");

	*puerto = config_get_int_value(g_config, "PUERTO");

	*ip = config_get_string_value(g_config,"IP");

}

