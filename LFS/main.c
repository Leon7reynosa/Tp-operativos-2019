#include"main.h"

int main(){

	////////////////////////////INICIALIZACIONES/////////////////////////
	creacion_del_config_fileSystem();

	obtener_datos_config();
	obtener_datos_metadata();

	inicializar_memtable();

	//creacion_bitmap();

	/////////////////////////////////////////////////////////////////////

	pruebas();

}

void pruebas(){
	//get_all_estados();
	//crear_archivos_particiones("Tabla_A" , 4 );

	char* path_particion = obtenerPath_ParticionTabla("Tabla_A" , 0);
	dato_t* dato_prueba = crear_dato( 10, "fede maincra" , 2001);


//	ingresar_a_memtable(dato_prueba, "Tabla_A");

//	realizar_dump();


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

	//char* aux = buscar_dato_bloque(10, 0, 4);

	//printf("Voy a buscar el dato: %s\n", aux);

//	dato_t* dato_prueba_dos = buscar_dato_en_particion(path_particion , 10);

//	printf("string: %s\n", dato_prueba_dos->value);

//	dato_t* dato_prueba_tres = buscar_dato_en_particion(path_particion , 8 );

//	printf("string: %s\n", dato_prueba_tres->value);

	request_drop("Tabla_B");


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


