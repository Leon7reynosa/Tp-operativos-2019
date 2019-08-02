/*
 * inotify.c
 *
 *  Created on: 13 jul. 2019
 *      Author: utnso
 */

#include"inotify_prueba.h"

void inicializar_hilo_inotify(void){

	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	int err = pthread_create(&inotify_thread, &attr, realizar_inotify, arg_inotify);

	if(err){
		perror("pthread_create inotify");
		exit(1);
	}

	pthread_attr_destroy(&attr);

}

void liberar_hilo_inotify(void){


	pthread_cancel(inotify_thread);

	liberar_inotify(arg_inotify);

}

void liberar_inotify(inotify_config a_liberar){

	close(a_liberar->fd_inotify);

	free(a_liberar->path_config);

	free(a_liberar);

	pthread_rwlock_destroy(&semaforo_tiempo_dump);
	pthread_rwlock_destroy(&semaforo_retardo);              //estos quedan aca y punto

}

inotify_config crear_inotify(void){

	inotify_config nuevo = malloc(sizeof(struct inotifyEstructura));

	nuevo->fd_inotify = inotify_init();  // OJO SI DEVUELVE -1

	nuevo->path_config = obtener_path_config();

	nuevo->fd_watch = inotify_add_watch(nuevo->fd_inotify, nuevo->path_config, IN_OPEN | IN_MODIFY | IN_CLOSE); // OJO SI DEVUELVE -1

	pthread_rwlock_init(&semaforo_tiempo_dump, NULL);
	pthread_rwlock_init(&semaforo_retardo, NULL);     //ya fue, estos quedan aca y punto

	return nuevo;
}

void* realizar_inotify(inotify_config argumento){

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

		printf("[INOTIFY] Se creo el hilo\n");

		int  tamanio_leido;

		int tamanio_buffer = sizeof(struct inotify_event) + string_length("fileSystem.config")  + 1;

		char buffer[tamanio_buffer*4];
		char* buffer_aux;
		struct inotify_event* cambio_config;

		printf("[INOTIFY] VOY A EMPEZAR A MIRAR EL ARCHIVO");


		while(1){



			printf("\n[INOTIFY] voy a leer un nuevo evento\n");
			tamanio_leido = read(argumento->fd_inotify, buffer, tamanio_buffer);

			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

			//si read devuelve -1 y errno es EAGAIN, significa que no hay eventos, entonces si no es EAGAIN es error de lectura
			if(tamanio_leido == -1 && errno != EAGAIN){
				perror("read");
				pthread_exit(NULL);
			}

			if(tamanio_leido <= 0){
				printf("No hay eventos!\n");
				pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
				continue;
			}

			for(buffer_aux = buffer; buffer_aux < buffer + tamanio_leido;
				buffer_aux += sizeof(struct inotify_event) + cambio_config->len)
			{

				cambio_config = (struct inotify_event*)buffer_aux;

				if(cambio_config->mask & IN_OPEN){
					printf("[INOTIFY] Abrieron el archivo config\n");

				}else if(cambio_config->mask & IN_CLOSE_WRITE){ // mirar IN_MODIFY, diferencias

					printf("[INOTIFY] Se modifico el metadata.config\n");


					t_config* archivo_config = config_create(argumento->path_config);

					pthread_rwlock_wrlock(&semaforo_tiempo_dump);
					tiempo_dump = config_get_int_value(archivo_config, "TIEMPO_DUMP");
					pthread_rwlock_unlock(&semaforo_tiempo_dump);
					//SEMAFORO

					pthread_rwlock_wrlock(&semaforo_retardo);
					retardo = config_get_int_value(archivo_config, "RETARDO");
					pthread_rwlock_unlock(&semaforo_retardo);
					//SEMAFORO

					printf("[INOTIFY] TIEMPO DUMP: %i\n", tiempo_dump);
					printf("[INOTIFY] RETARDO: %i\n", retardo);

					config_destroy(archivo_config);

					printf("[INOTIFY] Se Cerro el metadata.config\n");

				}else if(cambio_config->mask & IN_CLOSE_NOWRITE){

					printf("[INOTIFY] Cerraron el archivo\n");

				}else{

					continue;
				}

			}

			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		}

		return NULL;
}
