/*
 * kernel.c
 *
 *  Created on: 15 abr. 2019
 *      Author: utnso
 */

#include "kernel.h"

int main (int argc , char* argv[]){

	creacion_del_config();
	obtener_datos_config();


	t_queue* colas_exec[grado_multiprocesamiento];
	inicializar_cola_exec(colas_exec , grado_multiprocesamiento);
	inicializar_cola_new();
	inicializar_cola_ready();
	inicializar_cola_exit();

	for (int i = 1; i < argc ; i++){
		// pone los scripts en la cola de new
		queue_push(cola_new , argv[i]);

	}

	cola_new_to_ready();

	cola_ready_a_exec(colas_exec[0]);
	cola_ready_a_exec(colas_exec[0]);
	cola_ready_a_exec(colas_exec[0]);

	printf("\nFIN DEL PROGRAMA\n");



return 0;
}




