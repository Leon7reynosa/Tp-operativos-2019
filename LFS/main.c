#include"main.h"

int main(){
	crearYObtenerDatos();
//	creacion_bitmap();
//	setAllEstados(LIBRE);

	dato_t* dato2 = crearDato(666, 777, "Modo Diable");
	char* datoStringueado2 = datoEnFormatoBloque(dato2);
//	crear_archivos_particiones("Tabla_A" , 4);
//	getAllEstados();
/*
	int ficherin = open("/home/utnso/Escritorio/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/Bloques/0.bin", O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
	int posicion_fichero = lseek(ficherin, 0, SEEK_END);

	printf("\nPUNTERO DEL BLOQUE = %i\n", posicion_fichero);
	int tamanio = string_length(datoStringueado2);
	int contador = 0;
	while(contador < tamanio){
		write(ficherin, datoStringueado2 + contador,  sizeof(char));
		contador++;
	}
	posicion_fichero = lseek(ficherin, 0, SEEK_END);

	printf("\nPUNTERO DEL BLOQUE = %i\n", posicion_fichero);

	close(ficherin);
*/
	dato_t* dato3 = crearDato(123456, 7890, "Chino");
	dato_t* dato4 = crearDato(78901, 54321, "Lalo");

	char* _d2 = datoEnFormatoBloque(dato3);
	char* _d3 = datoEnFormatoBloque(dato4);

	llenarBloque(datoStringueado2, 0);
	getAllEstados();
	//llenarBloque(_d2, 0);
	//llenarBloque(_d3, 0);
//	leerBloque(0);
/*
	int pruebita = open("/home/utnso/Escritorio/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/pruebita.bin", O_CREAT |O_RDWR, S_IWUSR | S_IRUSR);
	int puntero = lseek(pruebita, 0, SEEK_END);

	printf("\n\nEl puntero esta en la posicion %i\n", puntero);

	write(pruebita, "NANI?", 5);

	puntero = lseek(pruebita, 0, SEEK_END);
	printf("\n\nEl puntero esta en la posicion %i\n", puntero);
	close(pruebita);
*/
	//VER QUE PASA CUANDO COMPLETAMOS EL BLOQUE, VER CONDICION WHILE "AGREGAR EOF QUIZAS"

//	leer_Particiones("Tabla_A");

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


