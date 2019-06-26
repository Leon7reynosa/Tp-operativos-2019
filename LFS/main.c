#include"main.h"

int main(){
	crearYObtenerDatos();
//	creacion_bitmap();
//	setAllEstados(LIBRE);

	dato_t* dato2 = crearDato(666, 777, "Modo Diable");
	char* datoStringueado2 = datoEnFormatoBloque(dato2);
/*
	ubicadorBloque auxiliar;

	auxiliar = ubicadorDelBloque(1);
	printf("#######Datos obtenidos########\n");
	printf("%s\n", auxiliar.nombreTabla);
	printf("%i\n", auxiliar.particion);
*/
	char* pathNuevoParaTemporal = obtenerPathParaTemporalEnLaTabla("Tabla_A");


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


