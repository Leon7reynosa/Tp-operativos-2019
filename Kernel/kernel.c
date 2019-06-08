/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

int main (int argc , char* argv[]){

	inicializar_cola_exec();


	char* linea_request;
	int tiempo;
	int particiones;
	char* nombre_tabla = string_new();
	char* consistencia = string_new();


	conexion_lissandra = conectar_servidor("127.0.0.1" , 4446);

	for (int i = 1; i < argc ; i++){

		FILE* archivo_lql = fopen(argv[i] , "r");

		parsear_LQL(archivo_lql);

		fclose(archivo_lql);

	}

	//para mostrar lo que hay en la cola_exec
	for(int j = 0; j < cola_exec->elements_count ; j++){

		linea_request = list_get(cola_exec, j);

		printf("request: %s\n" , linea_request);

	}

	linea_request = (char*) list_get(cola_exec, 0);

	printf("holitas\n");

	mandar_request(linea_request, conexion_lissandra);







//		mandar_request(linea_request);
/*
		obtener_parametros_create(linea_request, nombre_tabla, consistencia, &particiones, &tiempo);

		printf("----------------FUERA DE LA FUNCION ------------\n");
		printf("nombre tabla: %s\n", nombre_tabla);
		printf("consistencia: %s\n", consistencia);
		printf("particiones: %i\n", particiones);
		printf("tiempo: %i\n", tiempo);
*
		/*ahora que tenemos la linea del request del lql(solo la primera linea)
		 * hay que hacer que identifique que request es, y ejectute con los parametro que tenga
		 * (el request hay que pedirselo a memoria)
		 */


return 0;
}




