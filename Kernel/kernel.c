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

	for (int i = 1; i < argc ; i++){


		linea_request = parsear_LQL(argv[i]);

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


