/*
 * inotify.c
 *
 *  Created on: 13 jul. 2019
 *      Author: utnso
 */

#include"inotify_prueba.h"

void* notificar_cambio_config(void* argumento){

	printf("Se creo el hilo\n");

	int fd_inotify, watch_descriptor, tamanio_leido;
	char* path_config = obtener_path_metadata_de_tabla((char *)argumento);

	int tamanio_buffer = sizeof(struct inotify_event) + string_length("Metadata.config")  + 1;

	char buffer[tamanio_buffer*4];
	char* buffer_aux;
	struct inotify_event* cambio_config;

	fd_inotify = inotify_init();

	if(fd_inotify == -1){
		perror("inotify_init");
		//pthread_exit();
	}

	watch_descriptor = inotify_add_watch(fd_inotify, path_config, IN_OPEN | IN_MODIFY | IN_CLOSE);

	if(watch_descriptor == -1){
		perror("inotify_add_watch");
		//pthread_exit();
	}

	bool mirar_config = true;
	int numero_de_veces = 2;

	printf("[HILO] VOY A EMPEZAR A MIRAR EL ARCHIVO");
	while(mirar_config){

		printf("\n[HILO] voy a leer un nuevo evento\n");
		tamanio_leido = read(fd_inotify, buffer, tamanio_buffer);

		//si read devuelve -1 y errno es EAGAIN, significa que no hay eventos, entonces si no es EAGAIN es error de lectura
		if(tamanio_leido == -1 && errno != EAGAIN){
			perror("read");
			//pthread_exit();
		}

		if(tamanio_leido <= 0){
			printf("No hay eventos!\n");
			continue;
		}

		for(buffer_aux = buffer; buffer_aux < buffer + tamanio_leido;
			buffer_aux += sizeof(struct inotify_event) + cambio_config->len)
		{

			cambio_config = (struct inotify_event*)buffer_aux;

			if(cambio_config->mask & IN_OPEN){
				printf("[HILO] Abrieron el archivo config\n");

			}else if(cambio_config->mask & IN_CLOSE_WRITE){ // mirar IN_MODIFY, diferencias
				printf("[HILO] Se modifico el metadata.config\n");

				metadata_t* nueva_metadata = obtener_metadata((char *)argumento);

				printf("[HILO] Nuevo tiempo de compactacion = %i\n", nueva_metadata->compactacion);

				printf("[HILO] Se Cerro el metadata.config");
				free(nueva_metadata);

			}else if(cambio_config->mask & IN_CLOSE_NOWRITE){

				printf("[HILO] Cerraron el archivo\n");

				numero_de_veces--;

				if(numero_de_veces == 0){
					printf("[HILO] Dejo de mirar el archivo ;)");
					mirar_config = false;
				}

			}else{

				continue;
			}

		}


	}

	close(fd_inotify);
	free(path_config);

	return NULL;
}
