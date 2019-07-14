
#include"compactador.h"

void* compactar(char* nombre_tabla){

	printf("Empieza la compactacion\n");
	//los comentarios entre parentesis son para ver donde se libera cada variable;

	printf("1) Transformo los tmp en tmpc\n");
	transformar_tmp_a_tmpc(nombre_tabla);

/*	t_list* datos_particiones;*/                                                             // (2) Lista con los datos de los .bin antes de la compactacion

	t_list* datos_tmpc;																				// (3) Lista con los datos de los .tmpc

	t_list* datos_finales;																		// (4)  Lista con los datos de los .bin DESPUES de la particion

	printf("2) Obtengo la metadata\n");
	metadata_t* metadata_tabla = obtener_metadata(nombre_tabla);

	printf("hola %i\n", metadata_tabla->particion);

	printf("3) Obtengo los datos de las particiones\n");
	datos_finales = obtener_datos_particiones(nombre_tabla);

	printf("4) Obtengo los datos de los tmpc\n");
	datos_tmpc = obtener_datos_temporales(nombre_tabla);

	void _generar_lista_datos_finales(void* _dato_tmpc){

		char* dato_tmpc = (char *)_dato_tmpc;

		printf("dato_tpmc = %s\n" , dato_tmpc);

		//t_list* dato_con_key_particular = filtrar_dato_por_key(dato_particiones, datos_tmpc);   // (1) Lista filtrada

		int index = buscar_index_misma_key( dato_tmpc, datos_finales  );

		printf("index: %d\n" , index);

		if(index >= 0){

			printf("hola estoy en index > 0 \n");

			char* dato_timestamp_mayor = comparar_y_obtener_dato_actualizado( (char*) list_get(datos_finales, index) , dato_tmpc);

			printf("timestamp actualizado: %s\n" , dato_timestamp_mayor);

			list_replace_and_destroy_element(datos_finales , index  , dato_timestamp_mayor , free);

			printf("dato en la lista es %s en el index %d\n" , ( (char*) list_get(datos_finales, index) ) , index );

		}else{

			printf("hola estoy en index < 0 \n");

			char* aux_dato_tmpc = string_duplicate(dato_tmpc);

			list_add(datos_finales , aux_dato_tmpc);

		}
                                              // (1) Libero lista filtrada SIN liberar los elementos

	}

	printf("5) Por cada dato de las particiones, comparo y obtengo los datos finales\n");

	printf("el dato esta bien : %s\n" ,   ( (char* ) list_get(datos_finales , 1)  ));

	list_iterate(datos_tmpc, _generar_lista_datos_finales);

	//ACA EMPIEZA EL PROCESO DE SACAR Y PONER
	//MUTEX?
	//HAY QUE BLOQUEAR PARA QUE NADIE OPERE SOBRE ESTA TABLA! SI NO SE PUDRE TODOO

	//LIBERO LAS PARTICIONES, DEBERIA QUEDARME NUEVOS .bin CON SIZE=0 y 1 BLOQUE

	printf("6) LIBERO LAS PARTICIONES\n ");

	for(int i = 0; i < metadata_tabla->particion; i++){

		char* path_particion = obtenerPath_ParticionTabla(nombre_tabla, i);

		liberar_bloques_particion(path_particion);

		free(path_particion);
	}

	void _funcion_loca2(void* _dato_final){

		char* dato_final = (char *)_dato_final;

		printf("Dato a cargar: %s\n", dato_final);

		u_int16_t key = obtener_key_dato(dato_final);

		printf("Key: %i\n", key);

		int particion_perteneciente = calcular_particion(metadata_tabla->particion, key );

		printf("Esa key va a la particion: %i\n", particion_perteneciente);

		char* path_particion = obtenerPath_ParticionTabla(nombre_tabla, particion_perteneciente);  // (5)

		printf("Y el path de la particion: %s\n", path_particion);

		dato_t* dato_a_cargar = convertir_a_dato(dato_final);                                     // (6)

		printf("El dato a cargar entonces seria: \n");

		printf("Key: %i\n", dato_a_cargar->key);

		printf("timestamp: %i\n", dato_a_cargar->timestamp);

		printf("Value: %s\n", dato_a_cargar->value);

		printf("Procedo a cargarlo a la particion ;)\n");

		cargar_a_particion(path_particion, dato_a_cargar);

		printf("Ya cargo en la particion el dato\n");

		printf("Libero estructuras admin\n");

		free(path_particion);                                                                    // (5)

		liberar_dato(dato_a_cargar);															 // (6)



	}

	printf("7) Por cada dato final (son %i datos), lo agrego a su correspondiente particion\n", list_size(datos_finales));
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
//	list_destroy_and_destroy_elements(datos_particiones, free);										//  (2) Libero los datos del .bin ANTES de la compactacion
	printf("10) Libero datos_tmpc\n");
	list_destroy_and_destroy_elements(datos_tmpc, free);											//  (3) Libero los datos del .tmpc
	printf("11) Libero datos_finales\n");
	list_destroy_and_destroy_elements(datos_finales, free);										// (4) Libero los datos del .bin DESPUES de la compactacion

	//TODO liberar_metadata(metadata_tabla); resuelto abajo por ahora, seguramente cambie la estructura metadata!
	free(metadata_tabla);

	return NULL;

}

/////////////////////////////////FUNCIONES COMPACTADOR HILO/////////////////////////////////////77


thread_args* crear_argumentos_tabla( int tiempo_compactacion , char* nombre_tabla ){

	thread_args* argumentos_hilo = malloc(sizeof(thread_args));

	argumentos_hilo->tiempo_compactacion = tiempo_compactacion;

	argumentos_hilo->nombre_tabla = malloc(strlen(nombre_tabla ) + 1);

	memcpy(argumentos_hilo->nombre_tabla , nombre_tabla , (strlen(nombre_tabla) + 1));

	return argumentos_hilo;
}

void correr_compactacion(int tiempo_compactacion , char* nombre_tabla){

	thread_args* argumentos_hilos = crear_argumentos_tabla(tiempo_compactacion , nombre_tabla);

	pthread_t hilo_tabla;

	pthread_create(hilo_tabla , NULL , ciclo_compactacion , argumentos_hilos);

	dictionary_put(diccionario_compactador , nombre_tabla , &hilo_tabla);


}

void* ciclo_compactacion(thread_args* argumentos){

	while(1){

		usleep(argumentos->tiempo_compactacion);

		compactar(argumentos->nombre_tabla);

	}

	return NULL;
}

void abortar_hilo_compactador(char* nombre_tabla){

	void _abortar_hilo(void* hilo){

		pthread_t hilo_a_destruir = (pthread_t) hilo;

		pthread_cancel(hilo_a_destruir);

	}


	dictionary_remove_and_destroy(diccionario_compactador, nombre_tabla, _abortar_hilo );

}

////////////////////////////////////////////////////////////////////////////////////////777

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

//No pasarle null :)
char* comparar_y_obtener_dato_actualizado(char* dato_a_ser_comparado, char* dato_a_analizar){

	char* dato_actualizado = string_duplicate(dato_a_ser_comparado);

	time_t timestamp_mayor = obtener_timestamp_dato(dato_a_ser_comparado);

	time_t timestamp_menor= obtener_timestamp_dato(dato_a_analizar);

	if(timestamp_menor >= timestamp_mayor){

		free(dato_actualizado);

		dato_actualizado = string_duplicate(dato_a_analizar);
		timestamp_mayor = timestamp_menor;

	}

	return dato_actualizado;

}


int buscar_index_misma_key(char*  dato_tmpc, t_list*  datos_finales  ){

	printf("hola entre al buscar_index_misma_key\n");

	for( int i = 0 ; i < list_size(datos_finales) 	; i++){

		printf("entre en la iteracion %d\n" , i);

		printf("el key 1 es: %d\n", obtener_key_dato(dato_tmpc) );

		printf("dato malo : %s\n" , (char*)list_get(datos_finales, i));

		printf("el key 1 es: %d\n" , obtener_key_dato( (char*)list_get(datos_finales, i) ));

		if( obtener_key_dato(dato_tmpc) == obtener_key_dato( (char*)list_get(datos_finales, i) ) ){

			return i;

		}

	}

	return -1;

}
