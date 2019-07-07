#include"main.h"

int main(){
	crearYObtenerDatos();
//	creacion_bitmap();
//	setAllEstados(LIBRE);

	dato_t* dato1 = crearDato(11, 22, "ESPERT");
	dato_t* dato2 = crearDato(666, 777, "Duko");
	dato_t* dato3 = crearDato(12345, 44444, "Peronazo");
	dato_t* dato4 = crearDato(999, 666, "Messi");

	char* datoStringueado3 = datoEnFormatoBloque(dato3);
	char* datoStringueado2 = datoEnFormatoBloque(dato2);
	char* datoStringueado1 = datoEnFormatoBloque(dato1);
	char* datoStringueado4 = datoEnFormatoBloque(dato4);


//	setEstado(0, OCUPADO);
//	setEstado(1, OCUPADO);

//	crear_archivos_particiones("Tabla_A", 2);

//	getAllEstados();

//	llenarBloque(datoStringueado2, 0);
//	llenarBloque(datoStringueado2, 0);
//	llenarBloque(datoStringueado3, 0);
//	llenarBloque(datoStringueado2, 1);
	leer_Particiones("Tabla_A");

//	asignarBloqueVacioAParticion("Tabla_A", 0, 2);
//	llenarBloque(datoStringueado4, 10);


//	int lol = cantidadDeBloques(0, "Tabla_A");
//	printf("cantidad = %i\n", lol);

/*	ubicadorBloque lol = ubicadorDelBloque(1);
	printf("---- ESTAMOS EN CONSOLA ----\n");
	printf("%s\n", lol.nombreTabla);
	printf("%i\n", lol.particion);
*/
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


