#include"main.h"
#include"inotify_prueba.h"

int main(int argc , char* argv[]){

	printf("HOLA BEBE\n");

	////////////////////////////INICIALIZACIONES////////////////////////



	creacion_del_config_fileSystem();

	obtener_datos_config();

	printf("PASE -1\n");
	obtener_datos_metadata();

	printf("PASE 0 \n");

	inicializar_loggers();

	printf("PASE 1\n");

	inicializar_memtable(); //Inicializa la memtable como diccionario junto con su lock rw

	printf("PASE 2\n");

	inicializar_memorias_conectadas(); //Inicializa una lista de memorias

	printf("PASE 3\n");

	inicializar_compactador();

	printf("PASE 4\n");


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



	pthread_join(hilo_consola , NULL);

	//mandaron exit por la consola


	pthread_cancel(administrador_hilos);

	liberar_conexiones();

	liberar_dump();

	liberar_memtable();

	liberar_compactadores();

	destruir_loggers();

	liberar_globales();


	printf("terminaron los hilos\n");

	printf("Termine bien\n");

	return EXIT_SUCCESS;


}

void main_inotify(){
	pthread_t inotify_config;

	int error_pthread;

	obtener_datos_config();
	obtener_datos_metadata();

	metadata_t* metadata_actual = obtener_metadata("Tabla_A");
	printf("Metadata inicial: \n");
	printf("compactacion = %i\n\n\n", metadata_actual->compactacion);

	free(metadata_actual);

	printf("Empiezo con los hilos ;)\n");
	error_pthread = pthread_create(&inotify_config, NULL , notificar_cambio_config,(void *) "Tabla_A");

	if(error_pthread != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	char* buffer_io = readline("Mete un valor a la compactacion: ");

	char* path_config = obtener_path_metadata_de_tabla("Tabla_A");

	t_config* metadata_config = config_create(path_config);

	config_set_value(metadata_config, "COMPACTION_TIME", buffer_io);

	printf("VOY A GUARDAR EN EL CONFIG\n");
	config_save(metadata_config);
	printf("GUARDE EN EL CONFIG\n");

	config_destroy(metadata_config);

	free(path_config);
	free(buffer_io);

	pthread_join(inotify_config, NULL);
	printf("Termino todo bien\n");


}

void pruebas(){

	prueba_dump();

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

void prueba_dump(){
	char* path_particion = obtenerPath_ParticionTabla("TABLA_A" , 0);

	dato_t* dato_prueba_uno = crear_dato( 15, "alan el desaparecido" , 8000); //si
	dato_t* dato_prueba_dos = crear_dato( 6, "thais alta zorra" , 10600); //sii
	dato_t* dato_prueba_tres = crear_dato( 9, "chino LoL" , 300); //no
	dato_t* dato_prueba_cuatro = crear_dato( 7 , "leon vergadura" , 8000 ); //si
	dato_t* dato_prueba_cinco = crear_dato( 10 , "fede el recursante" ,  10000 ); //SI
	dato_t* dato_prueba_seis = crear_dato( 10 , "fede el maincra king" ,  2000 ); //NO

	printf("Voy a ingresar los datos a la memtable\n");
	ingresar_a_memtable(dato_prueba_uno, "TABLA_A");
	ingresar_a_memtable(dato_prueba_dos, "TABLA_A");
	ingresar_a_memtable(dato_prueba_tres, "TABLA_A");
	ingresar_a_memtable(dato_prueba_cuatro, "TABLA_A");
	ingresar_a_memtable(dato_prueba_cinco, "TABLA_A");
	ingresar_a_memtable(dato_prueba_seis, "TABLA_A");

	printf("\nVoy a realizar el dump\n");
	realizar_dump();

	printf("\nTermino el DUMP\n");
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

