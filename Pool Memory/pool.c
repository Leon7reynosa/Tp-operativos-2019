/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"

int main (void){

	creacion_del_config();

	////////////////////////////////////INICIALIZACIONES/////////////////////////////////
	ip_escucha = obtener_ip_local();

	obtener_datos_config();

//TODO HANDSHAKE CON LISSANDRA (en archivo config por ahora)
	realizar_handshake();

	tamanio_dato = tamanio_value + sizeof(u_int16_t) + sizeof(time_t);

	desconexion_pool = false;

	inicializar_memoria(tamanio, tamanio_value, tamanio_dato); //TODO ARREGLAR ESTA FUNCION UN POCO

	inicializar_hilos();

	//////////////////////////////////////////MAIN///////////////////////////////////////

	fd_set master;
	fd_set read_fds;

	int fd_max;
	int new_fd;
	int listener;
	int i;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	if(( listener = iniciar_servidor(ip_escucha, puerto_escucha) ) == -1){

		perror("socket");
		exit(-1);
	}

	free(ip_escucha);

	FD_SET(0, &master);
	FD_SET(listener, &master);
	fd_max = listener;

	for ( ; ; ){

		read_fds = master;

		if(select(fd_max +1 , &read_fds, NULL, NULL, NULL) == -1){
			perror("select.");
			exit(1);
		}

		for(i = 0; i<= fd_max ; i++){

			if(FD_ISSET(i , &read_fds)){

				if( i == listener){

					if((new_fd = aceptar_conexion(i)) == -1){
						perror("Error al aceptar conexion");

					}else{

						FD_SET(new_fd, &master);

						if(new_fd > fd_max){
							fd_max = new_fd;
						}

					}


				}else if(i == 0){
//TODO parsear linea por requests!
					desconexion_pool = leer_consola();

				}else{
					printf("\n//////////////////////////////////////////////////////// NUEVA REQUEST c: ////////////////////////////////////////////////\n");
					request nueva_request = recibir_request(i);

//TODO Esta ranciada de la desconexion y los errores

					if(nueva_request->cod_op == DESCONEXION){

						FD_CLR(i, &master);
						close(i);
						printf("Desconecte todo re piola amigo, no la agites mas!\n");

					}else{

						printf("no da\n");

						trabajar_request(nueva_request, i);

					}

					liberar_request(nueva_request);
					printf("\n//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
				}


			}

		}

		if(desconexion_pool){
			break;
		}

	}

	//CIERRO TODOS LOS CLIENTES
	for(i = 1; i <= fd_max; i++){


		if(FD_ISSET(i, &master)){

			FD_CLR(i, &master);
			close(i);

		}


	}


	close(socket_lissandra);

	abortar_hilos();

	liberar_memoria();

	list_destroy_and_destroy_elements(ip_seeds, free);
	list_destroy_and_destroy_elements(puerto_seeds, free);

	free(ip_lfs);

	printf("Termino el programa memoria\n");


	return EXIT_SUCCESS;
}

void abortar_hilos(void){

	pthread_mutex_lock(&mutex_journal);
	pthread_cancel(journal_thread);
	pthread_mutex_unlock(&mutex_journal);

	pthread_mutex_lock(&mutex_gossip);
	pthread_cancel(gossip_thread);
	pthread_mutex_lock(&mutex_gossip);

	pthread_mutex_destroy(&mutex_journal);
	pthread_mutex_destroy(&mutex_gossip);

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

	pthread_attr_destroy(&attr);

}

//void pruebas(void){
//
//	tamanio_value = 49;
//	tamanio_dato = tamanio_value + sizeof(u_int16_t) + sizeof(time_t);
//
//	printf("TAMANIO DEL VALUE: %i\n", tamanio_value);
//	printf("TAMANIO DEL DATO %i\n", tamanio_dato);
//
//	char* tabla_a = "Tabla_A";
//
//	obtener_datos_config();
//
//	t_list* lista_tablas = list_create();
//	list_add(lista_tablas, (void*)tabla_a);
//
//	inicializar_memoria(tamanio, tamanio_value, tamanio_dato, lista_tablas);
//
//	int i;
//	printf("Cantidad de paginas: %i\n", memoria->paginas->elements_count);
//
//	for( i = 0; i < 35; i++){
//			printf("/////////////////// INSERT n°%i /////////////////////\n", i + 1);
//			request_insert("Tabla_A", i + 1, "Hola, soy alguna prueba.");
//	}
//
//
//	printf("Pruebas finalizadas con exito!\n");
//}
