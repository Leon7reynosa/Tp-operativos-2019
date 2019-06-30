#include"main.h"

int main(){
	crearYObtenerDatos();
//	creacion_bitmap();
//	setAllEstados(LIBRE);

	dato_t* dato1 = crearDato(11, 22, "ESPERT 2019");
	dato_t* dato2 = crearDato(666, 777, "Modo Diable");
	dato_t* dato3 = crearDato(1234, 4444, "Peron vive");

	char* datoStringueado3 = datoEnFormatoBloque(dato3);
	char* datoStringueado2 = datoEnFormatoBloque(dato2);
	char* datoStringueado1 = datoEnFormatoBloque(dato1);

//	crear_archivos_particiones("Tabla_A", 3);


//	llenarBloque(datoStringueado1, 0);
//	llenarBloque(datoStringueado2, 1);
//	llenarBloque(datoStringueado3, 2);

//	leerBloque(0);
//	leerBloque(1);
//	leerBloque(2);

//	actualizarParticion(2);

	leer_Particiones("Tabla_A");
	int i = 0;
	while(i < 3){
		printf("Cantidad de bloques particion %i: %i\n", cantidadDeBLoques(i, "Tabla_A"));
		i++;
	}

	asignarBloqueAParticion("Tabla_A", 0, 90);

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


