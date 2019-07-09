#include"main.h"

int main(){

	////////////////////////////INICIALIZACIONES/////////////////////////
	creacion_del_config_fileSystem();

	obtener_datos_config();
	obtener_datos_metadata();

	inicializar_memtable();

//	set_all_estados(LIBRE);

	/////////////////////////////////////////////////////////////////////

	pruebas();

}

void pruebas(){
	//get_all_estados();
	//crear_archivos_particiones("Tabla_A" , 4 );

	char* path_particion = obtenerPath_ParticionTabla("Tabla_A" , 0);
	dato_t* dato_prueba = crear_dato( 7, "chino cochino" , 1998);


	ingresar_a_memtable(dato_prueba, "Tabla_A");

	realizar_dump();
/*

	Particion particion = leer_particion(path_particion);

	printf("bytes = %i\n", particion->size);

	int* bloque = (int *) list_get(particion->bloques, 0);

	printf("bloque = %i\n", *bloque );

	printf("\nUSO LA FUNCION MOSTRAR PARTICION\n");
	mostrar_particion(particion);

	liberar_particion(particion);
*/

	//cargar_a_particion(path_particion, dato_prueba);



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


