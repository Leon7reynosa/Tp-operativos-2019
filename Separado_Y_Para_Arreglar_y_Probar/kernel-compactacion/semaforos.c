/*
 * semaforos.c
 *
 *  Created on: 20 jun. 2019
 *      Author: utnso
 */

#include"semaforos.h"


void inicializar_semaforo_ready(){

	sem_init(&semaforo_ready , 0, 0);

}
