/*
 * metricas.c
 *
 *  Created on: 26 jul. 2019
 *      Author: utnso
 */

#include"metricas.h"

void* realizar_metrics(){

	while(1){

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		sleep(30);

		printf("\n>>>>>>>>>>>SE REALIZARAN LAS METRICAS<<<<<<<<<<<<<<<\n");

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		log_info(logger_metricas, ">>>>>>>>>>EJECUCION HILO METRICS<<<<<<<<<\n");

		pthread_rwlock_rdlock(&semaforo_metrica_sc);
		pthread_rwlock_rdlock(&semaforo_metrica_ec);
		pthread_rwlock_rdlock(&semaforo_metrica_shc);

		loggear_metricas("STRONG CONSISTENCY" , metrica_sc);

		loggear_metricas("EVENTUAL  CONSISTENCY" , metrica_ec);

		loggear_metricas("STRONG HASH CONSISTENCY" , metrica_shc);


		loggear_memory_load();

		pthread_rwlock_unlock(&semaforo_metrica_shc);
		pthread_rwlock_unlock(&semaforo_metrica_ec);
		pthread_rwlock_unlock(&semaforo_metrica_sc);

		pthread_rwlock_wrlock(&semaforo_metrica_sc);
		limpiar_metricas(metrica_sc);
		pthread_rwlock_unlock(&semaforo_metrica_sc);


		pthread_rwlock_wrlock(&semaforo_metrica_ec);
		limpiar_metricas(metrica_ec);
		pthread_rwlock_unlock(&semaforo_metrica_ec);


		pthread_rwlock_wrlock(&semaforo_metrica_shc);
		limpiar_metricas(metrica_shc);
		pthread_rwlock_unlock(&semaforo_metrica_shc);

		reiniciar_memory_load();

		printf("\n>>>>>>>>>>>FIN DE LAS METRICAS<<<<<<<<<<<<<<<\n");

	}

	return NULL;
}

void limpiar_metricas(metrica_t* metrica){

	void _liberar_valor(void* _valor){

		time_t* valor = (time_t* ) _valor;

		free(valor);

	}

	list_clean_and_destroy_elements(metrica->lista_insert , _liberar_valor);

	list_clean_and_destroy_elements(metrica->lista_select , _liberar_valor);

}

void inicializar_metricas(){

	inicializar_metrica(&metrica_sc);

	inicializar_metrica(&metrica_ec);

	inicializar_metrica(&metrica_shc);

}

void inicializar_metrica(metrica_t** metrica_a_init){

	*metrica_a_init = malloc(sizeof(metrica_t));

	(*metrica_a_init)->lista_insert = list_create();

	(*metrica_a_init)->lista_select = list_create();


}


void agregar_a_metrica(cod_operacion codigo_operacion , void* tipo_request, time_t tiempo_ejecucion){

	cod_consistencia consistencia = identificar_consitencia_para_request(codigo_operacion , tipo_request);

	switch(consistencia){

		case SC:

			pthread_rwlock_wrlock(&semaforo_metrica_sc);
			sumador_metrica(codigo_operacion , metrica_sc , tiempo_ejecucion);
			pthread_rwlock_unlock(&semaforo_metrica_sc);
			break;

		case EC:


			pthread_rwlock_wrlock(&semaforo_metrica_ec);
			sumador_metrica(codigo_operacion, metrica_ec , tiempo_ejecucion);
			pthread_rwlock_unlock(&semaforo_metrica_ec);
			break;

		case SHC:

			pthread_rwlock_wrlock(&semaforo_metrica_shc);
			sumador_metrica(codigo_operacion , metrica_shc , tiempo_ejecucion);
			pthread_rwlock_unlock(&semaforo_metrica_shc);
			break;

		default:

			log_error(logger_metricas , "NO SE PUDO IDENTIFICAR LA CONSISTENCIA");
			break;

	}


}


void sumador_metrica( cod_operacion codigo_operacion , metrica_t* metrica,  time_t tiempo_ejecucion ){

	time_t* tiempo_a_guardar = malloc(sizeof(time_t));

	memcpy(tiempo_a_guardar,  &tiempo_ejecucion, sizeof(time_t));


	switch(codigo_operacion){

		case SELECT:

			list_add(metrica->lista_select , tiempo_a_guardar);
			break;

		case INSERT:

			list_add(metrica->lista_insert, tiempo_a_guardar);
			break;

		default:

			log_error(logger_metricas , "NO ES UNA REQUEST VALIDAD.-\n");
			break;
	}


}


void reiniciar_memory_load(){


	void _reiniciar_contador(void * _memoria){

		memoria_t* memoria = (memoria_t* ) _memoria;

		memoria->contador_requests = 0;


	}

	pthread_rwlock_wrlock(&semaforo_tabla_gossiping);
	list_iterate(tabla_gossiping  ,  _reiniciar_contador);
	pthread_rwlock_unlock(&semaforo_tabla_gossiping);
}


void request_metrics(){

	printf("\n>>>>>>>>>>>>>METRICAS<<<<<<<<<<<<<<<\n");

	printf("\n   -->STRONG CONSISTENCY<--  \n");

	pthread_rwlock_rdlock(&semaforo_metrica_sc);
	mostrar_metricas(metrica_sc);
	pthread_rwlock_unlock(&semaforo_metrica_sc);

	printf("\n   -->EVENTUAL CONSISTENCY<--  \n");

	pthread_rwlock_rdlock(&semaforo_metrica_ec);
	mostrar_metricas(metrica_ec);
	pthread_rwlock_unlock(&semaforo_metrica_ec);

	printf("\n   -->STRONG HASH CONSISTENCY<--  \n");

	pthread_rwlock_rdlock(&semaforo_metrica_shc);
	mostrar_metricas(metrica_shc);
	pthread_rwlock_unlock(&semaforo_metrica_shc);

	mostrar_memory_load();
}

void mostrar_memory_load(){

	printf("\n--->MEMORY LOAD<---\n");

	void _mostrar_memory_load(void*  _memoria){

		memoria_t* memoria = (memoria_t* ) _memoria;

		printf("\nMemoria %d\n" , memoria->numero_memoria);

		printf(" -> Memory load: %d\n" , memoria->contador_requests);

	}

	pthread_rwlock_rdlock(&semaforo_tabla_gossiping);
	list_iterate(tabla_gossiping , _mostrar_memory_load);
	pthread_rwlock_unlock(&semaforo_tabla_gossiping);


}

void sumar_contador_memoria(memoria_t* memoria){

	//LOS SEMAFOROS ESTAN AFUERA

	printf("Le sumo 1 al contador de la memoria %d\n" , memoria->numero_memoria);

	memoria->contador_requests += 1;

	printf("nuevo_valor de contador: %d\n" , memoria->contador_requests);

}

void loggear_memory_load(){

	log_info(logger_metricas, "--->MEMORY LOAD<---\n");

	int total_requests;

	void _mostrar_memory_load(void*  _memoria){

		memoria_t* memoria = (memoria_t* ) _memoria;
		int porcentaje_memory_load;

		if(total_requests == 0){

			porcentaje_memory_load = 0;

		}else{

			porcentaje_memory_load = (memoria->contador_requests / total_requests) * 100 ;


		}

		printf("porcentaje: %d\n" , porcentaje_memory_load);

		log_info(logger_metricas , "Memoria %d : " , memoria->numero_memoria );
		log_info(logger_metricas, "%d Ejecuciones  ->  %d % Memory load\n" , memoria->contador_requests , porcentaje_memory_load);

	}

	pthread_rwlock_rdlock(&semaforo_tabla_gossiping);

	total_requests = total_ejecuciones();

	printf("total de request: %d\n" , total_requests);

	list_iterate(tabla_gossiping , _mostrar_memory_load);

	pthread_rwlock_unlock(&semaforo_tabla_gossiping);


}

int total_ejecuciones(){

	int total_ejecuciones = 0;


	total_ejecuciones += total_ejecucion_de_metrica(metrica_sc);

	total_ejecuciones += total_ejecucion_de_metrica(metrica_ec);

	total_ejecuciones += total_ejecucion_de_metrica(metrica_shc);

	return total_ejecuciones;
}

int total_ejecucion_de_metrica(metrica_t* metrica){

	return (list_size(metrica->lista_insert) + list_size(metrica->lista_select));

}


void loggear_metricas(char* consistencia , metrica_t* metrica){

	int read_latency = 0;
	int write_latency = 0;

	if(!list_is_empty(metrica->lista_select)){

		read_latency = sumatoria_tiempos(metrica->lista_select) / list_size(metrica->lista_select);

	}

	if( !list_is_empty(metrica->lista_insert) ){

		write_latency = sumatoria_tiempos(metrica->lista_insert) / list_size(metrica->lista_insert);

	}

	log_info(logger_metricas , ">>%s<<\n" , consistencia);

	log_info(logger_metricas , ">Read Latency : %d" , read_latency);
	log_info(logger_metricas , ">Write Latency : %d" , write_latency);
	log_info(logger_metricas , ">Reads : %d" , list_size(metrica->lista_select));
	log_info(logger_metricas , ">Writes : %d\n" , list_size(metrica->lista_insert));



}

void mostrar_metricas(metrica_t* metrica){

	int seed = 0 ;
	int read_latency = 0 ;
	int write_latency = 0;


	if(!list_is_empty(metrica->lista_select)){

		read_latency = sumatoria_tiempos(metrica->lista_select) / list_size(metrica->lista_select);

	}

	if( !list_is_empty(metrica->lista_insert) ){

		write_latency = sumatoria_tiempos(metrica->lista_insert) / list_size(metrica->lista_insert);

	}

	printf(" ->Read Latency : %d\n" , read_latency);
	printf(" ->Write Latency : %d\n" , write_latency);
	printf(" ->Reads : %d\n" , list_size(metrica->lista_select));
	printf(" ->Writes : %d\n\n" , list_size(metrica->lista_insert));

}

int sumatoria_tiempos (t_list* lista_a_sumar){

	time_t sumatoria = 0;

	void* _sumar(void* _numero){

		time_t* tiempo = (time_t*) _numero;

		sumatoria += *tiempo;

		return NULL;
	}

	list_iterate(lista_a_sumar, _sumar);

	return sumatoria;

}
