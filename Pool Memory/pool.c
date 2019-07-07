/*
 * pool.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "pool.h"

int main (void){

	////////////////////////////////////INICIALIZACIONES/////////////////////////////////
	obtener_datos_config();
//TODO HANDSHAKE CON LISSANDRA
	tamanio_value = 50;
	tamanio_dato = tamanio_value + sizeof(u_int16_t) + sizeof(time_t);

	t_list* tablas = list_create();
	list_add(tablas, "TABLA_A");

	inicializar_memoria(tamanio, tamanio_value, tamanio_dato, tablas); //TODO ARREGLAR ESTA FUNCION UN POCO

	//////////////////////////////////////////MAIN///////////////////////////////////////

	fd_set master;
	fd_set read_fds;

	int fd_max;
	int new_fd;
	int listener;
	int i;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	if(( listener = iniciar_servidor("127.0.0.1" , puerto_escucha) ) == -1){

		perror("socket");
		exit(-1);
	}

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
					char* buffer = malloc(128);
					fgets(buffer, 128, stdin);
					printf("Lei de consola: %s\n", buffer);
					free(buffer);

				}else{
					printf("\n//////////////////////////////////////////////////////// NUEVA REQUEST c: ////////////////////////////////////////////////\n");
					request nueva_request = recibir_request(i);

//TODO Esta ranciada de la desconexion y los errores

					if(nueva_request->cod_op == DESCONEXION){

						FD_CLR(i, &master);
						close(i);
						printf("Desconecte todo re piola amigo, no la agites mas!\n");

					}else{

					trabajar_request(nueva_request, i);
					}
//TODO OJO AL LIBERAR UNA DESCONEXION, NO PUEDO HACER FREE DE NULL!!!!!!
					liberar_request(nueva_request);
					printf("\n//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
				}


			}



		}

	}

	printf("Termino el programa memoria\n");


	return EXIT_SUCCESS;
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
