#include"main.h"
#include"inotify_prueba.h"

int main(int argc , char* argv[]){

	system("clear");

	printf("Empieza el programa Lissandra\n");

	////////////////////////////INICIALIZACIONES////////////////////////

	//creacion_del_config_fileSystem();

	//creacion_del_metadata_fileSystem();

	printf("es por el condifg\n");

	obtener_datos_config();

	printf("si\n");

	obtener_datos_metadata();

	printf("si\n");
	//set_all_estados(LIBRE);

	inicializar_loggers();

	printf("si\n");

	arg_inotify = crear_inotify();

	inicializar_memtable(); //Inicializa la memtable como diccionario junto con su lock rw
	printf("si\n");

	inicializar_hilo_inotify();

	inicializar_memorias_conectadas(); //Inicializa una lista de memorias

	inicializar_compactador();

	 /* Abre la carpeta TABLAS, y por cada tabla que haya, corre la  compactacion
	 * y agrega dicha tabla como thread_args* al diccionario compactador
	 *
	 * Sirve para cuando se desconecta y se vuelva a levantar, para chequear
	 * las tablas anteriores que existían*/

	inicializar_compactadores();


	 /* Crea un hilo para el dump y lo inicializa
	 * Ejecuta el "Ciclo Dump"*/

	inicializar_dump();

	creacion_bitmap();

	char* ip_escucha;

	ip_escucha = obtener_ip_address();
	int socket_servidor;
	socket_servidor = iniciar_servidor(ip_escucha, puerto_lfs);

	free(ip_escucha);



	///////////////////////////////MAIN////////////////////////////////////

	pthread_t administrador_hilos;
	pthread_t hilo_consola;
	int error_pthread;


	error_pthread = pthread_create(&hilo_consola, NULL , consola, NULL);

	if(error_pthread != 0){
		perror("pthread_create del hilo consola");
		exit(1);
	}


	 /* Creamos un hilo por cada conexion que recibamos de las memorias,
	 * la agrega a la lista de memorias conectadas y realizamos el handshake*/

	error_pthread = pthread_create(&administrador_hilos , NULL , administrar_conexiones_hilos , &socket_servidor);

	if(error_pthread != 0){
		perror("pthread_create del administrador de hilos");
		exit(1);
	}


	pthread_detach(administrador_hilos);

	printf("Se inicializo todo correctamente\n");

	pthread_join(hilo_consola , NULL);

	//mandaron exit por la consola

	pthread_cancel(administrador_hilos);

	liberar_conexiones();

	liberar_dump();

	liberar_hilo_inotify();

	liberar_memtable();

	liberar_compactadores();

	destruir_loggers();

	liberar_globales();


	printf("terminaron los hilos\n");

	printf("Termine bien\n");

	return EXIT_SUCCESS;


}

void pruebas(){


//	Particion temporal = leer_particion("/home/utnso/Escritorio/TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/Tablas/Tabla_A/0.tmp");
//
//	printf("TEMPORAL: \n");
//	mostrar_particion(temporal);

	compactar("TABLA_A");

//	get_all_estados();

//	for(int k = 0; k < 2; k++){
//		printf("\nParticion %i: \n", k);
//
//		char* path = obtenerPath_ParticionTabla("Tabla_A", k);
//
//		Particion particion = leer_particion(path);
//
//		mostrar_particion(particion);
//
//		liberar_particion(particion);
//
//		free(path);
//
//	}

}


void* conectar_memoria(){
	int puerto = 4445;
	char* ip = "127.0.0.1";
	int socket_cliente;
	int socket_sv = iniciar_servidor(ip, puerto);
	request request;

	socket_cliente = aceptar_conexion(socket_sv);

	request = recibir_request(socket_cliente);

	printf("codigo de operacion: %d\n" , request->cod_op);

	//faltaria mandar la respuesta

	close(socket_sv);
	return NULL;
}


void liberar_variables_globales(){

	free(ip_lfs);
	free(punto_montaje);
	free(magic_number);

}

