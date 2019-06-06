/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

//para ejecutar --> click derecho en carpeta Kenel -> run configurations -> Arguments -> y agregar "lql.txt"
int main (int argc , char* argv[]){

	char* linea_request;
	int tiempo, particiones, lol;
	char* nombre_tabla = string_new();
	char* consistencia = string_new();


	for (int i = 1; i < argc ; i++){


		linea_request = parsear_LQL(argv[i]);

//		mandar_request(linea_request);

		obtener_parametros_create(linea_request, nombre_tabla, consistencia, &particiones, &tiempo);

		printf("----------------FUERA DE LA FUNCION ------------\n");
		printf("nombre tabla: %s\n", nombre_tabla);
		printf("consistencia: %s\n", consistencia);
		printf("particiones: %i\n", particiones);
		printf("tiempo: %i\n", tiempo);

		/*ahora que tenemos la linea del request del lql(solo la primera linea)
		 * hay que hacer que identifique que request es, y ejectute con los parametro que tenga
		 * (el request hay que pedirselo a memoria)
		 */


	}


return 0;
}




////////////////////// FUNCIONES NUEVAS///////////////////////////////


//Por ahora solo lee una linea del archivo.
char* parsear_LQL(char* path_archivo){

	FILE* archivo_lql = fopen(path_archivo, "r");

	int i = 0;

	char* linea_leida = string_new();

	char caracter;
	char* caracter_temp;

	caracter = fgetc(archivo_lql);;

	while(caracter != '\n'){

		caracter_temp = string_from_format("%c", caracter);

		string_append(&linea_leida, caracter_temp);

		caracter = fgetc(archivo_lql);


	}

	printf("Linea leida: %s\n" , linea_leida);

	fclose(archivo_lql);

	return linea_leida;
}

void mandar_request(char* request_lql, int conexion){

	int cod_request;
	char* nombre_tabla = string_new();
	u_int16_t key;
	char* value = string_new();
	char* consistencia = string_new();
	int tiempo_compactacion, particiones;
	time_t timestamp;


	cod_request = identificar_request(request_lql);

	switch(cod_request){
		case SELECT:
			obtener_parametros_select(request_lql, nombre_tabla, &key);
			enviar_select(conexion, nombre_tabla, key);
			break;
		case INSERT:
			obtener_parametros_insert(request_lql, nombre_tabla, &key, value, &timestamp);
			enviar_insert(conexion, nombre_tabla, key, value, timestamp);
			break;
		case CREATE:
			obtener_parametros_create(request_lql, nombre_tabla, consistencia, &particiones, &tiempo_compactacion);
			enviar_create(conexion, nombre_tabla, consistencia, particiones, tiempo_compactacion);
			break;
		case DROP:
			obtener_parametros_drop(request_lql, nombre_tabla);
			enviar_drop(conexion, nombre_tabla);
			break;
		case DESCRIBE:
			obtener_parametros_describe_de_una_tabla(request_lql, nombre_tabla);
			enviar_describe(conexion, nombre_tabla);
			break;
		default:
			printf("LA REQUEST NO ES VALIDA\n");
			exit(-1);
	}

}

int identificar_request(char* request_lql){

	string_to_upper(request_lql);

	if(string_starts_with(request_lql , "SELECT")){

		return SELECT;

	}else if(string_starts_with(request_lql , "INSERT")){

		return INSERT;

	}else if(string_starts_with(request_lql , "DESCRIBE")){

		return DESCRIBE;

	}else if(string_starts_with(request_lql , "CREATE")){

		return CREATE;

	}else if(string_starts_with(request_lql , "DROP")){

		return DROP;

	}else{
		return -1;
	}


}

void obtener_parametros_select(char* linea_request, char* nombre_tabla, u_int16_t key){
	char* funcion = string_new();

	sscanf(linea_request, "%s %s %d", funcion, nombre_tabla, key);

}

void obtener_parametros_insert(char* linea_request, char* nombre_tabla, u_int16_t key, char* value, time_t timestamp){
	char* funcion = string_new();

	sscanf(linea_request, "%s %s %i %s %i", funcion, nombre_tabla, key, value, timestamp);
}
void obtener_parametros_insert_sin_timestamp(char* linea_request, char* nombre_tabla, u_int16_t key, char* value){
	char* funcion = string_new();

	sscanf(linea_request, "%s %s %i %s %i", funcion, nombre_tabla, key, value);
}

void obtener_parametros_create(char* linea_request, char* nombre_tabla, char* criterio, int numero_particiones, int tiempo_compactacion){
	char* funcion = string_new();

	sscanf(linea_request, "%s %s %s %i %i %i", funcion, nombre_tabla, criterio, numero_particiones, tiempo_compactacion);

}

void obtener_parametros_describe_de_una_tabla(char* linea_request, char* nombre_tabla){
	char* funcion = string_new();

	sscanf(linea_request, "%s %s", funcion, nombre_tabla);
}

void obtener_parametros_describe(char* linea_request){
	char* funcion = string_new();

	sscanf(linea_request, "%s", funcion);
}
void obtener_parametros_drop(char* linea_request, char* nombre_tabla){
	char* funcion = string_new();

	sscanf(linea_request, "%s %s", funcion, nombre_tabla);
}





/*	while(1){
		mandar_mensaje(conexion);
	}
	config_destroy(g_config);
*/

/*
	int menu;
	char* nombre_tabla;
	int key;
	printf("Bienvenido al kernel\n¿Que desea hacer?\n\n");
	printf("1. SELECT\n");
	printf("2. INSERT\n");
	printf("3. CREATE\n");
	printf("4. DESCRIBE\n");
	printf("5. DROP\n");
	printf("6. JOURNAL\n");
	printf("7. ADD\n");
	printf("8. RUN\n");
	printf("9. METRICS\n");
	printf("10. -- EXIT --\n");

	(void*)scanf("%i", menu);

	while(menu >= 1 && menu < 10){
		switch(menu){
			case 1:
				printf("Ingrese nombre de tabla: ");
				(void*)scanf("%s", nombre_tabla);
				printf("Ingrese la key: ");
				(void*)scanf("%i", key);
				select(nombre_tabla, key);
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				break;
		}

		printf("Bienvenido al kernel\n¿Que desea hacer?\n\n");
			printf("1. SELECT\n");
			printf("2. INSERT\n");
			printf("3. CREATE\n");
			printf("4. DESCRIBE\n");
			printf("5. DROP\n");
			printf("6. JOURNAL\n");
			printf("7. ADD\n");
			printf("8. RUN\n");
			printf("9. METRICS\n");
			printf("10. -- EXIT --\n");

			scanf("%i", menu);
	}
*/


