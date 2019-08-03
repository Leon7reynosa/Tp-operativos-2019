/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"

int main (int argc , char* argv[]){

	system("clear");

	arg_inotify = crear_inotify();

	//creacion_del_config();
	////////////////////////////////////INICIALIZACIONES/////////////////////////////////
	ip_escucha = obtener_ip_local();

	obtener_datos_config();

	inicializar_logger();

//TODO HANDSHAKE CON LISSANDRA (en archivo config por ahora)
	realizar_handshake();

	tamanio_dato = tamanio_value + sizeof(u_int16_t) + sizeof(time_t);

	conexion_lissandra = true;

	inicializar_memoria(tamanio, tamanio_value, tamanio_dato); //TODO ARREGLAR ESTA FUNCION UN POCO

	inicializar_hilos();

	//////////////////////////////////////////MAIN///////////////////////////////////////

	fd_set master;
	fd_set read_fds;

	int fd_max;
	int new_fd;
	int listener;
	int i;

	//CADA CUANTO TIEMPO, el select va a estar esperando que pase algo (cada 60 segundos)
	struct timeval espera;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	if(( listener = iniciar_servidor(ip_escucha, puerto_escucha) ) == -1){

		log_info(logger_estado, "No se pudo levantar el servidor para la memoria\n");
		exit(-1);
	}

	FD_SET(0, &master);
	FD_SET(listener, &master);
	fd_max = listener;

	for ( ; ; ){

		read_fds = master;

		espera.tv_sec = 30;
		espera.tv_usec = 0;

		if(select(fd_max +1 , &read_fds, NULL, NULL, &espera) == -1){
			perror("select.");
			exit(1);
		}

		for(i = 0; i<= fd_max ; i++){

			if(FD_ISSET(i , &read_fds)){

				if( i == listener){

					if((new_fd = aceptar_conexion(i)) == -1){
						log_info(logger_estado, "No se pudo aceptar una nueva conexion");

					}else{

						printf("\n>Se conecto un cliente\n\n");
						log_info(logger_estado, "Se conecto un cliente\n");

						FD_SET(new_fd, &master);

						if(new_fd > fd_max){
							fd_max = new_fd;
						}

					}


				}else if(i == 0){
//TODO parsear linea por requests!
					leer_consola();

				}else{

					request nueva_request = recibir_request(i);

//TODO Esta ranciada de la desconexion y los errores

					if(nueva_request->cod_op == DESCONEXION){

						FD_CLR(i, &master);
						close(i);
						log_info(logger_estado, "Se desconecto un cliente.");

						printf("\n>Se desconecto un cliente\n");

					}else{

						log_info(logger, "Se recibio una nueva REQUEST.");

						printf("\n>>>>>>>>>>>>>>>>>>>>>>>>NUEVA REQUEST<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

						trabajar_request(nueva_request, i);

						printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>FIN REQUEST<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");


					}

					liberar_request(nueva_request);
					log_info(logger, "Se termino de trabajar la REQUEST.\n");


				}

			}

			if(desconexion_pool) break;

		}

		if(desconexion_pool) break;

	}

	printf(">Se va a cerrar la memoria\n");



	//CIERRO TODOS LOS CLIENTES
	for(i = 1; i <= fd_max; i++){


		if(FD_ISSET(i, &master)){

			FD_CLR(i, &master);
			close(i);

		}


	}

	abortar_hilos();

	log_info(logger_estado, "Se va a cerrar la memoria"); //ESTO ROMPIA , NANI?

	liberar_memoria();

	list_destroy_and_destroy_elements(ip_seeds, free);
	list_destroy_and_destroy_elements(puerto_seeds, free);

	free(ip_lfs);
	free(ip_escucha);

	printf("Termino el programa memoria\n");

	log_info(logger_estado, "Termino el programa memoria\n");

	liberar_logger();

	return EXIT_SUCCESS;
}

void abortar_hilos(void){

//	pthread_mutex_lock(&mutex_journal);

	pthread_cancel(journal_thread);
//	pthread_mutex_unlock(&mutex_journal);

//	pthread_mutex_lock(&mutex_gossip);
	pthread_cancel(gossip_thread);
//	pthread_mutex_lock(&mutex_gossip);

	//pthread_cancel(inotify_thread);

	printf("Se realiza un JOURNAL para cerrar la memoria correctamente\n");
	log_info(logger_estado, "Se realiza un JOURNAL para cerrar la memoria correctamente");


	if(conexion_lissandra){
		printf("Se realiza un JOURNAL para cerrar la memoria correctamente\n");
		log_info(logger_estado, "Se realiza un JOURNAL para cerrar la memoria correctamente");

		realizar_journal();

		close(socket_lissandra);

	}


	pthread_mutex_destroy(&mutex_journal);
	pthread_mutex_destroy(&mutex_gossip);

	liberar_inotify(arg_inotify);

	log_info(logger_estado, "Se liberan los hilos de ejecucion");

}

void inicializar_hilos(void){

	int err;
	//Atributos
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_mutex_init(&mutex_journal, NULL);
	pthread_mutex_init(&mutex_gossip, NULL);

	err = pthread_create(&journal_thread, &attr, auto_journal, NULL );

	if(err){

		perror("pthread_create journal");
		exit(1);

	}

	err = pthread_create(&gossip_thread, &attr, auto_gossip, NULL);

	if(err){

		perror("pthread_create gossip");
		exit(1);

	}

	err = pthread_create(&inotify_thread, &attr, realizar_inotify, arg_inotify);

	if(err){

		perror("pthread_create inotify");

		exit(1);
	}

	pthread_attr_destroy(&attr);

	log_info(logger_estado, "Se inicializaron hilos para el Auto-Gossiping, Auto-Journal e Inotify");

}
