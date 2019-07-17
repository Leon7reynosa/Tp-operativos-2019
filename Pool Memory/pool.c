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
	obtener_datos_config();
//TODO HANDSHAKE CON LISSANDRA (en archivo config por ahora)
	realizar_handshake();

	tamanio_dato = tamanio_value + sizeof(u_int16_t) + sizeof(time_t);

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
					leer_consola();

				}else{
					printf("\n//////////////////////////////////////////////////////// NUEVA REQUEST c: ////////////////////////////////////////////////\n");
					request nueva_request = recibir_request(i);

					printf("codigo_ %d\n" , nueva_request->cod_op);

//TODO Esta ranciada de la desconexion y los errores

					if(nueva_request->cod_op == DESCONEXION){

						FD_CLR(i, &master);
						close(i);
						printf("Desconecte todo re piola amigo, no la agites mas!\n");

					}else{

					pthread_mutex_lock(&mutex_journal);
					//DOUBLE FREE CORRUPTION, OJO EN EL ENVIAR_REQUEST, ESTOY GENERANDO DENUEVO UNA REQUEST Y LA LIBERO AHI! ojo
					trabajar_request(nueva_request, i);
					pthread_mutex_unlock(&mutex_journal);
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

bool leer_consola(void){

	int tamanio_buffer = 256;

	char* buffer = malloc(tamanio_buffer + 1);

	fgets(buffer, tamanio_buffer, stdin);

	*(buffer + tamanio_buffer) = '\0';

	if(string_length(buffer) + 1 >= tamanio_buffer){
		printf("Pusiste 200 millones de caracteres en la consola\n");
	}

	buffer = realloc(buffer, string_length(buffer) + 1);

	char** tokens = string_split(buffer, " ");

	free(buffer);

	string_to_upper(tokens[0]);

	if(string_equals_ignore_case(tokens[0], "SELECT")){

		parsear_request(SELECT, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "INSERT")){

		parsear_request(INSERT, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "CREATE")){

		parsear_request(CREATE, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "DESCRIBE")){

		parsear_request(DESCRIBE, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "DROP")){

		//parsear_request(DROP, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "JOURNAL")){

		//parsear_request(JOURNAL, tokens);

	}
	else if(string_equals_ignore_case(tokens[0], "EXIT")){

		liberar_puntero_doble(tokens);

		return true;

	}else{
		printf("No es valido lo ingresado\n");
	}

	liberar_puntero_doble(tokens);


	return false;
}

void parsear_request(cod_operacion operacion, char** tokens){

	int cantidad_argumentos = obtener_cantidad_argumentos(tokens);

	char* tabla;
	u_int16_t key;
	time_t timestamp;

	select_t dato_select;
	Dato dato;

	insert dato_insert;


	switch(operacion){

		case SELECT:

			if(cantidad_argumentos == 2){

				tabla = tokens[1];
				key = atoi(tokens[2]);       //VER ESTO

				dato_select = crear_dato_select(tabla, key);

				dato = request_select(dato_select);

				liberar_dato(dato);

			}else{

				printf("Cantidad de argumentos invalido\n");

			}

			break;

		default:
			break;


	}


}

void liberar_puntero_doble(char** puntero_doble){

	int i = 0;
	while(*(puntero_doble + i) != NULL){

		free(*(puntero_doble + i));
		i++;

	}

	free(puntero_doble);

}

int obtener_cantidad_argumentos(char** tokens){

	int i = 1;
	int cantidad = 0;

	while(*(tokens + i) != NULL){
		cantidad++;
		i++;
	}

	return cantidad;
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
