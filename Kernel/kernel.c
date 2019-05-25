/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

int main (){
	int conexion;
	int puerto;
	char* ip;

	obtener_puerto_ip(&puerto,&ip);

	conexion = conectar_servidor(ip,puerto);

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

	return EXIT_SUCCESS;
}

