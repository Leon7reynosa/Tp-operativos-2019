/*
 * config.c
 *
 *  Created on: 30 abr. 2019
 *      Author: utnso
 */

#include "configuracion.h"


void creacion_del_config_fileSystem(){
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
	//USAR SOLO AL INICIALIZAR !, SETEA TODO EN 0 (aparentemente xd)

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

	munmap(archivo_en_memoria, myStat.st_size);

	close(archivo);


}


void obtener_puerto_ip(int* puerto,char** ip){

	g_config = config_create("lissandra.config");

	*puerto = config_get_int_value(g_config, "PUERTO");

	*ip = config_get_string_value(g_config,"IP");

}

void obtener_datos_metadata(){
	g_config = config_create("fileSystem.metadata");
	//BLOCK SIZE, BLOCKS, MAGIC

	block_size   = config_get_int_value(g_config, "BLOCK_SIZE");
	magic_number = config_get_string_value(g_config, "MAGIC_NUMBER");
	blocks       = config_get_int_value(g_config, "BLOCKS");

	config_destroy(g_config);
}

void obtener_datos_config(){
	g_config = config_create("fileSystem.metadata");
	//BLOCK SIZE, BLOCKS, MAGIC

	tamanio_value_max   = config_get_int_value(g_config, "TAMANIO_VALUE");
	tiempo_dump         = config_get_string_value(g_config, "TIEMPO_DUMP");
	puerto_lfs          = config_get_int_value(g_config, "PUERTO");
	punto_montaje       = config_get_int_value(g_config, "PUNTO_MONTAJE");
	retardo             = config_get_int_value(g_config, "RETARDO");
	ip_lfs              = config_get_string_value(g_config, "IP");

	config_destroy(g_config);
}



