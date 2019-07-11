
#include"compactador.h"

void* compactar(char* nombre_tabla){

	printf("Empieza la compactacion\n");
	//los comentarios entre parentesis son para ver donde se libera cada variable;

	printf("1) Transformo los tmp en tmpc\n");
	transformar_tmp_a_tmpc(nombre_tabla);

	t_list* datos_particiones;                                                                    // (2) Lista con los datos de los .bin antes de la compactacion

	t_list* datos_tmpc;																				// (3) Lista con los datos de los .tmpc

	t_list* datos_finales = list_create();																		// (4)  Lista con los datos de los .bin DESPUES de la particion

//	printf("2) Obtengo la metadata\n");
//	metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

//	printf("hola %i\n", metadata_tabla->particion);

	printf("3) Obtengo los datos de las particiones\n");
	datos_particiones = obtener_datos_particiones(nombre_tabla);

	printf("4) Obtengo los datos de los tmpc\n");
	datos_tmpc = obtener_datos_temporales(nombre_tabla);


	void _generar_lista_datos_finales(void* _dato_particion){

		char* dato_particiones = (char *)_dato_particion;

		t_list* dato_con_key_particular = filtrar_dato_por_key(dato_particiones, datos_tmpc);   // (1) Lista filtrada

		char* dato_timestamp_mayor = buscar_dato_actualizado(dato_particiones, dato_con_key_particular);

		list_add(datos_finales, dato_timestamp_mayor);

		list_destroy(dato_con_key_particular);                                                 // (1) Libero lista filtrada SIN liberar los elementos

	}

	printf("5) Por cada dato de las particiones, comparo y obtengo los datos finales\n");
	list_iterate(datos_particiones, _generar_lista_datos_finales);

	//ACA EMPIEZA EL PROCESO DE SACAR Y PONER
	//MUTEX?
	//HAY QUE BLOQUEAR PARA QUE NADIE OPERE SOBRE ESTA TABLA! SI NO SE PUDRE TODOO

	//LIBERO LAS PARTICIONES, DEBERIA QUEDARME NUEVOS .bin CON SIZE=0 y 1 BLOQUE

	printf("6) LIBERO LAS PARTICIONES ");

	metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

	for(int i = 0; i < metadata_tabla->particion; i++){

		char* path_particion = obtenerPath_ParticionTabla(nombre_tabla, i);

		liberar_bloques_particion(path_particion);

		free(path_particion);
	}

	void _funcion_loca2(void* _dato_final){

		char* dato_final = (char *)_dato_final;

		u_int16_t key = obtener_key_dato(dato_final);

		int particion_perteneciente = calcular_particion(metadata_tabla->particion, key );

		char* path_particion = obtenerPath_ParticionTabla(nombre_tabla, particion_perteneciente);  // (5)

		dato_t* dato_a_cargar = convertir_a_dato(dato_final);                                     // (6)

		cargar_a_particion(path_particion, dato_a_cargar );

		free(path_particion);                                                                    // (5)

		liberar_dato(dato_a_cargar);															 // (6)



	}

	printf("7) Por cada dato final, lo agrego a su correspondiente particion\n");
	//time_t inicio_de_bloqueo = time(NULL);
	list_iterate(datos_finales, _funcion_loca2);
	//time_t fin_de_bloqueo = time(NULL)
	//Sacar la diferencia entre estos dos para saber cuanto tiempo estuvo bloqueadoa la tabla

	printf("8) Borro los tmpc y sus bloques\n");
	liberar_tmpc(nombre_tabla);

	//Por aca habria que liberar el "mutex"
	//time_t fin_de_bloqueo = time(NULL)
	//Sacar la diferencia entre estos dos para saber cuanto tiempo estuvo bloqueadoa la tabla

	printf("9) Libero las estructuras administrativas\n");
	list_destroy_and_destroy_elements(datos_particiones, free);										//  (2) Libero los datos del .bin ANTES de la compactacion
	list_destroy_and_destroy_elements(datos_tmpc, free);											//  (3) Libero los datos del .tmpc

	list_destroy_and_destroy_elements(datos_finales, free);										// (4) Libero los datos del .bin DESPUES de la compactacion

	//TODO liberar_metadata(metadata_tabla); resuelto abajo por ahora, seguramente cambie la estructura metadata!
	free(metadata_tabla);

	return NULL;

}

u_int16_t obtener_key_dato(char* dato){

	char** dato_separado = string_split(dato, ";");

	u_int16_t key = (u_int16_t )atoi(dato_separado[1]);

	liberar_puntero_doble(dato_separado);

	return key;

}

time_t obtener_timestamp_dato(char* dato){

	char** dato_separado = string_split(dato, ";");

	time_t timestamp =  (time_t)atoi(dato_separado[0]);

	liberar_puntero_doble(dato_separado);

	return timestamp;
}


t_list* filtrar_dato_por_key(char* dato_particiones, t_list* datos_tmpc){

	u_int16_t key = obtener_key_dato(dato_particiones);

	t_list* datos_filtrados = list_create();

	bool _filtrar_por_key(void* _dato){

		char* dato = (char *)_dato;

		u_int16_t key_analizada = obtener_key_dato(dato);

		return key == key_analizada;

	}

	datos_filtrados = list_filter(datos_tmpc, _filtrar_por_key);

	return datos_filtrados;

}

//OJO, pasarle en dato_tmpc datos con la misma key al dato_particiones
char* buscar_dato_actualizado(char* dato_particion, t_list* dato_tmpc){


	int index_lista = 0;
	int length = string_length(dato_particion);

	char* dato_actualizado = malloc(length);
	memcpy(dato_actualizado, dato_particion, length + 1);

	time_t timestamp_mayor = obtener_timestamp_dato(dato_actualizado);

	char* dato_a_analizar;
	time_t timestamp_a_analizar;

	while(index_lista < list_size(dato_tmpc)){

		dato_a_analizar = list_get(dato_tmpc, index_lista);

		timestamp_a_analizar = obtener_timestamp_dato(dato_a_analizar);

		if(timestamp_a_analizar >= timestamp_mayor){

			free(dato_actualizado);

			length = string_length(dato_a_analizar);

			dato_actualizado = malloc(length);
			memcpy(dato_actualizado, dato_a_analizar, length + 1);

			timestamp_mayor = timestamp_a_analizar;

		}

	}

	return dato_actualizado;


}
