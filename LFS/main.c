#include"main.h"

int main(){
	crearYObtenerDatos();
//	creacion_bitmap();
//	setAllEstados(LIBRE);

	dato_t* dato1 = crearDato(11, 22, "ESPERT 2019");
	dato_t* dato2 = crearDato(666, 777, "Modo Diable");
	dato_t* dato3 = crearDato(1234, 4444, "Peron vive");
	dato_t* dato4 = crearDato(999, 666, "El diablo es gay");

	char* datoStringueado3 = datoEnFormatoBloque(dato3);
	char* datoStringueado2 = datoEnFormatoBloque(dato2);
	char* datoStringueado1 = datoEnFormatoBloque(dato1);
	char* datoStringueado4 = datoEnFormatoBloque(dato4);

//	crear_archivos_particiones("Tabla_A", 3);


//	llenarBloque(datoStringueado1, 80);
//	llenarBloque(datoStringueado2, 81);
//	llenarBloque(datoStringueado3, 82);

//	leerBloque(0);
//	leerBloque(1);
//	leerBloque(10);

	asignarBloqueVacioAParticion("Tabla_A", 0, 80);
	asignarBloqueVacioAParticion("Tabla_A", 1, 81);
	asignarBloqueVacioAParticion("Tabla_A", 2, 82);

//	int lol = cantidadDeBloques(0, "Tabla_A");
//	printf("cantidad = %i\n", lol);

//	ubicadorBloque lol = ubicadorDelBloque(11);
//	printf("---- ESTAMOS EN CONSOLA ----\n");
//	printf("%s\n", lol.nombreTabla);
//	printf("%i\n", lol.particion);



//	actualizarParticionConBloque(0);
//	actualizarParticionConBloque(1);

	leer_Particiones("Tabla_A");


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


