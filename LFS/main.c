#include"main.h"

int main(){

	////////////////////////////INICIALIZACIONES/////////////////////////
	creacion_del_config_fileSystem();

	obtener_datos_config();
	obtener_datos_metadata();

	inicializar_memtable();

	//creacion_bitmap();

	/////////////////////////////////////////////////////////////////////

	set_all_estados(LIBRE);

	crear_archivos_particiones("Tabla_A" , 2);


	pruebas();

//	Particion particion = leer_particion("/home/utnso/Escritorio/TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/Tablas/Tabla_A/0.bin");
//
//	mostrar_particion(particion);
}

void pruebas(){

	prueba_dump();

	compactar("Tabla_A");


}

void prueba_dump(){
	char* path_particion = obtenerPath_ParticionTabla("Tabla_A" , 0);

	dato_t* dato_prueba_uno = crear_dato( 10, "fede maincra" , 2001);
	dato_t* dato_prueba_dos = crear_dato( 6, "lalo" , 200);
	dato_t* dato_prueba_tres = crear_dato( 9, "chino LoL" , 1999);
	dato_t* dato_prueba_cuatro = crear_dato( 7 , "leon rocket" , 1998 );

	printf("Voy a ingresar los datos a la memtable\n");
	ingresar_a_memtable(dato_prueba_uno, "Tabla_A");
	ingresar_a_memtable(dato_prueba_dos, "Tabla_A");
	ingresar_a_memtable(dato_prueba_tres, "Tabla_A");
	ingresar_a_memtable(dato_prueba_cuatro, "Tabla_A");

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


