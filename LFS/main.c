#include"main.h"

int main(){
	crearYObtenerDatos();
//	creacion_bitmap();
//	setAllEstados(LIBRE);

	dato_t* dato1 = crearDato(11, 22, "ESPERT");
	dato_t* dato2 = crearDato(666, 777, "ModoDiable");
	dato_t* dato3 = crearDato(1234, 4444, "Peronazo");
	dato_t* dato4 = crearDato(999, 666, "Messi");

	char* datoStringueado3 = datoEnFormatoBloque(dato3);
	char* datoStringueado2 = datoEnFormatoBloque(dato2);
	char* datoStringueado1 = datoEnFormatoBloque(dato1);
	char* datoStringueado4 = datoEnFormatoBloque(dato4);

//	crear_archivos_particiones("Tabla_A", 2);


	llenarBloque(datoStringueado1, 0);
//	llenarBloque(datoStringueado1, 0);
//	llenarBloque(datoStringueado2, 1);
//	asignarBloqueVacioAParticion("Tabla_A", 0, 80);
//	llenarBloque(datoStringueado4, 10);

//	leerBloque(80);
//	leerBloque(81);
//	leerBloque(82);


//	asignarBloqueVacioAParticion("Tabla_A", 0, 10);
//	asignarBloqueVacioAParticion("Tabla_A", 1, 11);

//	int lol = cantidadDeBloques(0, "Tabla_A");
//	printf("cantidad = %i\n", lol);

//	ubicadorBloque lol = ubicadorDelBloque(10);
//	printf("---- ESTAMOS EN CONSOLA ----\n");
//	printf("%s\n", lol.nombreTabla);
//	printf("%i\n", lol.particion);



//	actualizarParticionConBloque(0);
//	actualizarParticionConBloque(1);
//	actualizarParticionConBloque(10);

//	leer_Particiones("Tabla_A");

	//SI LEO DESPUES DEL CREAR, SIN LLENAR BLOQUES, SE DUPLICAN LAS WEAS !

//	leer_Particion("Tabla_A", 0);


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


